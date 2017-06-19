/***************************
4099056008 陳昱全 期末Project 6/18
4099056030 黃湋縜 期末Project 6/18
***************************/
#include "stdafx.h"
#include "Vector.h"


//Construct a vector
CVector::CVector(GLvoid)
{
	x = 0.0;	y = 0.0;	z = 0.0;
}


//Construct a vector with data
CVector::CVector(GLfloat vVector_x, GLfloat vVector_y, GLfloat vVector_z)
{
	x = vVector_x;	y = vVector_y;	z = vVector_z;
}


//Construct a vector by a vector
CVector::CVector(const CVector &vVector)
{
	x = vVector.x;
	y = vVector.y;
	z = vVector.z;
}


//Deconstruct a vector
CVector::~CVector(GLvoid)
{
}


//Addition operator of vector
CVector CVector::operator+(CVector vVector)
{
	return CVector(x + vVector.x, y + vVector.y, z + vVector.z);
}


//Minus operator of vector
CVector CVector::operator-(CVector vVector)
{
	return CVector(x - vVector.x, y - vVector.y, z - vVector.z);
}


//Multiply operator of vector
CVector CVector::operator*(GLfloat num)
{
	return CVector(x * num, y * num, z * num);
}


//Division operator of vector
CVector CVector::operator/(GLfloat num)
{
	return CVector(x / num, y / num, z / num);
}


//Set the vector data
GLvoid CVector::SetVector(GLfloat vVector_x, GLfloat vVector_y, GLfloat vVector_z)
{
	x = vVector_x;	y = vVector_y;	z = vVector_z;
}


//Return the Magnitude of a vector
GLfloat Magnitude(CVector vVector)
{
	return sqrt(Dot(vVector, vVector));
}


//Return the normalized vector of a vector
CVector Normalize(CVector vVector)
{
	GLfloat magnitude = Magnitude(vVector);
	return vVector / magnitude;
}


//Return the cross vector of two vectors
CVector Cross(CVector vVector1, CVector vVector2)
{
	return CVector((vVector1.y * vVector2.z) - (vVector2.y * vVector1.z),
				   (vVector1.z * vVector2.x) - (vVector2.z * vVector1.x),
				   (vVector1.x * vVector2.y) - (vVector2.x * vVector1.y));
}


//Return the dot value of two vectors
GLfloat Dot(CVector vVector1, CVector vVector2)
{
	return vVector1.x * vVector2.x + vVector1.y * vVector2.y + vVector1.z * vVector2.z;
}


//Return the distance between two vectors
GLfloat Distance(CVector vVector1, CVector vVector2)
{
	return Magnitude(vVector1 - vVector2);
}


GLfloat Angle(CVector vVector1, CVector vVector2)
{
	return (GLfloat)abs(acos(Dot(vVector1, vVector2) / Magnitude(vVector1) / Magnitude(vVector2)) * 180 / M_PI);
}



GLfloat ComputeAngleY(CVector vVector)
{
	CVector vVectorX(1.0, 0.0, 0.0);

	if(vVector.z < 0)
		return (acos(Dot(vVector, vVectorX) / Magnitude(vVector) / Magnitude(vVectorX)) * 180 / M_PI);
	else if(vVector.z > 0)
		return - (acos(Dot(vVector, vVectorX) / Magnitude(vVector) / Magnitude(vVectorX)) * 180 / M_PI);
	else if(vVector.x > 0)
		return 0.0;
	else if(vVector.x < 0)
		return 180.0;
}