/*
 *  ofxGeometry.h
 *
 *  Created by Rick Companje on 23-03-11.
 *  Copyright 2010 Globe4D. All rights reserved.
 *
 */

#ifndef	OFX_GEOMETRY
#define OFX_GEOMETRY

#include "ofMain.h"
//#include "ofxVectorMath.h"

class ofxCartesian;
class ofxQuaternionExtra;

class ofxLatLon {
public:
	ofxLatLon() {};
	ofxLatLon(ofxQuaternionExtra q);
	ofxLatLon(float lat, float lon);

	ofxLatLon& operator=(ofxQuaternionExtra q);

	friend ostream& operator <<(ostream &os,const ofxLatLon ll);

	float lon,lat;
};

class ofxAxisAngle {
public:
	ofxAxisAngle() : angle(0) {};
	ofxAxisAngle(ofQuaternion q) { *this = q; };
	ofxAxisAngle(ofVec3f axis, float angle) : axis(axis), angle(angle) {};
	ofxAxisAngle(float x, float y, float z, float angle) : angle(angle) { axis.set(x,y,z); };
	ofxAxisAngle(ofxCartesian c);

	ofxAxisAngle& operator= (ofxQuaternionExtra q);

	ofVec3f  axis;
	float angle;
};

class ofxCartesian : public ofVec3f  {
public:
	ofxCartesian() {};
	ofxCartesian(ofxLatLon ll) { *this = ll; };
	ofxCartesian(float x, float y, float z) { set(x,y,z); };
	ofxCartesian(ofQuaternion q) { *this = q; };

	ofxCartesian& operator= (ofxLatLon ll);
	ofxCartesian& operator= (ofQuaternion q);

	friend ostream& operator <<(ostream &os,const ofxCartesian c);
};

class ofxQuaternionExtra : public ofQuaternion {
public:

	ofxQuaternionExtra() {};
	ofxQuaternionExtra(ofxLatLon ll) { *this = ll; };
	//ofxQuaternionExtra(ofxLatLon ll, float degrees);
	ofxQuaternionExtra(ofQuaternion q) { *this = q; };
	ofxQuaternionExtra(ofxCartesian c) { *this = c; };
	ofxQuaternionExtra(ofMatrix4x4 m) { *this = m; };

	ofxQuaternionExtra& operator=(ofxLatLon ll);
	ofxQuaternionExtra& operator=(ofQuaternion q);
	ofxQuaternionExtra& operator=(ofxCartesian c);
	ofxQuaternionExtra& operator=(ofMatrix4x4 m) { set(m); };

	void rotate(ofxAxisAngle aa);
	void rotateX(float angle);
	void rotateY(float angle);
	void rotateZ(float angle);

	ofVec3f  getAxis();
	float getAngle();
	float getDistance(ofxLatLon ll);

};


#endif
