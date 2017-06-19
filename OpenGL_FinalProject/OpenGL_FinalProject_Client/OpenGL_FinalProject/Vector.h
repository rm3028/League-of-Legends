/***************************
4099056008 陳昱全 期末Project 6/18
4099056030 黃湋縜 期末Project 6/18
***************************/
#pragma once
#define _USE_MATH_DEFINES
#include <cmath>
#include <GL\glut.h>


class CVector
{
public:
	CVector(GLvoid);	//Construct a vector
	CVector(GLfloat  vVector_x, GLfloat vVector_y, GLfloat  vVector_z);	//Construct a vector with data
	CVector(const CVector &vVector);	//Construct a vector by a vector
	~CVector(GLvoid);	//Deconstruct a vector

	GLfloat x, y, z;	//Vector data

public:
	CVector operator+(CVector vVector);	//Addition operator of vector
	CVector operator-(CVector vVector);	//Minus operator of vector
	CVector operator*(GLfloat num);	//Multiply operator of vector
	CVector operator/(GLfloat num);	//Division operator of vector

	GLvoid SetVector(GLfloat vVector_x, GLfloat vVector_y, GLfloat vVector_z);	//Set the vector data

	friend GLfloat Magnitude(CVector vVector);	//Return the Magnitude of a vector
	friend CVector Normalize(CVector vVector);	//Return the normalized vector of a vector
	friend CVector Cross(CVector vVector1, CVector vVector2);	//Return the cross vector of two vectors
	friend GLfloat Dot(CVector vVector1, CVector vVector2);	//Return the dot value of two vectors
	friend GLfloat Distance(CVector vVector1, CVector vVector2);	//Return the distance between two vectors
	friend GLfloat Angle(CVector vVector1, CVector vVector2);	//Return the angle between two vectors
	friend GLfloat ComputeAngleY(CVector vVector);
};
