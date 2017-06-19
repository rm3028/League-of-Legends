/***************************
4099056008 陳昱全 期末Project 6/18
4099056030 黃湋縜 期末Project 6/18
***************************/
#include "stdafx.h"
#include "Camera.h"


//Construct a camera
CCamera::CCamera(GLvoid)
{
	vEye.SetVector(0.0, 0.0, 0.0);
	vCenter.SetVector(0.0, 0.0, 0.0);
	vUp.SetVector(0.0, 0.0, 0.0);
	SetFollow(&vEye);

	m_up = GL_FALSE; m_down = GL_FALSE; m_front = GL_FALSE;	m_back = GL_FALSE;	m_left = GL_FALSE;	m_right = GL_FALSE, r_left = GL_FALSE, r_right = GL_FALSE;
}


//Construct a camera with data
CCamera::CCamera(GLfloat vEye_x, GLfloat vEye_y, GLfloat vEye_z,
				 GLfloat vCenter_x, GLfloat vCenter_y, GLfloat vCenter_z,
				 GLfloat vUp_x, GLfloat vUp_y, GLfloat vUp_z,
				 CVector *v_follow)
{
	vEye.SetVector(vEye_x, vEye_y, vEye_z);
	vCenter.SetVector(vCenter_x, vCenter_y, vCenter_z);
	vUp.SetVector(vUp_x, vUp_y, vUp_z);
	SetFollow(v_follow);

	m_up = GL_FALSE; m_down = GL_FALSE; m_front = GL_FALSE;	m_back = GL_FALSE;	m_left = GL_FALSE;	m_right = GL_FALSE, r_left = GL_FALSE, r_right = GL_FALSE;
}


//Construct a camera with vector data
CCamera::CCamera(CVector v_eye, CVector v_center, CVector v_up,  CVector *v_follow)
{
	vEye = v_eye;
	vCenter = v_center;
	vUp = v_up;
	SetFollow(v_follow);

	m_up = GL_FALSE; m_down = GL_FALSE; m_front = GL_FALSE;	m_back = GL_FALSE;	m_left = GL_FALSE;	m_right = GL_FALSE, r_left = GL_FALSE, r_right = GL_FALSE;
}


//Deconstruct a camera
CCamera::~CCamera(GLvoid)
{
}


//Return the angle between camera and (1.0, 0.0, 0.0);
GLfloat CCamera::GetAngleY(GLvoid)
{
	CVector vVector(vCenter.x - vEye.x, 0.0, vCenter.z - vEye.z);
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


//Return the angle between camera and XZ plane;
GLfloat CCamera::GetAngleZ(GLvoid)
{
	CVector vVector(vCenter.x - vEye.x, vCenter.y - vEye.y, vCenter.z - vEye.z);
	CVector vVectorXZ(vCenter.x - vEye.x, 0.0, vCenter.z - vEye.z);

	if(vVector.y > 0)
		return (acos(Dot(vVector, vVectorXZ) / Magnitude(vVector) / Magnitude(vVectorXZ)) * 180 / M_PI);
	else if(vVector.y < 0)
		return - (acos(Dot(vVector, vVectorXZ) / Magnitude(vVector) / Magnitude(vVectorXZ)) * 180 / M_PI);
	else
		return 0.0;
}


//Set camera vector data
GLvoid CCamera::SetCamera(GLfloat vEye_x, GLfloat vEye_y, GLfloat vEye_z,
						GLfloat vCenter_x, GLfloat vCenter_y, GLfloat vCenter_z,
						GLfloat vUp_x, GLfloat vUp_y, GLfloat vUp_z)
{
	vEye.SetVector(vEye_x, vEye_y, vEye_z);
	vCenter.SetVector(vCenter_x, vCenter_y, vCenter_z);
	vUp.SetVector(vUp_x, vUp_y, vUp_z);
	SetFollow(vFollow);
}


//Set follow vector
GLvoid CCamera::SetFollow(CVector *vVector)
{
	vFollow = vVector;

	vEyeOffset = vEye - *vFollow;
	vCenterOffset = vCenter - *vFollow;
}


//Calling OpenGL function to see the scene
GLvoid CCamera::LookAt(GLvoid)
{
	vEye = *vFollow + vEyeOffset;
	vCenter = *vFollow + vCenterOffset;
	
	glLoadIdentity();
	gluLookAt(vEye.x, vEye.y, vEye.z,
			  vCenter.x, vCenter.y, vCenter.z,
			  vUp.x, vUp.y, vUp.z);
}


//Camera movement handler
GLvoid CCamera::Movement(GLfloat m_speed)
{
	if(m_up)
	{
		vEye = vEye + (Normalize(vUp) * m_speed);
		vCenter = vCenter + (Normalize(vUp) * m_speed);
	}
	if(m_down)
	{
		vEye = vEye - (Normalize(vUp) * m_speed);
		vCenter = vCenter - (Normalize(vUp) * m_speed);
	}
	if(m_front)
	{
		CVector vVector = vCenter - vEye;
		vVector = Normalize(vVector);
		vEye = vEye + (vVector * m_speed);
		vCenter = vCenter + (vVector * m_speed);
	}
	if(m_back)
	{
		CVector vVector = vCenter - vEye;
		vVector = Normalize(vVector);
		vEye = vEye - (vVector * m_speed);
		vCenter = vCenter - (vVector * m_speed);
	}
	if(m_left)
	{
		CVector vVector = Cross(vUp, vCenter - vEye);
		vVector = Normalize(vVector);
		vEye = vEye + (vVector * m_speed);
		vCenter = vCenter + (vVector * m_speed);
	}
	if(m_right)
	{
		CVector vVector = Cross(vCenter - vEye, vUp);
		vVector = Normalize(vVector);
		vEye = vEye + (vVector * m_speed);
		vCenter = vCenter + (vVector * m_speed);
	}
}


//Reset the movement flag
GLvoid CCamera::MoveReset(GLvoid)
{
	m_up = GL_FALSE; m_down = GL_FALSE; m_front = GL_FALSE;	m_back = GL_FALSE;	m_left = GL_FALSE;	m_right = GL_FALSE;
}


//Switching the move up event 
GLvoid CCamera::MoveUp(GLboolean m_enable)
{
	switch(m_enable)
	{
	case GL_TRUE:
		m_up = GL_TRUE;
		break;
	case GL_FALSE:
		m_up = GL_FALSE;
		break;
	}
}


//Switching the move down event 
GLvoid CCamera::MoveDown(GLboolean m_enable)
{
	switch(m_enable)
	{
	case GL_TRUE:
		m_down = GL_TRUE;
		break;
	case GL_FALSE:
		m_down = GL_FALSE;
		break;
	}
}


//Switching the move front event 
GLvoid CCamera::MoveFront(GLboolean m_enable)
{
	switch(m_enable)
	{
	case GL_TRUE:
		m_front = GL_TRUE;
		break;
	case GL_FALSE:
		m_front = GL_FALSE;
		break;
	}
}


//Switching the move back event 
GLvoid CCamera::MoveBack(GLboolean m_enable)
{
	switch(m_enable)
	{
	case GL_TRUE:
		m_back = GL_TRUE;
		break;
	case GL_FALSE:
		m_back = GL_FALSE;
		break;
	}
}


//Switching the move left event 
GLvoid CCamera::MoveLeft(GLboolean m_enable)
{
	switch(m_enable)
	{
	case GL_TRUE:
		m_left = GL_TRUE;
		break;
	case GL_FALSE:
		m_left = GL_FALSE;
		break;
	}
}


//Switching the move right event 
GLvoid CCamera::MoveRight(GLboolean m_enable)
{
	switch(m_enable)
	{
	case GL_TRUE:
		m_right = GL_TRUE;
		break;
	case GL_FALSE:
		m_right = GL_FALSE;
		break;
	}
}


//Camera move front handler
GLvoid CCamera::EyeFront(GLfloat m_speed)
{
	CVector vVector = Normalize(vCenter - vEye);
	vEyeOffset = vEyeOffset + (vVector * m_speed);
	vCenterOffset = vCenterOffset + (vVector * m_speed);
}


//Camera move back handler
GLvoid CCamera::EyeBack(GLfloat m_speed)
{
	CVector vVector = Normalize(vCenter - vEye);
	vEyeOffset = vEyeOffset - (vVector * m_speed);
	vCenterOffset = vCenterOffset - (vVector * m_speed);
}


//Camera rotation handler
GLvoid CCamera::MoveRotate(GLfloat m_angle, CVector vPoint, CVector vAxis)
{
	GLfloat sinTheta = sin(M_PI * m_angle / 180.0);
	GLfloat cosTheta = cos(M_PI * m_angle / 180.0);
	GLfloat dotAxis = Dot(vAxis, vAxis);
	GLfloat magnitudeAxis = Magnitude(vAxis);
	
	vEye = vEyeOffset;
	vCenter = vCenterOffset;

	vEye.SetVector(((GLfloat)pow(vAxis.x, 2) + ((GLfloat)pow(vAxis.y, 2) + (GLfloat)pow(vAxis.z, 2)) * cosTheta) / dotAxis * vEye.x +
				   (vAxis.x * vAxis.y * (1 - cosTheta) / dotAxis - vAxis.z * sinTheta / magnitudeAxis)					   * vEye.y +
				   (vAxis.x * vAxis.z * (1 - cosTheta) / dotAxis + vAxis.y * sinTheta / magnitudeAxis)					   * vEye.z ,
				   (vAxis.y * vAxis.x * (1 - cosTheta) / dotAxis + vAxis.z * sinTheta / magnitudeAxis)					   * vEye.x +
				   ((GLfloat)pow(vAxis.y, 2) + ((GLfloat)pow(vAxis.x, 2) + (GLfloat)pow(vAxis.z, 2)) * cosTheta) / dotAxis * vEye.y +
				   (vAxis.y * vAxis.z * (1 - cosTheta) / dotAxis - vAxis.x * sinTheta / magnitudeAxis)					   * vEye.z ,
				   (vAxis.z * vAxis.x * (1 - cosTheta) / dotAxis - vAxis.y * sinTheta / magnitudeAxis)					   * vEye.x +
				   (vAxis.z * vAxis.y * (1 - cosTheta) / dotAxis + vAxis.x * sinTheta / magnitudeAxis)					   * vEye.y +
				   ((GLfloat)pow(vAxis.z, 2) + ((GLfloat)pow(vAxis.x, 2) + (GLfloat)pow(vAxis.y, 2)) * cosTheta) / dotAxis * vEye.z );

	vCenter.SetVector(((GLfloat)pow(vAxis.x, 2) + ((GLfloat)pow(vAxis.y, 2) + (GLfloat)pow(vAxis.z, 2)) * cosTheta) / dotAxis * vCenter.x +
					  (vAxis.x * vAxis.y * (1 - cosTheta) / dotAxis - vAxis.z * sinTheta / magnitudeAxis)				 	  * vCenter.y +
					  (vAxis.x * vAxis.z * (1 - cosTheta) / dotAxis + vAxis.y * sinTheta / magnitudeAxis)				 	  * vCenter.z ,
					  (vAxis.y * vAxis.x * (1 - cosTheta) / dotAxis + vAxis.z * sinTheta / magnitudeAxis)				 	  * vCenter.x +
					  ((GLfloat)pow(vAxis.y, 2) + ((GLfloat)pow(vAxis.x, 2) + (GLfloat)pow(vAxis.z, 2)) * cosTheta) / dotAxis * vCenter.y +
					  (vAxis.y * vAxis.z * (1 - cosTheta) / dotAxis - vAxis.x * sinTheta / magnitudeAxis)					  * vCenter.z ,
					  (vAxis.z * vAxis.x * (1 - cosTheta) / dotAxis - vAxis.y * sinTheta / magnitudeAxis)					  * vCenter.x +
					  (vAxis.z * vAxis.y * (1 - cosTheta) / dotAxis + vAxis.x * sinTheta / magnitudeAxis)					  * vCenter.y +
					  ((GLfloat)pow(vAxis.z, 2) + ((GLfloat)pow(vAxis.x, 2) + (GLfloat)pow(vAxis.y, 2)) * cosTheta) / dotAxis * vCenter.z );

	vEyeOffset = vEye;
	vCenterOffset = vCenter;
}


//Rotate the camera up or down by angle r_speed;
GLvoid CCamera::TurnUpDown(GLfloat r_speed)
{
	CVector vAxis(Cross(vCenter - vEye, vUp));
	MoveRotate(r_speed, *vFollow, Normalize(vAxis));

	CVector vVector = vCenter - vEye;
	vVector.y = 0.0;
	vUp = Normalize(Cross(vAxis, vVector));
}


//Rotate the camera left or right by angle r_speed;
GLvoid CCamera::TurnAround(GLfloat r_speed)
{
	CVector vAxis(vUp);
	MoveRotate(r_speed, *vFollow, vAxis);
}


//Switching the turn left event 
GLvoid CCamera::TurnLeft(GLboolean r_enable)
{
	switch(r_enable) {
	case GL_TRUE:
		r_left = GL_TRUE;
		break;
	case GL_FALSE:
		r_left  = GL_FALSE;
		break;
	}
}


//Switching the turn right event 
GLvoid CCamera::TurnRight(GLboolean r_enable)
{
	switch(r_enable) {
	case GL_TRUE:
		r_right = GL_TRUE;
		break;
	case GL_FALSE:
		r_right  = GL_FALSE;
		break;
	}
}


//Camera rotation handler
GLvoid CCamera::TurnSet(GLfloat r_speed)
{
	if(r_left) {
		TurnAround(r_speed);
	}
	if(r_right) {
		TurnAround(-r_speed);
	}
}


//Reset rotation flag
GLvoid CCamera::TurnReset(GLvoid)
{
	r_left = GL_FALSE, r_right = GL_FALSE;
}
