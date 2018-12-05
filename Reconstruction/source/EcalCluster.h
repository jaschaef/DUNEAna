#ifndef ECALCLUSTER_H
#define ECALCLUSTER_H

#include "TROOT.h"
#include "TVector3.h"
#include "TString.h"

#include "EcalHit.h"


class EcalCluster{

public:
	EcalCluster();
	EcalCluster(EcalHit* hit);
	EcalCluster(const EcalCluster& cluster);
	~EcalCluster();


	void AddHit(EcalHit* hit);
	void AddCluster(EcalCluster* cluster);
	void ClearHits();
	void ComputeEtaPhiE();
	void Print();

	float GetE(){return E;}
	float GetX(){return x;}
	float GetY(){return y;}
	float GetZ(){return z;}
	// float GetDX(){return dx;}
	// float GetDY(){return dy;}
	// float GetDZ(){return dz;}

	float GetPhi(){return phi;}
	float GetTheta(){return theta;}
	TVector3 GetDirection(){return direction;}
	float GetDistToOrigin();

	std::vector<EcalHit*> GetHits(){return hits;}

	static std::vector<EcalCluster*> GetClusters(std::vector<EcalHit*> hits);
	static std::vector<EcalCluster*> GetAntiKtClusters(std::vector<EcalHit*> hits, double p = -2.);

	float GetAntiKtDistance(EcalCluster* cluster, double p);


	static double GetWeight(const double *xx);


private:
	float E;

	// absolute positioning
	float x; //, dx;
	float y; //, dy;
	float z; //, dz;

	TVector3 direction;

	float phi, theta;

	std::vector<EcalHit*> hits;

public:
	static float energyThreshold;
	static float AntiKtRadius;
	static std::vector<EcalHit*> staticHits;
	static float static_x;
	static float static_y;
	static float static_z;

};


#endif
