#ifndef __Looper_H__
#define __Looper_H__ 1

#include <string>
#include <vector>
#include <map>

#include "TChain.h"
#include "TH1.h"

#include "Utils.h"

#include "TG4Event.h"

#define DEBUG 1

class Looper
{
public:

    Looper(Utils U, std::string sim);

    ~Looper();

    bool Init();

    bool loop_art();

    bool loop_edep();

    bool WriteHistograms();

    bool CloseTFile();

protected:

    bool CheckIfConvertedInTracker();

    bool PerformGausFit();

    unsigned int GetHitLayer(TLorentzVector &pos, unsigned int id);

    void InsertAllHistograms()
    {
        vhist.push_back(_hEnergySumSim);
        vhist.push_back(_hEnergySumDigi);
        vhist.push_back(_hEnergySumReco);

        #if DEBUG
        vdebug.push_back(_hEnergySim);
        vdebug.push_back(_hEnergyLongSim);
        vdebug.push_back(_hEnergySumLongSim);
        vdebug.push_back(_hnHitsSim);
        #endif
    }

private:

    Utils _U;
    std::string _sim;

    bool _isEndcap;

    std::vector<std::string> _vecFile;

    TH1F* _hEnergySumSim;
    TH1F* _hEnergySumReco;
    TH1F* _hEnergySumDigi;

    #if DEBUG
    TH1F* _hEnergySim;
    TH1F* _hEnergyLongSim;
    TH1F* _hEnergySumLongSim;
    TH1F* _hnHitsSim;
    std::map<unsigned int, float> m_EnergySumLayer;
    #endif

    TChain* _chain;

    std::vector<TH1F*> vhist;
    #if DEBUG
    std::vector<TH1F*> vdebug;
    #endif

    //art
    std::vector<std::string> *mcp_proc = 0;
    std::vector<float> *mcp_endx = 0;
    std::vector<float> *mcp_endy = 0;
    std::vector<float> *mcp_endz = 0;

    std::vector<unsigned int> *sim_id = 0;
    std::vector<float> *sim_energy = 0;
    std::vector<float> *sim_x = 0;
    std::vector<float> *sim_y = 0;
    std::vector<float> *sim_z = 0;

    std::vector<unsigned int> *digi_id = 0;
    std::vector<unsigned int> *digi_energy = 0;

    std::vector<unsigned int> *reco_id = 0;
    std::vector<float> *reco_energy = 0;

    //edep-sim
    TG4Event* event = nullptr;

    const float fOriginX = 0.; //cm
    const float fOriginY = 218.196; //cm
    const float fOriginZ = 1124.02; //cm

    const float fECALRinner = 274.0; //cm
    const float fECALLayerThickness = 0.8; //cm
    const float fECALPVThickness = 0.1; //cm
    const float fEndcapStartXPosition = 260.0; //cm

    int _nEvtSim;
};

#endif
