#include "EcalFunctions.h"

#include "TGeoManager.h"
#include "TGeoNode.h"
#include "TObjArray.h"
#include "TObjString.h"


using namespace std;

EcalFunctions::EcalFunctions(){
	c1 = new TCanvas("c1", "c1", 800, 600);
	c1->cd();

	gROOT->SetBatch(true);
	gStyle->SetOptStat("");

	histCounter = 0;
}


EcalFunctions::~EcalFunctions(){
	delete c1;
}



vector<TString> EcalFunctions::Split(TString input, TString delim){
	TObjArray *tx = input.Tokenize(delim);

	vector<TString> splits;

	TString filename = ((TObjString *)(tx->At(tx->GetEntries()-1)))->String();

	for (Int_t i = 0; i < tx->GetEntries(); i++)
		splits.push_back(((TObjString *)(tx->At(i)))->String());

	return splits;
}



TGeoNode* EcalFunctions::GetNode(TString absoluteNodeName){

	TGeoManager *geom = gGeoManager;
	if(!geom){
		cout << endl << "Problem getting GeoManager!" << endl;
		return 0;
	}


	vector<TString> nodeNames = EcalFunctions::Split(absoluteNodeName, "/");

	TGeoNode* currentNode = geom->GetTopNode();

	for(unsigned int i=0;i<nodeNames.size();i++){
		currentNode = currentNode->GetVolume()->GetNode(nodeNames[i]);

		if(!currentNode){
			cout << "Can't get child node '" << nodeNames[i] << "' of node '" << currentNode->GetName() << "'!"<< endl;
			return 0;
		}
	}

	return currentNode;
}


void EcalFunctions::MakeThetaPhiMap(TString savename, std::vector<EcalHit*> hits){
	vector<float> E;
	vector<float> Phi;
	vector<float> Theta;
	vector<float> PhiOrig;
	vector<float> ThetaOrig;


	for(const auto& hit : hits){
		E.push_back(hit->GetE());
		Phi.push_back(hit->GetPhi());
		Theta.push_back(hit->GetTheta());

		TVector3 _direction = TVector3(hit->x_orig, hit->y_orig, hit->z_orig);
		PhiOrig.push_back(_direction.Phi());
		ThetaOrig.push_back(_direction.Theta());
	}

	TH2F* hist = GetThetaPhiHistogram(Phi, Theta, E);
	TH2F* histOrig = GetThetaPhiHistogram(PhiOrig, ThetaOrig, E);

	DrawHistogram(savename, hist);
	DrawHistogram(savename+"_Original", histOrig);
}


void EcalFunctions::MakeThetaPhiMap(TString savename, std::vector<EcalCluster*> clusters){

	vector<TH2F*> hists;
	vector<TH2F*> histsOrig;

	for(const auto& cluster : clusters){
		vector<float> E;
		vector<float> Phi;
		vector<float> Theta;
		vector<float> PhiOrig;
		vector<float> ThetaOrig;

		for(const auto& hit : cluster->GetHits()){
			E.push_back(hit->GetE());
			Phi.push_back(hit->GetPhi());
			Theta.push_back(hit->GetTheta());

			TVector3 _direction = TVector3(hit->x_orig, hit->y_orig, hit->z_orig);
			PhiOrig.push_back(_direction.Phi());
			ThetaOrig.push_back(_direction.Theta());
		}

		hists.push_back(GetThetaPhiHistogram(Phi, Theta, E));
		histsOrig.push_back(GetThetaPhiHistogram(PhiOrig, ThetaOrig, E));
	}

	DrawHistograms(savename, hists);
	DrawHistograms(savename+"_Original", histsOrig);

}


TH2F* EcalFunctions::GetThetaPhiHistogram(std::vector<float> Phi, std::vector<float> Theta, std::vector<float> E){
	TH2F* thetaPhiMap = new TH2F("hist_"+EcalFunctions::ToString(histCounter), "", 100, 1.4, 1.7, 100, 1.4, 1.7);

	thetaPhiMap->GetXaxis()->SetTitle("#phi [rad]");
	thetaPhiMap->GetYaxis()->SetTitle("#theta [rad]");

	for(unsigned int i=0;i<Phi.size();i++)
			thetaPhiMap->Fill(Phi[i], Theta[i], E[i]);

	histCounter += 1;

	return thetaPhiMap;
}


void EcalFunctions::DrawHistogram(TString savename, TH2F* hist){
	c1->cd();

	hist->Draw("colz");
	c1->SaveAs(savename+".pdf");
}


void EcalFunctions::DrawHistograms(TString savename, vector<TH2F*> hists){
	c1->cd();

	if(hists.size() == 0)
		return;

	for(int i=0;i<hists.size();i++){
		int col = i + 2;
		if(col >= 5)
			col += 1;

		hists[i]->SetLineColor(col);
		hists[i]->SetMarkerColor(col);

		if(i == 0)
			hists[i]->Draw();
		else
			hists[i]->Draw("same");
	}


	c1->SaveAs(savename+".pdf");
}


// void EcalFunctions::MakeThetaPhiMap(TString savename, std::vector<float> Phi, std::vector<float> Theta, std::vector<float> E){
// 	TH2F* thetaPhiMap = GetThetaPhiHistogram(Phi, Theta, E);
//
// 	c1->cd();
// 	thetaPhiMap->Draw("colz");
// 	c1->SaveAs(savename+".pdf");
//
// 	delete thetaPhiMap;
// }



