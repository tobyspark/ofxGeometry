/*
 *  ofxGeometry.cpp
 *
 *  Created by Rick Companje on 23-03-11.
 *  Feel free to use and improve this code
 *
 */

#include "ofxGeometry.h"

ofxCartesian& ofxCartesian::operator=(ofxLatLon ll) {
	//from ofxLatLon to ofxCartesian
	float phi = ofDegToRad(ll.lat + 90);
	float theta = ofDegToRad(ll.lon - 90);
	this->x =  sin(phi) * cos(theta);
	this->y =  sin(phi) * sin(theta);
	this->z = -cos(phi);
	if (ll.lat>=0) x=-x;		
	return *this;
}

ofxQuaternionExtra& ofxQuaternionExtra::operator=(ofxLatLon ll) {	
	//from ofxLatLon to ofxQuaternionExtra
	return *this = ofxCartesian(ll);
}

ofxQuaternionExtra& ofxQuaternionExtra::operator=(ofxCartesian cp) {
	//from ofxCartesian to ofxQuaternionExtra
	*this = ofMatrix4x4(0,0,0,0, 0,0,0,0, -cp.x,-cp.y,-cp.z,0, 0,0,0,1);
	return *this;
}

ofxLatLon& ofxLatLon::operator=(ofxQuaternionExtra q) {
	//from ofxQuaternionExtra to ofxLatLon
	ofxCartesian c(q);
	lat = ofRadToDeg(asin(c.z));
	lon = ofRadToDeg(atan2(c.y,c.x))-90;
	if (lon<-180) lon+=360;
	return *this;
};

ofxCartesian& ofxCartesian::operator=(ofQuaternion q) {
	//from ofxQuaternionExtra to ofxCartesian
	ofVec4f v(0,0,-1,0);
	ofMatrix4x4 m;
	q.get(m);
	ofVec4f mv = m*v;
	set(mv.x,mv.y,mv.z);
	return *this;
}

//from quaternion to axis angle
ofxAxisAngle& ofxAxisAngle::operator=(ofxQuaternionExtra q) {
	q.getRotate(this->angle, this->axis);
	return *this;
}

void ofxQuaternionExtra::rotate(ofxAxisAngle aa) {
	(*this) *= ofQuaternion(aa.angle,aa.axis);
}

void ofxQuaternionExtra::rotateX(float angle) { //degrees?
	rotate(ofxAxisAngle(1,0,0,angle));
}

void ofxQuaternionExtra::rotateY(float angle) {
	rotate(ofxAxisAngle(0,1,0,angle));
}

void ofxQuaternionExtra::rotateZ(float angle) {
	rotate(ofxAxisAngle(0,0,1,angle));
}

ostream& operator <<(ostream &os,const ofxLatLon ll) {
	os << ll.lat << "," << ll.lon; 
	return os;
}

ostream& operator <<(ostream &os,const ofxCartesian c) {
	os << c.x << "," << c.y << "," << c.z; 
	return os;
}

ofxLatLon::ofxLatLon(float lat, float lon) {
	this->lat = lat;
	this->lon = lon;
}

float iso6709ToFloat(string str)
{
    stringstream ss(str);

    string tempStr, dirStr;
    
    char degreeSign = 0xb0;
    getline(ss, tempStr, degreeSign);
    stringstream degstr(tempStr);
    
    getline(ss, tempStr, '\'');
    stringstream minstr(tempStr);
    
    getline(ss, tempStr, '"');
    stringstream secstr(tempStr);
    
    getline(ss, dirStr);
    
    float degs, mins, secs, dir = 0;
    if (dirStr == "N") dir = 1;
    if (dirStr == "E") dir = 1;
    if (dirStr == "S") dir = -1;
    if (dirStr == "W") dir = -1;
    degstr >> degs;
    minstr >> mins;
    secstr >> secs;
    
    if (dir == 0) { ofLog(OF_LOG_WARNING, "Failed to parse NSEW in Lat/Long string"); }
        
    return dir * (degs + mins/60.0f + secs/3600.0f);
}

ofxLatLon::ofxLatLon(string lat, string lon) {
	this->lat = iso6709ToFloat(lat);
	this->lon = iso6709ToFloat(lon);
}

ofxLatLon::ofxLatLon(ofxQuaternionExtra q) {
	*this = q;
}

ofxQuaternionExtra& ofxQuaternionExtra::operator=(ofQuaternion q) {
	set(q);
	return *this;
}

float ofxQuaternionExtra::getDistance(ofxLatLon ll) {
	ofxLatLon ll2;
	float lat1 = ll.lat;
	float lon1 = ll.lon;
	float lat2 = ll2.lat;
	float lon2 = ll2.lon;
	float R = 1; //6371; // km
	float dLat = ofDegToRad(lat2-lat1);
	float dLon = ofDegToRad(lon2-lon1);
	float a = sin(dLat/2) * sin(dLat/2) + cos(ofDegToRad(lat1)) * cos(ofDegToRad(lat2)) * sin(dLon/2) * sin(dLon/2); 
	float c = 2 * atan2(sqrt(a), sqrt(1-a)); 
	float d = R * c;
	return d;
}

