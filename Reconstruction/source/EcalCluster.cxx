#include "EcalCluster.h"

using namespace std;

EcalCluster::EcalCluster(){


}


EcalCluster::~EcalCluster(){

}


void EcalCluster::AddHit(EcalHit* hit){
	hits.push_back(hit);
}



void EcalCluster::ComputeEtaPhiE()
{
	E = 0.;

	// TODO: Replace by fit through points
	// currently: calculate center of gravity and use it with origin to calculate eta, phi
	float center_x = 0.;
	float center_y = 0.;
	float center_z = 0.;
	float sow = 0.;

	for(const auto& hit : hits){
		float _E = hit->GetE();
		E += _E;

		sow += _E;
		center_x += hit->GetX() * _E;
		center_y += hit->GetY() * _E;
		center_z += hit->GetZ() * _E;
	}

	center_x /= sow;
	center_y /= sow;
	center_z /= sow;

	x = center_x;
	y = center_y;
	z = center_z;
	dx = 0.;
	dy = 0.;
	dz = 0.;

	TVector3 pos = TVector3(x,y,z);
	
	phi = pos.Phi();
	theta = pos.Theta();
	direction.SetMagThetaPhi(E, phi, theta);

	Print();
}


void EcalCluster::Print()
{
	cout << "EcalCluster: E = " << E << ", x = " << x << " +- " << dx << ", y = " << y << " +- " << dy << ", z = " << z << " +- " << dz << endl;
	cout << "         phi = " << phi << ", theta = " << theta << endl;
}

vector<EcalCluster*> EcalCluster::GetClusters(std::vector<EcalHit*> hits){
	//TODO: Everything
	vector<EcalCluster*> clusters;

	EcalCluster* cluster = new EcalCluster();
	for(const auto& hit : hits)
		cluster->AddHit(hit);

	clusters.push_back(cluster);

	return clusters;
}

