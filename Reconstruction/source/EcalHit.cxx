#include "EcalHit.h"
#include "EcalFunctions.h"

using namespace std;


bool EcalHit::useOriginalPositions = false;
float EcalHit::energyThreshold = 0.;
bool EcalHit::transformInput = false;


EcalHit::EcalHit(float _E, double _x, double _y, double _z){
	TGeoNode* _node = gGeoManager->FindNode(_x, _y, _z);
	if(!_node){
		cout << endl << "Can't find node at ("<<_x<<","<<_y<<","<<_z<< ")!" << endl;
		return;
	}

	// cout << "Found Node: " << _node->GetName() << endl;

	// TString nodePath = gGeoManager->GetPath();
    //
	// TGeoNavigator* navigator = gGeoManager->GetCurrentNavigator();
	// if(!navigator){
	// 	cout << endl << "Can't find navigator!" << endl;
	// 	return;
	// }

	x_orig = _x;
	y_orig = _y;
	z_orig = _z;

	// vector<TString> splits = EcalFunctions::Split(nodePath, "/");
	// TString naviPath = "";
    //
	// for(unsigned int i=0;i<splits.size();i++){
	// 	naviPath += splits[i] + "/";
	// }

	Initialize(_E, _node);

	// navigator->cd(naviPath);
	// Initialize(_E, _node, navigator);
}


EcalHit::EcalHit(float _E, TGeoNode* _node, TString _navigatorPath){
	TGeoNavigator* navigator = gGeoManager->GetCurrentNavigator();
	if(!navigator){
		cout << endl << "Can't find navigator!" << endl;
		return;
	}

	navigator->cd(_navigatorPath);
	navigator->CdDown(_node);

	Initialize(_E, _node);

	// we don't have the actual points, so we use this to have something non-zero
	x_orig = x;
	y_orig = y;
	z_orig = z;
}


// EcalHit::EcalHit(double _E, TGeoNode* _node, TGeoNavigator* _navigator){

// 	// TGeoVolume* motherVol = _node->GetMotherVolume();
// 	// _node->SetMotherVolume(_navigator->GetCurrentVolume());

// 	Initialize(_E, _node, _navigator);

// 	// _node->SetMotherVolume(motherVol);
// }


// EcalHit::EcalHit(double _E, TGeoNode* _node){
// 	cout << "EcalHit: from two nodes" << endl;

// 	TGeoNavigator* navigator = gGeoManager->GetCurrentNavigator();
// 	if(!navigator){
// 		cout << endl << "Can't find navigator!" << endl;
// 		return;
// 	}

// 	// TGeoVolume* motherVol =  _node->GetMotherVolume();
// 	// Int_t id = motherVol->GetIndex(_node);

// 	navigator->cd("Default/volWorld_1/volDetEnclosure_0/volNDHPgTPC_0");
// 	// TString naviPath = GetNavigatorPath(_node, _masterNode);
// 	// navigator->CdUp();

// 	cout << "curr nav path = " << navigator->GetPath() << endl;
// 	cout << "curr nav vol = " << navigator->GetCurrentVolume()->GetName() << endl;


// 	// cout << "Found naviPath: '" << naviPath << "'" << endl;
// 	// navigator->cd(naviPath);

// 	Initialize(_E, _node, navigator);
// 	node = _node;
// }


EcalHit::EcalHit(const EcalHit& ecalHit){
	E = ecalHit.E;

	point = TVector3(ecalHit.point);

	x = ecalHit.x;
	y = ecalHit.y;
	z = ecalHit.z;

	dx = ecalHit.dx;
	dy = ecalHit.dy;
	dz = ecalHit.dz;

	x_orig = ecalHit.x_orig;
	y_orig = ecalHit.y_orig;
	z_orig = ecalHit.z_orig;

	phi = ecalHit.phi;
	theta = ecalHit.theta;
	psi = ecalHit.psi;

}


EcalHit::EcalHit(float _E,
					float _x, float _y, float _z,
					float _dx, float _dy, float _dz,
					float _phi, float _theta, float _psi
				){
	E = _E;

	x = _x;
	y = _y;
	z = _z;

	point = TVector3(x,y,z);

	dx = _dx;
	dy = _dy;
	dz = _dz;

	x_orig = x;
	y_orig = y;
	z_orig = z;

	phi = _phi;
	theta = _theta;
	psi = _psi;
}


EcalHit::~EcalHit(){
	// if (node != 0)
	// 	delete node;
}


// Navigator needs to be set to the current node before calling this!
void EcalHit::Initialize(float _E, TGeoNode* _node){ //, TGeoNavigator* navigator
	E = _E;

	// if(navigator != 0)
	// 	navigator->CdDown(_node); //navigator->GetCurrentVolume()->FindMatrixOfDaughterVolume(_node->GetVolume());

	// if(!foundMatrix){
	// 	cout << "Can not find matrix for node '" << _node->GetName() << "'! " << endl;
	// 	return;
	// }


	// cout << "coordinatesLocal  = " << coordinatesLocal[0] << ", " << coordinatesLocal[1] << ", " << coordinatesLocal[2] << endl;


	if(!EcalHit::useOriginalPositions){
		// cout << "Current Node: " << gGeoManager->GetCurrentNode()->GetName() << endl;
		const Double_t* coordinatesGlobal = gGeoManager->GetCurrentMatrix()->GetTranslation();

		// Set the absolute position in global coordinates
		// const Double_t* coordinatesLocal = _node->GetMatrix()->GetTranslation();
		// Double_t coordinatesGlobal[3] = {0.,0.,0.};
		// navigator->LocalToMaster(coordinatesLocal, coordinatesGlobal);

		// cout << "coordinatesLocal  = " << coordinatesLocal[0] << ", " << coordinatesLocal[1] << ", " << coordinatesLocal[2] << endl;
		// cout << "coordinatesMaster = " << coordinatesMaster[0] << ", " << coordinatesMaster[1] << ", " << coordinatesMaster[2] << endl;

		x = EcalFunctions::Round(coordinatesGlobal[0], 3);
		y = EcalFunctions::Round(coordinatesGlobal[1], 3);
		z = EcalFunctions::Round(coordinatesGlobal[2], 3);
	}else{
		x = x_orig;
		y = y_orig;
		z = z_orig;
	}

	if(EcalHit::transformInput){
		float h = y;
		y = z;
		z = x;
		x = h;

		h = y_orig;
		y_orig = z_orig;
		z_orig = x_orig;
		x_orig = h;
	}

	point = TVector3(x,y,z);

	// Set the size of the box
	// TODO: What if it's not a box?
	TGeoBBox* box = (TGeoBBox*)_node->GetVolume()->GetShape();
	dx = EcalFunctions::Round(box->GetDX(), 3);
	dy = EcalFunctions::Round(box->GetDY(), 3);
	dz = EcalFunctions::Round(box->GetDZ(), 3);



	// TString currentNavPath = navigator->GetPath();
    //
	// cout << "CurrPath = " << currentNavPath << endl;
    //
	// vector<TString> splitPath = EcalFunctions::Split(currentNavPath, "/");
    //
	// vector<TGeoNode*> currentNavNodes;
	// TString myPath = "";
    //
	// // ignore highest level node, aka 'world'
	// for(unsigned int i=1;i<splitPath.size();i++){
	// 	if(myPath != TString(""))
	// 		myPath += "/";
	// 	myPath += splitPath[i];
    //
	// 	// cout << "CurrNodePath: " << myPath << endl;
    //
	// 	TGeoNode* thisNode = EcalFunctions::GetNode(myPath);
    //
	// 	// cout << "  => " << thisNode->GetName() << endl;
    //
	// 	currentNavNodes.push_back(thisNode);
	// }
	// currentNavNodes.push_back(_node);
    //
	// Double_t rotationGlobal[9] = {0.,0.,0.,0.,0.,0.,0.,0.,0.};
    //
	// for(unsigned int i=0;i<currentNavNodes.size()-1;i++){
	// 	bool success = currentNavNodes[i]->GetVolume()->FindMatrixOfDaughterVolume(currentNavNodes[i+1]->GetVolume());
    //
	// 	if(!success){
	// 		cout << "Can't find daughter matrix '" << currentNavNodes[i+1]->GetName() << "' of '" << currentNavNodes[i]->GetName() << "'!"<< endl;
	// 		continue;
	// 	}
    //
	// 	TGeoHMatrix* matr = gGeoManager->GetCurrentMatrix();
	// 	const Double_t* rotationLocal =  matr->GetRotationMatrix();
    //
	// 	navigator->LocalToMaster(rotationLocal, rotationGlobal);
    //
	// 	for(int i=0;i<9;i++)
	// 		cout << "local mat[" << (i % 3) << "," << (i / 3) << "] = " << rotationLocal[i] << endl;
    //
	// 	for(int i=0;i<9;i++)
	// 		cout << "global mat[" << (i % 3) << "," << (i / 3) << "] = " << rotationGlobal[i] << endl;
	// }


	// const Double_t* rotationLocal = _node->GetMatrix()->GetRotationMatrix();
	// Double_t rotationGlobal[9] = {0.,0.,0.,0.,0.,0.,0.,0.,0.};
	// navigator->LocalToMaster(rotationLocal, rotationGlobal);

	// for(int i=0;i<9;i++)
	// 	cout << "mat[" << (i % 3) << "," << (i / 3) << "] = " << rotationGlobal[i] << endl;

	TGeoRotation* rot = new TGeoRotation(*gGeoManager->GetCurrentMatrix());
	const Double_t* rotationGlobal = rot->GetRotationMatrix();

	myX = TVector3(EcalFunctions::Round(rotationGlobal[0], 3), EcalFunctions::Round(rotationGlobal[3], 3), EcalFunctions::Round(rotationGlobal[6], 3));
	myY = TVector3(EcalFunctions::Round(rotationGlobal[1], 3), EcalFunctions::Round(rotationGlobal[4], 3), EcalFunctions::Round(rotationGlobal[7], 3));
	myZ = TVector3(EcalFunctions::Round(rotationGlobal[2], 3), EcalFunctions::Round(rotationGlobal[5], 3), EcalFunctions::Round(rotationGlobal[8], 3));


	// I don't think this is needed, but just to be sure
	myX *= 1./myX.Mag();
	myY *= 1./myY.Mag();
	myZ *= 1./myZ.Mag();


	// Set Euler angles
	double _phi = 0.;
	double _theta = 0.;
	double _psi = 0.;
	rot->GetAngles(_phi, _theta, _psi);

	phi = EcalFunctions::Round(_phi, 3);
	theta = EcalFunctions::Round(_theta, 3);
	psi = EcalFunctions::Round(_psi, 3);
}


// TString EcalHit::GetNavigatorPath(TGeoNode* _node, TGeoNode* _masterNode, TString _result){
// 	TString result = _result;

// 	if(_node == _masterNode)
// 		return result;

// 	TGeoVolume* motherVol = _node->GetMotherVolume();

// 	if(!motherVol){
// 		cout << "Can not retrieve mother of node '" << _node->GetName() << endl;
// 		return "DummyResultSoTheCodeWillCrash";
// 	}

// 	cout << "Found mother: " << motherVol->GetName() << endl;

// 	result = TString(motherVol->GetName()) + "/" + result;
// 	TGeoNode* mother = motherVol->GetNode(TString(motherVol->GetName()));

// 	if(mother)
// 		return EcalHit::GetNavigatorPath(mother, _masterNode, result);
// 	else{
// 		cout << "Broken mother node" << endl;
// 		return result;
// 	}

// }



void EcalHit::Print(){
	cout << "EcalHit: E = " << E << ", x = " << x << " +- " << dx << ", y = " << y << " +- " << dy << ", z = " << z << " +- " << dz << endl;
	cout << "         eulerPhi = " << phi << ", eulerTheta = " << theta << ", eulerPsi = " << psi << endl;
	cout << "         phi = " << point.Phi() << ", theta = " << point.Theta() << endl;
	cout << "         pos = " << GetPositionKey() << endl;
}



vector<EcalHit*> EcalHit::AddCells(vector<EcalHit*> hits){

	map<TString, unsigned int> cellsMap;
	vector<EcalHit*> cells;

	for(const auto& hit : hits){
		if(hit->GetE() < EcalHit::energyThreshold)
			continue;

		TString key = hit->GetPositionKey();

		// no cell at this position
		if(cellsMap.find(key) == cellsMap.end()){
			EcalHit* newHit = new EcalHit(*hit);
			cells.push_back(newHit);
			cellsMap[key] = cells.size()-1;
		}else{
			// found cell at this position
			cells[cellsMap[key]]->Add(hit);
		}
	}

	return cells;
}


void EcalHit::Add(EcalHit* hit){
	E += hit->E;
}


TString EcalHit::GetPositionKey(){
	float useX = x;
	float useY = y;
	float useZ = z;

	TString key = EcalFunctions::ToString(useX) + "_" + EcalFunctions::ToString(useY) + "_" + EcalFunctions::ToString(useZ);

	return key;
}

