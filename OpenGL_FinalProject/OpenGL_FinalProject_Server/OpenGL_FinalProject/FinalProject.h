/***************************
4099056008 陳昱全 期末Project 6/18
4099056030 黃湋縜 期末Project 6/18
***************************/
#pragma once
#pragma comment(lib,"winmm.lib")
#include "SocketServer.h"
#include <atlimage.h>
#include "FBX_Loader\FbxMod.h"
#include "Obj_Loader\glm.h"
#include "SceneMap.h"
#include "Server.h"
#include "Client.h"
#include <GL\glut.h>
#include <GL\freeglut_ext.h>
#include <thread>
#include <Windows.h>
#include "mmsystem.h"


using namespace std;

#define  GLUT_WHEEL_UP 3 
#define  GLUT_WHEEL_DOWN 4

namespace FinalProject {
	enum GameState {
		GameStart, 
		GameSelect,
		GameLoading,  
		GameConnect, 
		GamePlay, 
		GameVictory, 
		GameDefeat
	};

	enum GameCharacter {
		SelectAkali, 
		SelectShen, 
		SelectAlistar,
		SelectDiana, 
		SelectLeona, 
		SelectMalphite,
		CharacterNum
	};

	class CFinalProject
	{
	private:
		static CFinalProject *instance;	//Pointer to the FinalProject object
		static GLfloat LightPos[];	//Light0 position

		static GLfloat spotDif[];	//Diffuse of spotlight
		static GLfloat spotSpe[];	//Specular of spotlight
		static GLfloat spotDir[];	//Direction of spotlight

		HDC mainDC;
		HGLRC mainRC, subRC;

		GLboolean gameState;

		GLuint startTex, startNTex;
		GLuint titleTex, loadingIconTex, loadingTex, connectingTex,connectArrowTex;
		GLuint selectLeftTex, selectRightTex;
		GLuint mouseLoc[2];
		GLboolean mouseLock;

		CCamera *camera;
		CSceneMap *sceneMap;
		
		Server *server;
		Client *client;

		SocketServer *socket_server;

		GLuint shenTex, akaliTex, alistarTex, dianaTex, leonaTex,malphiteTex;
		GLuint shenNTex, akaliNTex, alistarNTex, dianaNTex, leonaNTex,malphiteNTex;

		GLuint vicTex, defTex;

		int charSelect;
		thread loadThread, SocketThread;
		GLboolean exitGame;
		GLboolean fogMode;
		GLboolean screenMode;
		GLboolean spotLight;
		
		
	public:
		//Construct the FinalProject object
		CFinalProject();
		//Construct the glut OpenGL environment window
		CFinalProject(int argc, char* argv[]);
		//Deconstruct the FinalProject object
		~CFinalProject(GLvoid);

		//Load obj data to memory
		GLboolean LoadObj(GLuint *id, GLfloat scale, GLuint mode, char *path);
		//Load texture data to memory
		GLboolean LoadTexture(GLuint *id, char *path);

		//Create the glut OpenGL environment window
		GLvoid OnCreate(int argc, char* argv[]);
		//OnClose event
		GLvoid OnClose(GLvoid);

		GLvoid OnLoadData(GLvoid);

		//OnTimer event
		GLvoid OnTimer(int id);
		//OnDisplay event
		GLvoid OnDisplay(GLvoid);
		//OnReshap event
		GLvoid OnReshape(int width, int height);
		//OnMouse event
		GLvoid OnMouseFunc(int button, int state, int x, int y) ;
		GLvoid OnMousePassiveMotion(int x, int y);
		//OnKeyboard event
		GLvoid OnKeyboardFunc(unsigned char key, int x, int y);
		//OnKeyboardUp event
		GLvoid OnKeyboardUpFunc(unsigned char key, int x, int y);

		GLvoid LoadThreadFunc(GLvoid);
		GLvoid SocketServerThreadFunc(GLvoid);
		
		
		//Set close event
		static GLvoid SetClose(GLvoid);
		//Set timer event
		static GLvoid SetTimer(int id);
		//Set display event
		static GLvoid SetDisplay(GLvoid);
		//Set reshape event
		static GLvoid SetReshape(int width, int height);
		//Set mouse event
		static GLvoid SetMouseFunc(int button, int state, int x, int y);
		static GLvoid SetMousePassiveMotion(int x, int y);
		//Set keyboard event
		static GLvoid SetKeyboardFunc(unsigned char key, int x, int y);
		//Set keyboardUp event
		static GLvoid SetKeyboardUpFunc(unsigned char key, int x, int y);

		static GLvoid SetLoadThread();
		static GLvoid SetSocketServerThread();

		static GLvoid playWAV(char*, bool loop);
	};

}
