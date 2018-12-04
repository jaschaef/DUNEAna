#include "EcalHit.h"
#include "TObjArray.h"
#include "TObjString.h"


using namespace std;

EcalHit::EcalHit(float _E, double _x, double _y, double _z){
	TGeoNode* _node = gGeoManager->FindNode(_x, _y, _z);
	if(!_node){
		cout << endl << "Can't find node at ("<<_x<<","<<_y<<","<<_z<< ")!" << endl;
		return;
	}

	TString nodePath = gGeoManager->GetPath();
	// cout << "Found node: " << nodePath << endl;
	
	TGeoNavigator* navigator = gGeoManager->GetCurrentNavigator();
	if(!navigator){
		cout << endl << "Can't find navigator!" << endl;
		return;
	}

	vector<TString> splits = Split(nodePath, "/");
	TString naviPath = "";

	for(unsigned int i=0;i<splits.size();i++){
		naviPath += splits[i] + "/";
	}

	navigator->cd(naviPath);
	gGeoManager->CdUp();

	Initialize(_E, _node, navigator);

	// Print();
}


EcalHit::EcalHit(float _E, TGeoNode* _node, TString _navigatorPath){
	TGeoNavigator* navigator = gGeoManager->GetCurrentNavigator();
	if(!navigator){
		cout << endl << "Can't find navigator!" << endl;
		return;
	}

	navigator->cd(_navigatorPath);
	
	Initialize(_E, _node, navigator);
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

	x = ecalHit.x;
	y = ecalHit.y;
	z = ecalHit.z;

	dx = ecalHit.dx;
	dy = ecalHit.dy;
	dz = ecalHit.dz;

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

	dx = _dx;
	dy = _dy;
	dz = _dz;

	phi = _phi;
	theta = _theta;
	psi = _psi;
}


EcalHit::~EcalHit(){
	// if (node != 0)
	// 	delete node;
}



void EcalHit::Initialize(float _E, TGeoNode* _node, TGeoNavigator* navigator){
	E = _E;
	// node = _node;

	// Set the absolute position in global coordinates
	const Double_t* coordinatesLocal = _node->GetMatrix()->GetTranslation();
	Double_t coordinatesMaster[3] = {0.,0.,0.};
	navigator->LocalToMaster(coordinatesLocal, coordinatesMaster);

	// cout << "coordinatesLocal  = " << coordinatesLocal[0] << ", " << coordinatesLocal[1] << ", " << coordinatesLocal[2] << endl;
	// cout << "coordinatesMaster = " << coordinatesMaster[0] << ", " << coordinatesMaster[1] << ", " << coordinatesMaster[2] << endl;

	x = float(round(coordinatesMaster[0]*1000.)/1000.);
	y = float(round(coordinatesMaster[1]*1000.)/1000.);
	z = float(round(coordinatesMaster[2]*1000.)/1000.);
	
	// Set the size of the box
	// TODO: What if it's not a box?
	TGeoBBox* box = (TGeoBBox*)_node->GetVolume()->GetShape();
	dx = float(round(box->GetDX()*1000.)/1000.);
	dy = float(round(box->GetDY()*1000.)/1000.);
	dz = float(round(box->GetDZ()*1000.)/1000.);

	// Set Euler angles
	double _phi = 0.;
	double _theta = 0.;
	double _psi = 0.;

	TGeoRotation* rot = new TGeoRotation(*_node->GetMatrix());
	rot->GetAngles(_phi, _theta, _psi);

	phi = float(round(_phi)/1000.);
	theta = float(round(_theta)/1000.);
	psi = float(round(_psi)/1000.);
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


TGeoNode* EcalHit::GetNode(TString absoluteNodeName){

	TGeoManager *geom = gGeoManager;
	if(!geom){
		cout << endl << "Problem getting GeoManager!" << endl;
		return 0;
	}


	vector<TString> nodeNames = Split(absoluteNodeName, "/");

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



vector<TString> EcalHit::Split(TString input, TString delim){
	TObjArray *tx = input.Tokenize(delim);

	vector<TString> splits;

	TString filename = ((TObjString *)(tx->At(tx->GetEntries()-1)))->String();

	for (Int_t i = 0; i < tx->GetEntries(); i++)
		splits.push_back(((TObjString *)(tx->At(i)))->String());

	return splits;
}



void EcalHit::Print(){
	cout << "EcalHit: E = " << E << ", x = " << x << " +- " << dx << ", y = " << y << " +- " << dy << ", z = " << z << " +- " << dz << endl;
	cout << "         phi = " << phi << ", theta = " << theta << ", psi = " << psi << endl;
	cout << "         pos = " << GetPositionKey() << endl;
}



vector<EcalHit*> EcalHit::AddCells(vector<EcalHit*> hits){

	map<TString, unsigned int> cellsMap;
	vector<EcalHit*> cells;
	
	for(const auto& hit : hits){
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
	float useX = x; // round(x*1000.) / 1000.;
	float useY = y; // round(y*1000.) / 1000.;
	float useZ = z; // round(z*1000.) / 1000.;

	// hack to avoid "-0" in the string
	if(useX == 0.)
		useX = 0.;
	if(useY == 0.)
		useY = 0.;
	if(useZ == 0.)
		useZ = 0.;

	TString key = ToString(useX) + "_" + ToString(useY) + "_" + ToString(useZ);

	return key;
}