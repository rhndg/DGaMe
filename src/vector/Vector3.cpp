#include "Vector3.h"
#include <math.h>

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

void Vector3::set(float x_,float y_,float z_){
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

Vector3 Vector3::sub(Vector3 V){
	return Vector3(x-V.x,y-V.y,z-V.z);
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

// Vector3 Vector3::rotate(Vector3 axis,float rad)			//left->counter clockwise
// {
// 	Vector3 component1 = this->mult(cos(rad));
// 	Vector3 component2 = (this->cross(axis)).mult(-sin(rad));
// 	return component1.add(component2);
// }

Vector3 Vector3::rotate(Vector3 axis,float r)			//left->counter clockwise
{
 	Vector3 component1 = this->mult(r);
 	float a = acos(r);
 	Vector3 component2 = (this->cross(axis)).mult(-sin(a));
 	return component1.add(component2);
}
bool Vector3::equal(Vector3 v)
{
	return (x==v.x && y==v.y && z==v.z);
} 
