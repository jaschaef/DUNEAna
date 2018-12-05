#ifndef ECALHIT_H
#define ECALHIT_H

#include "TROOT.h"
#include "TString.h"
#include "TGeoManager.h"
#include "TGeoNode.h"
#include "TGeoMatrix.h"
#include "TGeoVolume.h"
#include "TGeoNavigator.h"
#include "TGeoShape.h"
#include "TGeoBBox.h"
#include "TVector3.h"

#include "iostream"


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

	float GetPhi(){return direction.Phi();}
	float GetTheta(){return direction.Theta();}
	TVector3 GetDirection(){return direction;}

	float GetOriginalX(){return x_orig;}
	float GetOriginalY(){return y_orig;}
	float GetOriginalZ(){return z_orig;}


 private:
	void Initialize(float _E, TGeoNode* _node, TGeoNavigator* navigator);
	TString GetPositionKey();


private:
	float E;

	// absolute positioning
	TVector3 direction;
	float x, dx;
	float y, dy;
	float z, dz;

	// euler angles
	float phi, theta, psi;


public:
	static bool useOriginalPositions;
	static float energyThreshold;
	static bool transformInput;

	float x_orig, y_orig, z_orig;

	// TGeoNode* node;
};


#endif
