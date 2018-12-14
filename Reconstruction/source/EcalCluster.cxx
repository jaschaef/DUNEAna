#include "EcalCluster.h"
#include "EcalFunctions.h"

// #include "Math/GSLSimAnMinimizer.h"
#include "Math/Functor.h"
#include "Minuit2/Minuit2Minimizer.h"

using namespace std;

float EcalCluster::energyThreshold = 0.;
float EcalCluster::clusterRadius = 0.1;
std::vector<EcalHit*> EcalCluster::staticHits = {};
float EcalCluster::static_x = 0.;
float EcalCluster::static_y = 0.;
float EcalCluster::static_z = 0.;


EcalCluster::EcalCluster(){
	E = 0.;

	point = TVector3();
	direction = TVector3();

	isMerged = false;

	hits.clear();
}

EcalCluster::EcalCluster(EcalHit* hit) : EcalCluster(){
	AddHit(hit);
}


EcalCluster::EcalCluster(const EcalCluster& cluster){
	E = cluster.E;

	point = cluster.point;
	direction = cluster.direction;

	isMerged = cluster.isMerged;

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

	ComputeDirection();
}



void EcalCluster::ComputeDirection()
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
		center_x += hit->GetX() * _E;
		center_y += hit->GetY() * _E;
		center_z += hit->GetZ() * _E;
	}

	center_x /= sow;
	center_y /= sow;
	center_z /= sow;

	point = TVector3(center_x, center_y, center_z);

	if(hits.size() > 2){
		direction = GetSimpleDirection();
		// direction = GetFittedDirection();
	}else if(hits.size() == 2){
		direction = hits[0]->GetPoint() - hits[1]->GetPoint();
		direction *= 1./direction.Mag();
	}else if(hits.size() == 1){
		direction = TVector3(point);
		direction *= 1./direction.Mag();
	}else
		direction = TVector3();


	// Print();
}


TVector3 EcalCluster::GetSimpleDirection(){
	// Now calculate direction

	float bestPhi = -1.;
	float bestTheta = -1.;
	float minWeight = 0.;

	for(float phi = 0.;phi<2.*TMath::Pi();phi += 0.1){
		for(float theta = 0.;theta<TMath::Pi();theta += 0.1){
			TVector3 dir;
			dir.SetMagThetaPhi(1., theta, phi);

			float w = 0.;

			for(const auto& hit :hits){
				float E_hit = hit->GetE();
				TVector3 dist = dir.Cross(point - hit->GetPoint());
				w += E_hit*E_hit * dist.Mag2();
			}

			if(w < minWeight || bestPhi < 0.){
				bestPhi = phi;
				bestTheta = theta;
				minWeight = w;
			}
		}
	}


	TVector3 result;
	result.SetMagThetaPhi(1., bestTheta, bestPhi);

	return result;
}

TVector3 EcalCluster::GetFittedDirection(){
	// Now calculate direction
	EcalCluster::staticHits = hits;
	EcalCluster::static_x = point.X();
	EcalCluster::static_y = point.Y();
	EcalCluster::static_z = point.Z();

	ROOT::Minuit2::Minuit2Minimizer min( ROOT::Minuit2::kMigrad );

	min.SetMaxFunctionCalls(1000000);
	min.SetMaxIterations(100000);
	min.SetTolerance(0.001);

	ROOT::Math::Functor f(&EcalCluster::GetWeight,2);
	min.SetFunction(f);

	double stepSize = 0.01;

	// Set the free variables to be minimized!
	min.SetLimitedVariable(0, "phi", 0, stepSize, 0., 2.*TMath::Pi());
	min.SetLimitedVariable(1, "theta", 0, stepSize, 0., TMath::Pi());

	bool success = min.Minimize();

	TVector3 result;

	if(success){
		const double *xs = min.X();
		result.SetMagThetaPhi(1., xs[1], xs[0]);
	}else{
		cout << "Problem fitting direction of cluster!" << endl;
	}

	return result;
}


double EcalCluster::GetWeight(const double *xx){
	double L = 0.;

	// calculate and add up orthogonal distance to line for each hit
	// this value will be minimized to find best fit

	float phi = xx[0];
	float theta = xx[1];

	TVector3 dir;
	dir.SetMagThetaPhi(1., theta, phi);

	TVector3 s = TVector3(static_x, static_y, static_z);

	for(const auto& hit : EcalCluster::staticHits){
		float E_hit = hit->GetE();
		TVector3 dist = dir.Cross(s - hit->GetPoint());
		L += E_hit*E_hit * dist.Mag2();
	}

	return L;
}


// TVector3 EcalCluster::GetFittedDirection(){
// 	// Now calculate direction
// 	EcalCluster::staticHits = hits;
// 	EcalCluster::static_x = point.X();
// 	EcalCluster::static_y = point.Y();
// 	EcalCluster::static_z = point.Z();
//
// 	ROOT::Minuit2::Minuit2Minimizer min( ROOT::Minuit2::kMigrad );
//
// 	TVector3 guessDirection = TVector3(point);
// 	guessDirection *= 1. / guessDirection.Mag();
//
// 	min.SetMaxFunctionCalls(1000000);
// 	min.SetMaxIterations(100000);
// 	min.SetTolerance(0.001);
//
// 	ROOT::Math::Functor f(&EcalCluster::GetWeight,3);
// 	double step[3] = {0.01,0.01,0.01};
// 	double variable[3] = { guessDirection.X(), guessDirection.Y(), guessDirection.Z()};
//
// 	min.SetFunction(f);
//
// 	// Set the free variables to be minimized!
// 	min.SetVariable(0,"x",variable[0], step[0]);
// 	min.SetVariable(1,"y",variable[1], step[1]);
// 	min.SetVariable(2,"z",variable[2], step[2]);
//
// 	min.Minimize();
//
// 	const double *xs = min.X();
//
// 	TVector3 result = TVector3(xs[0], xs[1], xs[2]);
// 	result *= 1. / result.Mag();
//
// 	return result;
// }
//
//
// double EcalCluster::GetWeight(const double *xx){
// 	double L = 0.;
//
// 	// calculate and add up orthogonal distance to line for each hit
// 	// this value will be minimized to find best fit
//
// 	TVector3 d = TVector3(xx[0], xx[1], xx[2]);
// 	TVector3 s = TVector3(static_x, static_y, static_z);
// 	float d_mag2 = d.Mag2();
//
// 	for(const auto& hit : EcalCluster::staticHits){
// 		float E_hit = hit->GetE();
//
// 		//TVector3 p = TVector3(hit->GetX(), hit->GetY(), hit->GetZ());
// 		TVector3 p = TVector3(hit->GetX() / hit->GetDX(), hit->GetY() / hit->GetDY(), hit->GetZ() / hit->GetDZ());
//
// 		TVector3 dist = d.Cross(s - p);
//
// 		L += E_hit*E_hit / d_mag2 * dist.Mag2();
// 	}
//
// 	return L;
// }



void EcalCluster::Print()
{
	// cout << "EcalCluster: E = " << E << ", x = " << x << " +- " << dx << ", y = " << y << " +- " << dy << ", z = " << z << " +- " << dz << endl;
	cout << "EcalCluster: E = " << E << ", x = " << GetX() << ", y = " << GetY() << ", z = " << GetZ() << endl;
	cout << "             phi = " << GetPhi() << ", theta = " << GetTheta() << endl;
	cout << "             direction: phi = " << direction.Phi() << ", theta = " << direction.Theta() << endl;
	cout << "             dist to origin = " << GetDistToOrigin() << endl;
}

// vector<EcalCluster*> EcalCluster::GetClusters(std::vector<EcalHit*> hits){
// 	//TODO: Everything
//
// 	sort(hits.begin(), hits.end(), EcalFunctions::Energy_sort());
//
// 	vector<EcalCluster*> clusters = GetSimpleClusters(hits);
// 	// vector<EcalCluster*> clusters = GetAntiKtClusters(hits);
//
// 	return clusters;
// }


float EcalCluster::GetDistToOrigin(){
	TVector3 dist = direction.Cross(point);
	return dist.Mag();
}


vector<EcalCluster*> EcalCluster::GetSimpleClusters(std::vector<EcalHit*> hits){
	vector<EcalCluster*> clusters;

	sort(hits.begin(), hits.end(), EcalFunctions::Energy_sort());

	EcalCluster* cluster = new EcalCluster();
	for(const auto& hit : hits)
		cluster->AddHit(hit);

	cluster->ComputeDirection();

	clusters.push_back(cluster);

	return clusters;
}


vector<EcalCluster*> EcalCluster::GetNeighborClustersInDirection(std::vector<EcalHit*> hits, NeighborDirection direction, bool resetNeighborhood){

	vector<EcalCluster*> clusters;

	if(resetNeighborhood){
		for(const auto& hit : hits)
			hit->ResetNeighborhood();
	}

	for(const auto& hit : hits){
		if(hit->IsInNeighborHood())
			continue;

		vector<EcalHit*> neighbors = hit->GetNeighbors(hits, direction);

		// neighbors can be empty if the strip is in the wrong direction
		if(neighbors.size() > 0){
			EcalCluster* cluster = new EcalCluster();
			for(const auto& nb : neighbors)
				cluster->AddHit(hit);

			cluster->ComputeDirection();
			clusters.push_back(cluster);
		}
	}

	return clusters;
}


vector<EcalCluster*> EcalCluster::GetNeighborClusters(std::vector<EcalHit*> hits){
	vector<EcalCluster*> clusters;


	sort(hits.begin(), hits.end(), EcalFunctions::Energy_sort());

	vector<EcalCluster*> clustersX = GetNeighborClustersInDirection(hits, NeighborDirection::X, true);
	vector<EcalCluster*> clustersY = GetNeighborClustersInDirection(hits, NeighborDirection::Y, true);
	// vector<EcalCluster*> clustersZ = GetNeighborClustersInDirection(hits, NeighborDirection::Z, true);

	cout << "Found clustersX: " << clustersX.size() << endl;
	cout << "Found clustersY: " << clustersY.size() << endl;

	unsigned int notClustered = 0;
	for(const auto& hit : hits)
		if(!hit->IsInNeighborHood())
			notClustered += 1;

	cout << notClustered << " cells have not been clustered!" << endl;

	vector<EcalCluster*> clustersXMerged = MergeClusters(clustersX);
	vector<EcalCluster*> clustersYMerged = MergeClusters(clustersY);

	for(const auto& cluster : clustersXMerged)
		clusters.push_back(cluster);
	for(const auto& cluster : clustersYMerged)
		clusters.push_back(cluster);

	return clusters;
}


vector<EcalCluster*> EcalCluster::MergeClusters(std::vector<EcalCluster*> clusters){
	vector<EcalCluster*> mergedClusters;

	// this might need some additional logic, if the strips are shorter than the full detector length

	float maxDist = EcalCluster::clusterRadius*EcalCluster::clusterRadius;

	sort(clusters.begin(), clusters.end(), EcalFunctions::Energy_sort());

	while(clusters.size() > 0){
		unsigned int minIndex = 0;
		float minR2 = maxDist;

		for(unsigned int i=1;i<clusters.size();i++){
			float r2 = (clusters[0]->point-clusters[i]->point).Mag2();

			if(r2 > minR2)
				continue;

			minR2 = r2;
			minIndex = (int) i;
		}


		if(minIndex > 0){
			clusters[0]->AddCluster(clusters[minIndex]);
			delete clusters[minIndex];
			clusters.erase(clusters.begin() + minIndex);
			sort(clusters.begin(), clusters.end(), EcalFunctions::Energy_sort());
		}else{
			mergedClusters.push_back(clusters[0]);
			clusters.erase(clusters.begin() + 0);
		}
	}


	return mergedClusters;
}




vector<EcalCluster*> EcalCluster::GetAntiKtClusters(std::vector<EcalHit*> hits, double p){
	vector<EcalCluster*> clusters;

	sort(hits.begin(), hits.end(), EcalFunctions::Energy_sort());

	for(const auto& hit : hits){
		EcalCluster* ec = new EcalCluster(hit);
		ec->ComputeDirection();
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
			sort(clusters.begin(), clusters.end(), EcalFunctions::Energy_sort());
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
	float d = this->GetPoint().DeltaR(cluster->GetPoint()) / EcalCluster::clusterRadius;

	return k * d;
}









