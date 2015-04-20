#include "Vector3.h"

Vector3::Vector3(float x_,float y_,float z_){
	x=x_;
	y=y_;
	z=z_;
}

Vector3::Vector3(){
	x=0;
	y=0;
	z=0;
}

void Vector3::set(float x_, float y_, float z_){
	x=x_;
	y=y_;
	z=z_;
}

void Vector3::set(Vector3 v){
	x=v.x;
	y=v.y;
	z=v.z;
}

Vector3 Vector3::add(Vector3 V){
	return Vector3(x+V.x,y+V.y,z+V.z);
}

Vector3 Vector3::neg(){
	return Vector3(-x,-y,-z);
}

float Vector3::dot(Vector3 V){
	return x*V.x+y*V.y+z*V.z;
}

float Vector3::mod(){
	return sqrt(x*x+y*y+z*z);
}

Vector3 Vector3::setlen(float R){
	float len=mod();
	return Vector3(R*x/len,R*y/len,R*z/len);
}

Vector3 Vector3::mult(float C){
	return Vector3(x*C,y*C,z*C);
}

Vector3 Vector3::cross(Vector3 V){
	return Vector3(y*V.z-z*V.y,V.x*z-x*V.z,x*V.y-y*V.x);
}
