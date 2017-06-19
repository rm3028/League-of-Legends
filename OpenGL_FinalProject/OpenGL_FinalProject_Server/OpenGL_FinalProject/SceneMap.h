/***************************
4099056008 陳昱全 期末Project 6/18
4099056030 黃湋縜 期末Project 6/18
***************************/
#pragma once
#include <atlimage.h>
#include "Obj_Loader\glm.h"
#include "Vector.h"
#include <GL\glut.h>


#define SCENEWIDTH 2000
#define SCENEOFFSET 1000


class CSceneMap
{
private:
	GLuint m_file,	//Data of scene model
		m_texture,	//Information of scene's texture
		m_aerialTexture,	//Information of scene's aerial texture
		m_monLocation, //Information of monster texture
		m_masLocation, //Information of master texture
		m_width,	//Information of scene's width
		m_height;	//Information of scene's height

	GLboolean *m_data;	//Information of scene's collision data

public:
	//Load scene data to memory
	CSceneMap(GLvoid);
	//Deconstruct the scene
	~CSceneMap(GLvoid);

	//Get scene's width
	GLuint GetMapWidth(GLvoid) { return m_width; }
	//Get scene's height
	GLuint GetMapHeight(GLvoid) { return m_height; }
	//Get scene's collision data
	GLboolean* GetMapData(GLvoid) { return m_data; }
	//Load scene model to memory
	GLboolean LoadObj(GLuint *id, GLfloat scale, GLuint mode, char *path);
	//Load scene's texture file to memory
	GLboolean LoadTexture(GLuint *id, char *path);
	//Load scene's collision data to memory
	GLboolean LoadSceneData(GLboolean **sceneData, char *path);
	//Load scene's aerial texture to memory
	GLboolean LoadAerialMap(GLvoid);

	//Collision detection
	GLboolean CollisionTest(GLfloat locationX, GLfloat locationY, GLfloat locationZ);

	//Rendering the scene by OpenGL function
	GLvoid ShowMap(GLvoid);
	//Rendering the aerial of the scene by OpenGL function
	GLvoid ShowAerialMap(GLvoid);
	GLvoid SetMonAerialLoc(CVector location, GLfloat locAngle);
	GLvoid SetMasAerialLoc(CVector location, GLfloat locAngle);
};

