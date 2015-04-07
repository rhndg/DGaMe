#ifndef VECTOR3_H
#define VECTOR3_H

#include <bits/stdc++.h>

class Vector3 {
public:
	float x,y,z; //denote the x, y and z components of the vector 
	/**
	 * @brief Constructor
	 * @details Initializes the vector with the passed parameters
	 * 
	 * @param x_ x-component of the vector
	 * @param y_ y-component of the vector
	 * @param z_ z-component of the vector
	 */
	Vector3(float x_,float y_,float z_);
	/**
	 * @brief initialized by zero vector
	 */
	Vector3();
	/**
	 * @brief set the coordinates of the vector
	 *  
	 * @param x_ x coordinate
	 * @param y_ y coordinate
	 * @param z_ z coordinate
	 */
	void set(float x_,float y_,float z_);
	/**
	 * @brief Vector Addition of 2 vectors
	 * @details Adds the vector passed as argument to the function to this vector without updating it
	 * 
	 * @param V The vector of type Vector3 to be added to this vector
	 * @return Vector sum of type Vector3 of this vector and the vector passed as argument 
	 */
	Vector3 add(Vector3 V);
	/**
	 * @brief Vector Negation
	 * @details Switches the sign of each component of the vector giving the reversed vector without updating it
	 * @return Reversed vector of type Vector3
	 */
	Vector3 neg();
	/**
	 * @brief Vector Dot Product
	 * @details Takes the dot product of the passed vector with this vector without updating it
	 * @param V The vector of type Vector3 whose dot product with this vector is to be evaluated
	 * @return Float type dot product of this vector and passed vector 
	 */
	float dot(Vector3 V);
	/**
	 * @brief Vector Magnitude
	 * @details Returns the length of this vector
	 * @return Float type magnitude of this vector
	 */
	float mod();
	/**
	 * @brief Stretch/compress vector
	 * @details Returns a vector of the specified length along this vector without updating this vector
	 * @param R Float type length of the required vector
	 * @return Vector of the specified length along this vector of type Vector3
	 */
	Vector3 setlen(float R);
	/**
	 * @brief Constant multiplication to vector
	 * @details Returns a vector by multiplying the length of this vector by the passed value without updating it
	 * @param C Multiplication factor of type float
	 * @return New multiplied vector of type Vector3
	 */
	Vector3 mult(float C);
	/**
	 * @brief Vector Cross Product
	 * @details Returns the cross product of this vector with the passed vector
	 * @param V The vector of type Vector3 with which cross product needs to be taken
	 * @return Cross product of type Vector3 of this vector with the passed vector 
	 */
	Vector3 cross(Vector3 V);
	
};

#endif