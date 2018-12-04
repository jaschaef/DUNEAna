#ifndef ECALCLUSTER_H
#define ECALCLUSTER_H

#include "TROOT.h"
#include "TVector3.h"

#include "EcalHit.h"


class EcalCluster{

 public:
	EcalCluster();
	~EcalCluster();


	void AddHit(EcalHit* hit);
	void ClearHits();
	void ComputeEtaPhiE();
	void Print();

	float GetE(){return E;}
	float GetX(){return x;}
	float GetY(){return y;}
	float GetZ(){return z;}
	float GetDX(){return dx;}
	float GetDY(){return dy;}
	float GetDZ(){return dz;}
	float GetPhi(){return phi;}
	float GetTheta(){return theta;}
	TVector3 GetDirection(){return direction;}

	static std::vector<EcalCluster*> GetClusters(std::vector<EcalHit*> hits);


 private:
	float E;

	// absolute positioning
	float x, dx;
	float y, dy;
	float z, dz;

	TVector3 direction;

	float phi, theta;

	std::vector<EcalHit*> hits;

	
};


#endif
