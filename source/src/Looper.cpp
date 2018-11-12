#include "Looper.h"

#include "TChain.h"
#include "TLorentzVector.h"

#include <iostream>

Looper::Looper(Utils U, std::string sim)
: _U(U), _sim(sim), _isEndcap(false), vhist(), vdebug(), _nEvtSim(0)
{

}

Looper::~Looper()
{

}

bool Looper::Init()
{
    _U.BookTFile();
    _U.GetListOfFiles();
    _vecFile = _U.GetVectorFiles();

    if(_vecFile.size() == 0) return false;

    std::cout << "Number of files found " << _vecFile.size() <<std::endl;

    _hEnergySumSim = new TH1F("_hEnergySumSim", "_hEnergySumSim", 100, 0, 600);
    _hEnergySumSim->SetDirectory(0);

    _hEnergySumReco = new TH1F("_hEnergySumReco", "_hEnergySumReco", 100, 0, 600);
    _hEnergySumReco->SetDirectory(0);

    _hEnergySumDigi = new TH1F("_hEnergySumDigi", "_hEnergySumDigi", 100, 0, 600);
    _hEnergySumDigi->SetDirectory(0);

    #if DEBUG
    _hEnergySim = new TH1F("_hEnergySim", "_hEnergySim", 300, 0, 30);
    _hEnergySim->SetDirectory(0);

    _hEnergyLongSim = new TH1F("_hEnergyLongSim", "_hEnergyLongSim", 81, -0.5, 80.5);
    _hEnergyLongSim->SetDirectory(0);

    _hEnergySumLongSim = new TH1F("_hEnergySumLongSim", "_hEnergySumLongSim", 81, -0.5, 80.5);
    _hEnergySumLongSim->SetDirectory(0);

    _hnHitsSim = new TH1F("_hnHitsSim", "_hnHitsSim", 180, 0, 1800);
    _hnHitsSim->SetDirectory(0);

    m_EnergySumLayer.clear();
    #endif

    if(_sim == "art")
    {
        _chain = new TChain("caloanatree/ECALAnaTree");
        for(auto &file : _vecFile)
        _chain->Add(file.c_str());

        _chain->SetBranchAddress("MCPProc", &mcp_proc);
        _chain->SetBranchAddress("MCPEndX", &mcp_endx);
        _chain->SetBranchAddress("MCPEndY", &mcp_endy);
        _chain->SetBranchAddress("MCPEndZ", &mcp_endz);

        _chain->SetBranchAddress("SimHitID", &sim_id);
        _chain->SetBranchAddress("SimHitEnergy", &sim_energy);
        _chain->SetBranchAddress("SimHitX", &sim_x);
        _chain->SetBranchAddress("SimHitY", &sim_y);
        _chain->SetBranchAddress("SimHitZ", &sim_z);

        _chain->SetBranchAddress("DigiHitID", &digi_id);
        _chain->SetBranchAddress("DigiHitEnergy", &digi_energy);

        _chain->SetBranchAddress("RecoHitID", &reco_id);
        _chain->SetBranchAddress("RecoHitEnergy", &reco_energy);
    }

    if(_sim == "edep")
    {
        _chain = new TChain("EDepSimEvents");
        for(auto &file : _vecFile)
        _chain->Add(file.c_str());

        _chain->SetBranchAddress("Event", &event);
    }

    if(_U.GetTypeStr() == "Endcap") _isEndcap = true;

    _nEvtSim = 0;

    return true;
}

bool Looper::loop_art()
{
    int nConv = 0;

    //Run over the files
    for(unsigned int i = 0; i < _chain->GetEntries(); i++)
    {
        _chain->GetEntry(i);

        if(this->CheckIfConvertedInTracker()) { nConv++; continue; }

        float sim_esum = 0.;
        float digi_esum = 0.;
        float reco_esum = 0.;
        m_EnergySumLayer.clear();
        unsigned int nHits = 0;

        for(unsigned int e = 0; e < sim_energy->size(); e++)
        {
            sim_esum += sim_energy->at(e)*1000.;//GeV
            _hEnergySim->Fill(sim_energy->at(e)*1000.);

            TLorentzVector pos(sim_x->at(e)*10, sim_y->at(e)*10, sim_z->at(e)*10, 0);
            unsigned int layer = GetHitLayer(pos, sim_id->at(e));
            _hEnergyLongSim->Fill(layer, sim_energy->at(e)*1000.);

            if(m_EnergySumLayer.count(layer) == 0)
            m_EnergySumLayer[layer] = 0.;

            m_EnergySumLayer[layer] += sim_energy->at(e)*1000.;

            nHits++;
        }

        if(sim_esum != 0) {
            // std::cout << esum << std::endl;
            _hEnergySumSim->Fill(sim_esum);
            _nEvtSim++;

            for(std::map<unsigned int, float>::iterator it = m_EnergySumLayer.begin(); it != m_EnergySumLayer.end(); ++it)
            _hEnergySumLongSim->Fill(it->first, it->second);

            _hnHitsSim->Fill(nHits);
        }

        for(unsigned int e = 0; e < digi_energy->size(); e++)
        digi_esum += (float)(digi_energy->at(e)/(20.*25.)*0.814);

        if(digi_esum != 0) {
            // std::cout << esum << std::endl;
            _hEnergySumDigi->Fill(digi_esum);
        }


        for(unsigned int e = 0; e < reco_energy->size(); e++)
        reco_esum += reco_energy->at(e);

        if(reco_esum != 0) {
            // std::cout << esum << std::endl;
            _hEnergySumReco->Fill(reco_esum);
        }

    }

    std::cout << "Number of converted photons " << nConv << std::endl;
    std::cout << "Simulation, nEvt " << _nEvtSim << std::endl;

    return true;
}

bool Looper::loop_edep()
{
    int nConv = 0;
    for(unsigned int i = 0; i < _chain->GetEntries(); i++)
    {
        _chain->GetEntry(i);

        if (!event) {
            std::cout << "Event not available" << std::endl;
        }

        if(this->CheckIfConvertedInTracker()) { nConv++; continue; }

        float sim_esum = 0.;
        unsigned int nHits = 0;
        m_EnergySumLayer.clear();

        for (auto d = event->SegmentDetectors.begin(); d != event->SegmentDetectors.end(); ++d)
        {
            if(d->first == "TPC_Drift1" || d->first == "TPC_Drift2") continue;
            unsigned int id = 0;
            if(d->first == "IBStrip") id = 1;
            else if(d->first == "OBStrip") id = 2;
            else id = 3;

            for (std::vector<TG4HitSegment>::iterator h = d->second.begin(); h != d->second.end(); ++h)
            {
                _hEnergySim->Fill(h->EnergyDeposit);

                TLorentzVector pos(0.5*(h->Start + h->Stop));
                unsigned int layer = GetHitLayer(pos, id);
                _hEnergyLongSim->Fill(layer, h->EnergyDeposit);

                sim_esum += h->EnergyDeposit;//MeV

                if(m_EnergySumLayer.count(layer) == 0)
                m_EnergySumLayer[layer] = 0.;

                m_EnergySumLayer[layer] += h->EnergyDeposit;

                nHits++;
            }
        }

        if(sim_esum != 0) {
            // std::cout << esum << std::endl;
            _hEnergySumSim->Fill(sim_esum);
            _nEvtSim++;

            for(std::map<unsigned int, float>::iterator it = m_EnergySumLayer.begin(); it != m_EnergySumLayer.end(); ++it)
            _hEnergySumLongSim->Fill(it->first, it->second);

            _hnHitsSim->Fill(nHits);
        }
    }

    std::cout << "Number of converted photons " << nConv << std::endl;
    std::cout << "Simulation, nEvt " << _nEvtSim << std::endl;

    return true;
}

bool Looper::CheckIfConvertedInTracker()
{
    bool isConv = false;

    if(_sim == "art"){
        for(unsigned int e = 0; e < mcp_proc->size(); e++)
        {
            if(mcp_proc->at(e) == "primary"){

                float end_x = mcp_endx->at(e) - fOriginX;
                float end_y = mcp_endy->at(e) - fOriginY;
                float end_z = mcp_endz->at(e) - fOriginZ;

                if( !_isEndcap && std::sqrt(end_y*end_y + end_z*end_z) < 274 ) isConv = true;

                if( _isEndcap && std::abs(end_x) < 260 ) isConv = true;

                break;
            }
            else{ continue; }
        }
    }
    else{
        for (std::vector<TG4Trajectory>::iterator t = event->Trajectories.begin(); t != event->Trajectories.end(); ++t)
        {
            int trackId = t->TrackId;

            if(trackId != 0) continue;

            // std::cout << t->Name << " " << t->ParentId << " " << t->Points.size() << std::endl;

            std::vector<TG4TrajectoryPoint> points = t->Points;
            TLorentzVector pos = points.at(points.size() - 1).Position;

            float end_x = pos.X() - fOriginX*10;
            float end_y = pos.Y() - fOriginY*10;
            float end_z = pos.Z() - fOriginZ*10;

            if( !_isEndcap && std::sqrt(end_y*end_y + end_z*end_z) < 2740 ) isConv = true;

            if( _isEndcap && std::abs(end_x) < 2600 ) isConv = true;
        }
    }

    return isConv;
}

unsigned int Looper::GetHitLayer(TLorentzVector &pos, unsigned int id)
{
    unsigned int layer = 0;

    //mm
    float x = pos.X() - fOriginX*10;
    float y = pos.Y() - fOriginY*10;
    float z = pos.Z() - fOriginZ*10;

    float R = std::sqrt(y*y + z*z);

    if(id == 1)
    layer = std::floor( (std::abs(R) - fECALRinner*10) / (fECALLayerThickness*10) );//starts at 0
    else if(id == 2)
    layer = std::floor( (std::abs(R) - (fECALRinner*10 + fECALPVThickness*10)) / (fECALLayerThickness*10) );//starts at last InnerBarrelLayer + 1
    else
    layer = std::floor( (std::abs(x) - fEndcapStartXPosition*10) / (fECALLayerThickness*10) );//starts at 0

    // std::cout << layer << std::endl;

    return layer;
}

bool Looper::PerformGausFit()
{
    for(auto &h : vhist){
        if(h->GetEntries() < 100) continue;

        float mean = h->GetMean();
        float RMS = h->GetRMS();

        h->Fit("gaus", "+", "", mean-5*RMS, mean+5*RMS);
    }

    return true;
}

bool Looper::WriteHistograms()
{
    InsertAllHistograms();
    PerformGausFit();

    for(auto &h : vhist){
        float mean = h->GetMean();
        float RMS = h->GetRMS();

        h->GetXaxis()->SetRangeUser(mean-5*RMS, mean+5*RMS);

        _U.WriteHistogram(h);
    }

    #if DEBUG
    for(auto &h : vdebug) {
        h->Scale(1./_nEvtSim);
        _U.WriteHistogram(h);
    }

    #endif

    return true;
}

bool Looper::CloseTFile()
{
    _U.CloseTFile();
    return true;
}
