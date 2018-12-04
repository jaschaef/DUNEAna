#ifndef ECALHIT_H
#define ECALHIT_H

#include "TROOT.h"
#include "TGeoManager.h"
#include "TGeoNode.h"
#include "TGeoMatrix.h"
#include "TGeoVolume.h"
#include "TGeoNavigator.h"
#include "TGeoShape.h"
#include "TGeoBBox.h"

#include "iostream"
#include "sstream"


class EcalHit
{
public:

	/** Constructors **/
	// use position of hit in the detector
	EcalHit(float _E = 0., double _x = 0., double _y = 0., double _z = 0.);
	// use node and navigator path to its mother volume
	EcalHit(float _E, TGeoNode* _node, TString _navigatorPath);
	// set all values by hand (mainly for debugging)
	EcalHit(float _E, 
					float _x, float _y, float _z, 
					float _dx, float _dy, float _dz,
					float _phi = 0., float _theta = 0., float _psi = 0.
					);
	// EcalHit(float _E, TGeoNode* _node, TGeoNavigator* _navigator);
	// EcalHit(float _E, TGeoNode* _node);

	// Copy constructor
	EcalHit(const EcalHit& ecalHit);


	~EcalHit();

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

	static std::vector<EcalHit*> AddCells(std::vector<EcalHit*> hits);
	

public:
	void Add(EcalHit* hit);

	void Print();

	float GetE(){return E;}
	float GetX(){return x;}
	float GetY(){return y;}
	float GetZ(){return z;}
	float GetDX(){return dx;}
	float GetDY(){return dy;}
	float GetDZ(){return dz;}
	float GetEulerPhi(){return phi;}
	float GetEulerTheta(){return theta;}
	float GetEulerPsi(){return psi;}


 private:
	void Initialize(float _E, TGeoNode* _node, TGeoNavigator* navigator);
	TString GetPositionKey();


private:
	float E;

	// absolute positioning
	float x, dx;
	float y, dy;
	float z, dz;

	// euler angles
	float phi, theta, psi;

	// TGeoNode* node;
};


#endif
