/***************************
4099056008 陳昱全 期末Project 6/18
4099056030 黃湋縜 期末Project 6/18
***************************/
#pragma once
#define _USE_MATH_DEFINES
#include <cmath>
#include <GL\glut.h>
#include "Vector.h"


class CCamera
{
public:
	//Construct a camera
	CCamera(GLvoid);
	//Construct a camera with data
	CCamera(GLfloat vEye_x, GLfloat vEye_y, GLfloat vEye_z,
			GLfloat vCenter_x, GLfloat vCenter_y, GLfloat vCenter_z,
			GLfloat vUp_x, GLfloat vUp_y, GLfloat vUp_z,
			CVector *v_follow);
	//Construct a camera with vector data
	CCamera(CVector v_eye, CVector v_center, CVector v_up,  CVector *v_follow);
	//Deconstruct a camera
	~CCamera(GLvoid);

	//Get the eye vector of the camera
	CVector GetEye()	{ return vEye;	 }
	//Get the center vector of the camera
	CVector GetCenter() { return vCenter;}
	//Get the up vector of the camera
	CVector GetUp()		{ return vUp;	 }

	//Get the eye pointer of the camera
	CVector* GetEyePtr()   { return &vEye;	 }
	//Get the center pointer of the camera
	CVector* GetCenterPtr(){ return &vCenter;}
	//Get the up pointer of the camera
	CVector* GetUpPtr()	   { return &vUp;	 }
	//Get the follow pointer of the camera
	CVector* GetFollowPtr(){ return vFollow; }
	
	//Return the angle between camera and (1.0, 0.0, 0.0);
	GLfloat GetAngleY(GLvoid);
	//Return the angle between camera and XZ plane;
	GLfloat GetAngleZ(GLvoid);

private:
	CVector vEye, vCenter, vUp;	//Camera vector data

	CVector *vFollow;	//Camera follow vector
	CVector vEyeOffset, vCenterOffset;	//Offset between camera data and follow vector

	GLboolean m_up, m_down, m_front, m_back, m_left, m_right, r_left, r_right;	//Camera movement and rotation flag

public:
	//Set camera vector data
	GLvoid SetCamera(GLfloat vEye_x, GLfloat vEye_y, GLfloat vEye_z,
				   GLfloat vCenter_x, GLfloat vCenter_y, GLfloat vCenter_z,
				   GLfloat vUp_x, GLfloat vUp_y, GLfloat vUp_z);
	//Set follow vector
	GLvoid SetFollow(CVector *vVector);

	//Calling OpenGL function to see the scene
	GLvoid LookAt(GLvoid);
	//Camera movement handler
	GLvoid Movement(GLfloat m_speed);
	//Reset the movement flag
	GLvoid MoveReset(GLvoid);

	//Switching the move up event 
	GLvoid MoveUp(GLboolean m_enable);
	//Switching the move down event 
	GLvoid MoveDown(GLboolean m_enable);
	//Switching the move front event 
	GLvoid MoveFront(GLboolean m_enable);
	//Switching the move back event 
	GLvoid MoveBack(GLboolean m_enable);
	//Switching the move left event 
	GLvoid MoveLeft(GLboolean m_enable);
	//Switching the move right event 
	GLvoid MoveRight(GLboolean m_enable);

	//Camera move front handler
	GLvoid EyeFront(GLfloat m_speed);
	//Camera move back handler
	GLvoid EyeBack(GLfloat m_speed);

	//Camera rotation handler
	GLvoid MoveRotate(GLfloat m_angle, CVector vPoint, CVector vAxis);

	//Rotate the camera up or down by angle r_speed;
	GLvoid TurnUpDown(GLfloat r_speed);
	//Rotate the camera left or right by angle r_speed;
	GLvoid TurnAround(GLfloat r_speed);
	//Switching the turn left event 
	GLvoid TurnLeft(GLboolean r_enable);
	//Switching the turn right event 
	GLvoid TurnRight(GLboolean r_enable);
	//Camera rotation handler
	GLvoid TurnSet(GLfloat r_speed);
	//Reset rotation flag
	GLvoid TurnReset(GLvoid);
};

