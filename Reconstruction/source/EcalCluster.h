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
	void ComputeDirection();
	void Print();

	float GetE(){return E;}
	float GetX(){return point.X();}
	float GetY(){return point.Y();}
	float GetZ(){return point.Z();}

	// float GetDX(){return dx;}
	// float GetDY(){return dy;}
	// float GetDZ(){return dz;}

	float GetPhi(){return point.Phi();}
	float GetTheta(){return point.Theta();}
	TVector3 GetPoint(){return point;}
	TVector3 GetDirection(){return direction;}
	float GetDistToOrigin();

	std::vector<EcalHit*> GetHits(){return hits;}

	// static std::vector<EcalCluster*> GetClusters(std::vector<EcalHit*> hits);
	static std::vector<EcalCluster*> GetSimpleClusters(std::vector<EcalHit*> hits);

	static std::vector<EcalCluster*> GetAntiKtClusters(std::vector<EcalHit*> hits, double p = -2.);

	static std::vector<EcalCluster*> GetNeighborClusters(std::vector<EcalHit*> hits);
	static std::vector<EcalCluster*> GetNeighborClustersInDirection(std::vector<EcalHit*> hits, NeighborDirection direction, bool resetNeighborhood);
	static std::vector<EcalCluster*> MergeClusters(std::vector<EcalCluster*> clusters);

	TVector3 GetFittedDirection();
	TVector3 GetSimpleDirection();

	float GetAntiKtDistance(EcalCluster* cluster, double p);


	static double GetWeight(const double *xx);


private:
	float E;

	// absolute positioning
	TVector3 point;
	TVector3 direction;

	std::vector<EcalHit*> hits;

	bool isMerged;

public:
	static float energyThreshold;
	static float clusterRadius;
	static std::vector<EcalHit*> staticHits;
	static float static_x;
	static float static_y;
	static float static_z;

};


#endif
