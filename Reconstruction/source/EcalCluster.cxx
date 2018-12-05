#include "EcalCluster.h"
#include "EcalFunctions.h"

// #include "Math/GSLSimAnMinimizer.h"
#include "Math/Functor.h"
#include "Minuit2/Minuit2Minimizer.h"

using namespace std;

float EcalCluster::energyThreshold = 0.;
float EcalCluster::AntiKtRadius = 0.1;
std::vector<EcalHit*> EcalCluster::staticHits = {};
float EcalCluster::static_x = 0.;
float EcalCluster::static_y = 0.;
float EcalCluster::static_z = 0.;


EcalCluster::EcalCluster(){
	E = 0.;

	x = 0.;
	y = 0.;
	z = 0.;

	// dx = 0.;
	// dy = 0.;
	// dz = 0.;

	direction = TVector3();

	phi = 0.;
	theta = 0.;

	hits.clear();
}

EcalCluster::EcalCluster(EcalHit* hit) : EcalCluster(){
	AddHit(hit);
}


EcalCluster::EcalCluster(const EcalCluster& cluster){
	E = cluster.E;

	x = cluster.x;
	y = cluster.y;
	z = cluster.z;

	// dx = cluster.dx;
    // dy = cluster.dy;
	// dz = cluster.dz;

	direction = cluster.direction;

	phi = cluster.phi;
	theta = cluster.theta;

	hits = cluster.hits;
}


EcalCluster::~EcalCluster(){

}


void EcalCluster::AddHit(EcalHit* hit){
	hits.push_back(hit);
}

void EcalCluster::AddCluster(EcalCluster* cluster){
	// speed could be improved by not doing recalculation for all hits

	for(const auto& hit : cluster->hits)
		AddHit(hit);

	ComputeEtaPhiE();
}



void EcalCluster::ComputeEtaPhiE()
{
	E = 0.;

	// Calculate total energy and center of gravity
	float center_x = 0.;
	float center_y = 0.;
	float center_z = 0.;
	float sow = 0.;

	for(const auto& hit : hits){
		float _E = hit->GetE();
		E += _E;

		sow += _E;
		// center_x += hit->GetOriginalX() * _E;
		// center_y += hit->GetOriginalY() * _E;
		// center_z += hit->GetOriginalZ() * _E;
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
	// dx = 0.;
	// dy = 0.;
	// dz = 0.;


	// Now calculate direction
	EcalCluster::staticHits = hits;
	EcalCluster::static_x = x;
	EcalCluster::static_y = y;
	EcalCluster::static_z = z;

	TVector3 guessDirection = TVector3(x,y,z);
	guessDirection *= 1. / guessDirection.Mag();

	phi = guessDirection.Phi();
	theta = guessDirection.Theta();

	ROOT::Minuit2::Minuit2Minimizer min( ROOT::Minuit2::kMigrad );
	//ROOT::Math::GSLSimAnMinimizer min;

	min.SetMaxFunctionCalls(1000000);
	min.SetMaxIterations(100000);
	min.SetTolerance(0.001);

	ROOT::Math::Functor f(&EcalCluster::GetWeight,3);
	double step[3] = {0.001,0.001,0.001};
	double variable[3] = { guessDirection.X(), guessDirection.Y(), guessDirection.Z()};

	min.SetFunction(f);

	// Set the free variables to be minimized!
	min.SetVariable(0,"x",variable[0], step[0]);
	min.SetVariable(1,"y",variable[1], step[1]);
	min.SetVariable(2,"z",variable[2], step[2]);

	min.Minimize();

	const double *xs = min.X();
	cout << "Minimum: f(" << xs[0] << "," << xs[1] << "," << xs[2] << "): " << EcalCluster::GetWeight(xs) << endl;

	direction = TVector3(xs[0], xs[1], xs[2]);
	direction *= E / direction.Mag();



	// phi = guessDirection.Phi();
	// theta = guessDirection.Theta();



	// TVector3 pos = TVector3(x,y,z);
    //
	// phi = pos.Phi();
	// theta = pos.Theta();
	// direction.SetMagThetaPhi(E, theta, phi);

	// Print();
}



double EcalCluster::GetWeight(const double *xx){
	double L = 0.;

	TVector3 d = TVector3(xx[0], xx[1], xx[2]);
	TVector3 s = TVector3(static_x, static_y, static_z);
	float d_mag2 = d.Mag2();

	for(const auto& hit : EcalCluster::staticHits){
		float E_hit = hit->GetE();

		TVector3 p = TVector3(hit->GetX(), hit->GetY(), hit->GetZ());
		TVector3 dist = d.Cross(s - p);

		L += E_hit*E_hit / d_mag2 * d.Mag2();
	}

	return L;
}



void EcalCluster::Print()
{
	// cout << "EcalCluster: E = " << E << ", x = " << x << " +- " << dx << ", y = " << y << " +- " << dy << ", z = " << z << " +- " << dz << endl;
	cout << "EcalCluster: E = " << E << ", x = " << x << ", y = " << y << ", z = " << z << endl;
	cout << "             phi = " << phi << ", theta = " << theta << endl;
	cout << "             direction: phi = " << direction.Phi() << ", theta = " << direction.Theta() << endl;
	cout << "             dist to origin = " << GetDistToOrigin() << endl;
}

vector<EcalCluster*> EcalCluster::GetClusters(std::vector<EcalHit*> hits){
	//TODO: Everything

	sort(hits.begin(), hits.end(), EcalFunctions::Energy_sort());

	vector<EcalCluster*> clusters; // = GetAntiKtClusters(hits);

	EcalCluster* cluster = new EcalCluster();
	for(const auto& hit : hits)
		cluster->AddHit(hit);

	cluster->ComputeEtaPhiE();

	clusters.push_back(cluster);

	return clusters;
}


float EcalCluster::GetDistToOrigin(){
	TVector3 center = TVector3(x,y,z);
	TVector3 dist = direction.Cross(center);
	dist *= 1./direction.Mag();

	return dist.Mag();
}


vector<EcalCluster*> EcalCluster::GetAntiKtClusters(std::vector<EcalHit*> hits, double p){
	vector<EcalCluster*> clusters;

	for(const auto& hit : hits){
		EcalCluster* ec = new EcalCluster(hit);
		ec->ComputeEtaPhiE();
		clusters.push_back(ec);
	}

	sort(clusters.begin(), clusters.end(), EcalFunctions::Energy_sort());

	vector<EcalCluster*> mergedClusters;

	unsigned int nClusters = clusters.size();
	while(nClusters > 0){
		if(nClusters == 1){
			mergedClusters.push_back(clusters[0]);
			break;
		}

		float d_crit = TMath::Power(clusters[0]->GetE(), p);
		float minD = d_crit;
		unsigned int minIndex = 0;

		for(unsigned int i=1;i<clusters.size();i++){
			float d = clusters[0]->GetAntiKtDistance(clusters[i], p);
			if(d < minD){
				minD = d;
				minIndex = i;
			}
		}

		if(minIndex > 0){
			clusters[0]->AddCluster(clusters[minIndex]);
			delete clusters[minIndex];
			clusters.erase(clusters.begin() + minIndex);
		}else{
			mergedClusters.push_back(clusters[0]);
			clusters.erase(clusters.begin() + 0);
		}

		nClusters = clusters.size();
	}


	return mergedClusters;
}


float EcalCluster::GetAntiKtDistance(EcalCluster* cluster, double p){
	double k = min(TMath::Power(this->E, p), TMath::Power(cluster->E, p));
	float d = this->direction.DeltaR(cluster->direction) / EcalCluster::AntiKtRadius;

	return k * d;
}









