/***************************
4099056008 陳昱全 期末Project 6/18
4099056030 黃湋縜 期末Project 6/18
***************************/
#include "stdafx.h"
#include "SceneMap.h"

//Load scene data to memory
CSceneMap::CSceneMap(GLvoid)
{
	LoadObj(&m_file, 1.0, GLM_TEXTURE, ".\\res\\Scene\\SceneMap.obj");
	LoadTexture(&m_texture, ".\\res\\Scene\\SceneTexture.png");
	LoadSceneData(&m_data, ".\\res\\Scene\\SceneData.hmp");
}


//Deconstruct the scene
CSceneMap::~CSceneMap(GLvoid)
{
	delete[] m_data;
}


//Load scene model to memory
GLboolean CSceneMap::LoadObj(GLuint *id, GLfloat scale, GLuint mode, char *path)
{
	GLMmodel *glm_model;

    glm_model = glmReadOBJ(path);
    glmScale(glm_model, scale);
    glmFacetNormals(glm_model);
    glmVertexNormals(glm_model, 90);
	*id = glmList(glm_model, mode | GLM_SMOOTH);

	glmDelete( glm_model );

	return GL_TRUE;
}


//Load scene's texture file to memory
GLboolean CSceneMap::LoadTexture(GLuint *id, char *path)
{
	USES_CONVERSION;

	GLuint width, height;
	CImage tex;

	if(tex.Load(A2W(path)))
		return GL_FALSE;

	width = tex.GetWidth();
	height = tex.GetHeight();

	GLubyte *texImg = new BYTE[height * width * 4];

	if(tex.GetBPP() == 32)
	{
		for (int y = 0; y < height; y++) 
		{
			for (int x = 0; x < width; x++)
			{
				GLubyte *img = (GLubyte*)tex.GetPixelAddress(x, height - 1 - y);
				texImg[width * y * 4 + 4 * x + 0] = *(img + 2);
				texImg[width * y * 4 + 4 * x + 1] = *(img + 1);
				texImg[width * y * 4 + 4 * x + 2] = *(img + 0);
				texImg[width * y * 4 + 4 * x + 3] = *(img + 3);
			}
		}
	}
	else if(tex.GetBPP()==24)
	{
		for (int y = 0; y < height; y++) 
		{
			for (int x = 0; x < width; x++)
			{
				GLubyte *img = (GLubyte*)tex.GetPixelAddress(x, height - 1 - y);
				texImg[width * 4 * y + 4 * x + 0] = *(img + 2);
				texImg[width * 4 * y + 4 * x + 1] = *(img + 1);
				texImg[width * 4 * y + 4 * x + 2] = *(img + 0);
				texImg[width * 4 * y + 4 * x + 3] = 255;
			}
		}
	}

	glGenTextures(1, id);
    glBindTexture(GL_TEXTURE_2D, *id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texImg);

	delete[] texImg;

	return GL_TRUE;
}


//Load scene's collision data to memory
GLboolean CSceneMap::LoadSceneData(GLboolean **sceneData, char *path)
{
	USES_CONVERSION;

	CImage tex;

	if(tex.Load(A2W(path))) {
		*sceneData = nullptr;
		return GL_FALSE;
	}

	m_width = tex.GetWidth();
	m_height = tex.GetHeight();


	GLboolean *heightMap = new GLboolean[m_height * m_width];

	if(tex.GetBPP()==32)
	{
		for (int y = 0; y < m_height; y++) 
		{
			for (int x = 0; x < m_width; x++)
			{
				GLubyte *img = (GLubyte*)tex.GetPixelAddress(x, y);

				if(*(img + 2) == 255)
					heightMap[m_width * y + x + 0] = GL_TRUE;
				else
					heightMap[m_width * y + x + 0] = GL_FALSE;
			}
		}
	}
	else if(tex.GetBPP()==24)
	{
		for (int y = 0; y < m_height; y++) 
		{
			for (int x = 0; x < m_width; x++)
			{
				GLubyte *img = (GLubyte*)tex.GetPixelAddress(x, y);

				if(*(img + 2) > 200)
					heightMap[m_width * y + x] = GL_TRUE;
				else
					heightMap[m_width * y + x] = GL_FALSE;
			}
		}
	}

	*sceneData = heightMap;
	return GL_TRUE;
}


//Load scene's aerial texture to memory
GLboolean CSceneMap::LoadAerialMap(GLvoid)
{
	return (LoadTexture(&m_aerialTexture, ".\\res\\Scene\\AerialScene.png") && LoadTexture(&m_monLocation, ".\\res\\Scene\\Red_Arrow_Up.png") && LoadTexture(&m_masLocation, ".\\res\\Scene\\Green_Arrow_Up.png"));
}


//Collision detection
GLboolean CSceneMap::CollisionTest(GLfloat locationX, GLfloat locationY, GLfloat locationZ)
{
	//Beyond the scenes
	if(m_data != nullptr && (locationX > SCENEOFFSET || locationX < -SCENEOFFSET || locationZ > SCENEOFFSET || locationZ < -SCENEOFFSET))
		return GL_TRUE;

	//Collision is Detected
	if(m_data != nullptr && m_data[m_width * (GLuint)((locationZ + SCENEOFFSET) / SCENEWIDTH * (m_height - 1) + 0.5) + (GLuint)((locationX + SCENEOFFSET) / SCENEWIDTH * (m_width - 1) + 0.5)])
		return GL_TRUE;

	return GL_FALSE;
}


//Rendering the scene by OpenGL function
GLvoid CSceneMap::ShowMap(GLvoid)
{
	glPushAttrib(GL_ENABLE_BIT);

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND); 

	glPushMatrix();
		glBindTexture(GL_TEXTURE_2D, m_texture);
		glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		GLfloat mapMtl[] = { 0.8, 0.8, 0.8, 1.0 };
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mapMtl);
		glCallList(m_file);
	glPopMatrix();

	glPopAttrib();
}


//Rendering the aerial of the scene by OpenGL function
GLvoid CSceneMap::ShowAerialMap(GLvoid)
{
	glPushAttrib(GL_ENABLE_BIT);
	
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND); 

	glPushMatrix();
		glBindTexture(GL_TEXTURE_2D, m_aerialTexture);
		glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_QUADS);
			glTexCoord2f(0.0, 0.0); glVertex3f(-300.0, -300.0, 0.0);
			glTexCoord2f(1.0, 0.0); glVertex3f(300.0, -300.0, 0.0);
			glTexCoord2f(1.0, 1.0); glVertex3f(300.0, 300.0, 0.0);
			glTexCoord2f(0.0, 1.0); glVertex3f(-300.0, 300.0, 0.0);
		glEnd();
	glPopMatrix();

	glPopAttrib();
}


GLvoid CSceneMap::SetMonAerialLoc(CVector location, GLfloat locAngle)
{
	glPushAttrib(GL_ENABLE_BIT);
	
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);

	glPushMatrix();
		glTranslatef(location.x * 0.3, - location.z * 0.3, 0.0);
		glRotatef(locAngle, 0.0, 0.0, 1.0);
		glBindTexture(GL_TEXTURE_2D, m_monLocation);
		glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_QUADS);
			glTexCoord2f(0.0, 0.0); glVertex3f(-30.0, -30.0, 0.0);
			glTexCoord2f(1.0, 0.0); glVertex3f(30.0, -30.0, 0.0);
			glTexCoord2f(1.0, 1.0); glVertex3f(30.0, 30.0, 0.0);
			glTexCoord2f(0.0, 1.0); glVertex3f(-30.0, 30.0, 0.0);
		glEnd();
	glPopMatrix();

	glPopAttrib();
}


GLvoid CSceneMap::SetMasAerialLoc(CVector location, GLfloat locAngle)
{
	glPushAttrib(GL_ENABLE_BIT);
	
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);

	glPushMatrix();
		glTranslatef(location.x * 0.3, - location.z * 0.3, 0.0);
		glRotatef(locAngle, 0.0, 0.0, 1.0);
		glBindTexture(GL_TEXTURE_2D, m_masLocation);
		glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_QUADS);
			glTexCoord2f(0.0, 0.0); glVertex3f(-30.0, -30.0, 0.0);
			glTexCoord2f(1.0, 0.0); glVertex3f(30.0, -30.0, 0.0);
			glTexCoord2f(1.0, 1.0); glVertex3f(30.0, 30.0, 0.0);
			glTexCoord2f(0.0, 1.0); glVertex3f(-30.0, 30.0, 0.0);
		glEnd();
	glPopMatrix();

	glPopAttrib();
}
