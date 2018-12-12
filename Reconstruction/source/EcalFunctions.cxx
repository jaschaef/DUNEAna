#include "EcalFunctions.h"

#include "TGeoManager.h"
#include "TGeoNode.h"
#include "TObjArray.h"
#include "TObjString.h"
#include "TPolyLine3D.h"
#include "TPolyMarker3D.h"
#include "TView3D.h"


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


float EcalFunctions::Round(float f, int n){
	float fac = TMath::Power(10., n);
	float F= float(round(f*fac)/fac);

	// hack to avoid "-0"
	if(F == 0.)
		F = 0.;

	return F;
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
			// cout << "Can't get child node '" << nodeNames[i] << "' of node '" << currentNode->GetName() << "'!"<< endl;
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
	MakeThetaPhiMap(savename, clusters, {});
}


void EcalFunctions::MakeThetaPhiMap(TString savename, std::vector<EcalCluster*> clusters, std::vector<std::pair<float,float> > extraPoints){

	vector<TH2F*> hists;
	vector<TH2F*> histsOrig;
	vector<TH2F*> extraHists;

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

	for(unsigned int i=0;i<extraPoints.size();i++){
		vector<float> Phi = {extraPoints[i].first};
		vector<float> Theta = {extraPoints[i].second};
		vector<float> E = {1.};

		TH2F* extraHist = GetThetaPhiHistogram(Phi, Theta, E);
		extraHists.push_back(extraHist);
	}

	DrawHistograms(savename, hists, extraHists);
	DrawHistograms(savename+"_Original", histsOrig, extraHists);

}


TH2F* EcalFunctions::GetThetaPhiHistogram(std::vector<float> Phi, std::vector<float> Theta, std::vector<float> E){
	TH2F* thetaPhiMap;

	// if(EcalHit::transformInput)
	// 	thetaPhiMap = new TH2F("hist_"+EcalFunctions::ToString(histCounter), "", 100, 1.2, 1.9, 100, 1.2, 1.9);
	// else
	// 	thetaPhiMap = new TH2F("hist_"+EcalFunctions::ToString(histCounter), "", 100, 0, 3.2, 100, 0, 0.15);

	thetaPhiMap = new TH2F("hist_"+EcalFunctions::ToString(histCounter), "", 640, -3.2, 3.2, 320, 0, 3.2);

	thetaPhiMap->GetXaxis()->SetTitle("#phi [rad]");
	thetaPhiMap->GetYaxis()->SetTitle("#theta [rad]");

	for(unsigned int i=0;i<Phi.size();i++)
			thetaPhiMap->Fill(Phi[i], Theta[i], E[i]);

	histCounter += 1;

	return thetaPhiMap;
}


void EcalFunctions::DrawHistogram(TString savename, TH2F* hist){
	c1->cd();

	hist->Draw("box");
	c1->SaveAs(savename+".pdf");
}


void EcalFunctions::DrawHistograms(TString savename, vector<TH2F*> hists){
	DrawHistograms(savename, hists, {});
}

void EcalFunctions::DrawHistograms(TString savename, vector<TH2F*> hists, vector<TH2F*> extraHists){
	c1->cd();

	if(hists.size() == 0)
		return;

	bool hasDrawn = false;

	for(int i=0;i<hists.size();i++){
		int col = GetColor(i);

		hists[i]->SetLineColor(col);
		hists[i]->SetMarkerColor(col);

		if(!hasDrawn)
			hists[i]->Draw("box");
		else
			hists[i]->Draw("boxsame");

		hasDrawn = true;
	}

	for(int i=0;i<extraHists.size();i++){
		int col = GetColor(i);

		extraHists[i]->SetLineColor(col);
		extraHists[i]->SetMarkerColor(col);
		extraHists[i]->SetMarkerStyle(4);

		if(!hasDrawn)
			extraHists[i]->Draw();
		else
			extraHists[i]->Draw("same");

		hasDrawn = true;
	}



	c1->SaveAs(savename+".pdf");
}



void EcalFunctions::DrawEventDisplay(TString savename, vector<EcalCluster*> clusters){
	c1->cd();

	float xMin = 0.;
	float xMax = 350.;
	float yMin = -250.;
	float yMax = 10.;
	float zMin = -175.;
	float zMax = 10.;

	// float xMin = -30.;
	// float xMax = 30.;
	// float yMin = 0.;
	// float yMax = 200.;
	// float zMin = -30.;
	// float zMax = 30.;
    //
	// if(!EcalHit::transformInput){
	// 	xMin = -10.;
	// 	xMax = 10.;
	// 	yMin = -10.;
	// 	yMax = 10.;
	// 	zMin = 0.;
	// 	zMax = 200.;
	// }

	TH3F* baseHist = new TH3F("hist_"+ToString(histCounter), "", 100, xMin, xMax, 100, yMin, yMax, 100, zMin, zMax);
	histCounter += 1;

	baseHist->GetXaxis()->SetTitle("x [cm]");
	baseHist->GetYaxis()->SetTitle("y [cm]");
	baseHist->GetZaxis()->SetTitle("z [cm]");
	baseHist->Draw();

	vector<TH3F*> hists;
	vector<TPolyLine3D*> lines;
	vector<TPolyMarker3D*> markers;

	TVector3 lineStart;
	TVector3 lineEnd;

	for(int i=0;i<3;i++){
		if(i == 0){
			lineStart = TVector3(xMin, 0., 0.);
			lineEnd = TVector3(xMax, 0., 0.);
		}else if(i == 1){
			lineStart = TVector3(0., yMin, 0.);
			lineEnd = TVector3(0., yMax, 0.);
		}else{
			lineStart = TVector3(0., 0., zMin);
			lineEnd = TVector3(0., 0., zMax);
		}

		TPolyLine3D* line_origin = new TPolyLine3D(2);
		line_origin->SetPoint(0, lineStart.X(), lineStart.Y(), lineStart.Z());
		line_origin->SetPoint(1, lineEnd.X(), lineEnd.Y(), lineEnd.Z());
		line_origin->SetLineColor(1);
		line_origin->SetLineStyle(2);
		lines.push_back(line_origin);
	}


	for(unsigned int i=0;i<clusters.size();i++){
		// histogram of hits
		TH3F* hist = (TH3F*) baseHist->Clone();

		int col = GetColor(i);

		hist->SetLineColor(col);
		hist->SetMarkerColor(col);

		for(const auto& hit : clusters[i]->GetHits())
			hist->Fill(hit->GetX(), hit->GetY(), hit->GetZ(), hit->GetE());

		hists.push_back(hist);

		// fitted line
		TVector3 dir = clusters[i]->GetDirection();
		lineStart = clusters[i]->GetPoint() - dir * 100.;
		lineEnd = clusters[i]->GetPoint() + dir * 100.;

		TPolyLine3D* line = new TPolyLine3D(2);
		line->SetPoint(0, lineStart.X(), lineStart.Y(), lineStart.Z());
		line->SetPoint(1, lineEnd.X(), lineEnd.Y(), lineEnd.Z());
		line->SetLineColor(col);
		lines.push_back(line);

		lineStart = TVector3(0., 0., 0.);
		lineEnd = clusters[i]->GetPoint() * 2.;

		TPolyLine3D* line2 = new TPolyLine3D(2);
		line2->SetPoint(0, lineStart.X(), lineStart.Y(), lineStart.Z());
		line2->SetPoint(1, lineEnd.X(), lineEnd.Y(), lineEnd.Z());
		line2->SetLineColor(col);
		line2->SetLineStyle(2);
		lines.push_back(line2);


		// markers
		lineStart = clusters[i]->GetPoint();
		TPolyMarker3D* marker = new TPolyMarker3D(1);
		marker->SetPoint(0, lineStart.X(), lineStart.Y(), lineStart.Z());
		marker->SetMarkerColor(1);
		marker->SetMarkerStyle(4);
		markers.push_back(marker);

		// hist->Draw("boxsame");
		// line->Draw("same");
		// line2->Draw("same");
	}

	for(const auto& hist : hists)
		hist->Draw("boxsame");
	for(const auto& marker : markers)
		marker->Draw("boxsame");
	for(const auto& line : lines)
		line->Draw("same");

	c1->SaveAs(savename+".pdf");

	TView3D* tview = new TView3D();
	tview->RotateView(0, 90, c1);
	c1->SaveAs(savename+"_yz.pdf");
	tview->RotateView(-90, 0, c1);
	c1->SaveAs(savename+"_xy.pdf");

	for(const auto& line : lines)
		delete line;
	for(const auto& hist : hists)
		delete hist;
	for(const auto& marker : markers)
		delete marker;
}


int EcalFunctions::GetColor(int index){
	int col = index + 2;
	if(col >= 5)
		col += 1;

	return col;
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



