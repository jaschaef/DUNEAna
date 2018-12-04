#include <iostream>
#include <string>

#include "EcalHit.h"
#include "EcalCluster.h"

#include "TFile.h"
#include "TString.h"


using namespace std;


int main(int argc, char* argv[]) {

	// if(argc!=2) {
	//    cerr << "Configuration file missing!" << endl
	//         << "Usage " << argv[0] << " <config.xml>" << endl;
	//    return 1;
	// }

	TString geoFile = "ND_Strawman_Concept_v01.gdml";
	bool saveGeoManager = false;

	if(argc > 0)
		geoFile = TString(argv[1]);


	cout << " >>>>> Running the EcalTest <<<<<" << endl << endl;

	TGeoManager *geom = 0;
	TFile* infile = 0; 

	if(geoFile.Contains("gdml"))
		geom = TGeoManager::Import(geoFile);
	else{
		infile = TFile::Open(geoFile);
		geom = (TGeoManager*) infile->Get("FAIRGeom");
	}
		
	if(!geom){
		cout << endl << "Problem loading geometry!" << endl;
		return 1;
	}

	cout << "Done loading gdml file." << endl << endl;

	if(saveGeoManager){
		vector<TString> splits = EcalHit::Split(geoFile, "/");
		TString fname = EcalHit::Split(splits[splits.size()-1], ".")[0]+".root";

		cout << " Saving GeoFile as '" << fname << "'." << endl;

		TFile* geomSaveFile = TFile::Open(fname, "RECREATE");

		geomSaveFile->cd();
		geom->Write();
		geomSaveFile->Write();
		geomSaveFile->Close();
	}

	// TString motherPath = "volDetEnclosure_0/volNDHPgTPC_0/InnerBarrelECal_vol_0/InnerBarrelECal_I0_L8_vol_0/IBStripOne_vol_0";
	// TString nodeName = "IBStripOne_L1_vol_0";
	// TString motherPath = "volDetEnclosure_0/volNDHPgTPC_0/InnerBarrelECal_vol_0/InnerBarrelECal_I1_L8_vol_0/IBStripTwo_vol_0";
	// TString nodeName = "IBStripTwo_L1_vol_0";
	// TString motherPath = "volDetEnclosure_0/volNDHPgTPC_0/InnerEndcapECal_vol_0/IECLayerOne_vol_0";
	// TString nodeName = "IECLayerOne_L1_vol_0";
	// TGeoNode* testNode = EcalHit::GetNode(motherPath + "/" + nodeName);

	// cout << "Starting frome node:" << motherPath + "/" + nodeName << endl;

	// EcalHit* eh = new EcalHit(10., testNode, "Default/volWorld_1/"+motherPath);
	// eh->Print();

	// EcalHit* eh2 = new EcalHit(eh->GetE(), eh->GetX(), eh->GetY(), eh->GetZ());
	// eh2->Print();

	std::vector<EcalHit*> debugHits;
	EcalHit* debugHit = 0;
	debugHit = new EcalHit(0.00108022685163, 0.0, 0.0, 162.726455688);
	debugHits.push_back(debugHit);
	debugHit = new EcalHit(0.00192801153753, -0.00128100172151, 0.0166151933372, 163.399993896);
	debugHits.push_back(debugHit);
	debugHit = new EcalHit(0.00193497422151, 0.0837961658835, 0.0804075524211, 164.600006104);
	debugHits.push_back(debugHit);
	debugHit = new EcalHit(0.00298676453531, 0.499042063951, -0.19498474896, 165.800003052);
	debugHits.push_back(debugHit);
	debugHit = new EcalHit(0.00208172108978, 0.91620349884, -0.941529154778, 167.0);
	debugHits.push_back(debugHit);
	debugHit = new EcalHit(0.00332155753858, 0.447562724352, -0.183049425483, 165.800003052);
	debugHits.push_back(debugHit);
	debugHit = new EcalHit(0.00197000149637, 0.0672629848123, 0.274432271719, 165.800003052);
	debugHits.push_back(debugHit);
	debugHit = new EcalHit(0.00157540373039, -0.0466206036508, 0.487023442984, 166.307022095);
	debugHits.push_back(debugHit);
	debugHit = new EcalHit(2.91009996545e-07, 0.559882164001, 0.576713204384, 177.800003052);
	debugHits.push_back(debugHit);
	debugHit = new EcalHit(0.000142001328641, 0.0, 0.0, 162.726455688);
	debugHits.push_back(debugHit);
	debugHit = new EcalHit(0.000116217866889, -5.10808314274e-17, -0.00720243295655, 162.825912476);
	debugHits.push_back(debugHit);
	debugHit = new EcalHit(0.000841665198095, -3.46944695195e-17, -0.00986553635448, 162.896408081);
	debugHits.push_back(debugHit);
	debugHit = new EcalHit(0.00153199280612, 0.89878821373, 0.920054912567, 182.600006104);
	debugHits.push_back(debugHit);
	debugHit = new EcalHit(6.76444833516e-05, 0.581693470478, 0.598888516426, 178.366699219);
	debugHits.push_back(debugHit);
	debugHit = new EcalHit(0.000358581339242, -1.87733447552, 2.8905134201, 176.600006104);
	debugHits.push_back(debugHit);
	debugHit = new EcalHit(0.00187426886987, -1.88883364201, 2.90827870369, 176.683914185);
	debugHits.push_back(debugHit);

	vector<EcalHit*> hits = debugHits; //{eh, eh2};

	vector<EcalHit*> cells = EcalHit::AddCells(hits);

	cout << "Merged " << hits.size() << " hits to " << cells.size() << " cells." << endl;

	// cout << endl << "Cells:" << endl;
	// for(unsigned int i=0;i<cells.size();i++)
	// 	cells[i]->Print();

	
	vector<EcalCluster*> clusters = EcalCluster::GetClusters(cells);
	cout << "Merged " << cells.size() << " cells to " << clusters.size() << " clusters." << endl;

	for(const auto& cluster : clusters)
		cluster->ComputeEtaPhiE();


	if(infile != 0){
		infile->Close();
		delete infile;		
	}


	

	return 0;

}
