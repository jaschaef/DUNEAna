#ifndef ECALFUNCTIONS_H
#define ECALFUNCTIONS_H

#include "TROOT.h"
#include "TString.h"
#include <TStyle.h>
#include "TCanvas.h"
#include "TH2F.h"

#include "iostream"
#include "sstream"

#include "EcalHit.h"
#include "EcalCluster.h"


class EcalFunctions{

public:
	EcalFunctions();
	~EcalFunctions();

	void MakeThetaPhiMap(TString savename, std::vector<EcalHit*> hits);
	void MakeThetaPhiMap(TString savename, std::vector<EcalCluster*> clusters);
	// void MakeThetaPhiMap(TString savename, std::vector<float> Phi, std::vector<float> Theta, std::vector<float> E);

	TH2F* GetThetaPhiHistogram(std::vector<float> Phi, std::vector<float> Theta, std::vector<float> E);
	void DrawHistogram(TString savename, TH2F* hist);
	void DrawHistograms(TString savename, std::vector<TH2F*> hists);


	// static TString GetNavigatorPath(TGeoNode* _node, TGeoNode* _masterNode, TString _result = "");
	static TGeoNode* GetNode(TString absoluteNodeName);

	static std::vector<TString> Split(TString input, TString delim);
	template <typename T> static TString ToString(T a){
		std::ostringstream ss;
		ss << a;
		std::string s(ss.str());
		TString result(s);
		return result;
	}

	struct Energy_sort {
		bool operator()(EcalCluster* ec1, EcalCluster* ec2){
			return (ec1->GetE() > ec2->GetE());
		}

		bool operator()(EcalHit* eh1, EcalHit* eh2){
			return (eh1->GetE() > eh2->GetE());
		}
	};


private:
	TCanvas* c1;
	unsigned int histCounter;

};

#endif
