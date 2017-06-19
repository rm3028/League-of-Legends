/***************************
4099056008 陳昱全 期末Project 6/18
4099056030 黃湋縜 期末Project 6/18
***************************/
#include "stdafx.h"
#include "FinalProject.h"


namespace FinalProject {

	CFinalProject* CFinalProject::instance = nullptr;	//Pointer to the FinalProject object
	GLfloat CFinalProject::LightPos[] = { -400.0, 700.0, 500.0, 0.0 };	//Position of Light0
	GLfloat CFinalProject::spotDif[] = { 1.0, 1.0, 1.0, 1.0 };	//Diffuse of spotlight
	GLfloat CFinalProject::spotSpe[] = { 1.0, 1.0, 1.0, 1.0 };	//Specular of spotlight
	GLfloat CFinalProject::spotDir[] = { 1.0, 0.0, 0.0 };	//Direction of spotlight


	//Construct the FinalProject object
	CFinalProject::CFinalProject(GLvoid)
	{	
	}


	//Construct the glut OpenGL environment window
	CFinalProject::CFinalProject(int argc, char* argv[])
	{	
		OnCreate(argc, argv);
		
	}


	//Deconstruct the FinalProject object
	CFinalProject::~CFinalProject(GLvoid)
	{
		exitGame = GL_TRUE;
		if(gameState >= GameLoading)
			loadThread.join();
		if(gameState >= GameLoading)
			SocketThread.join();
		if(gameState >= GameLoading) {
			socket_client->close();
			delete client,sceneMap, camera,socket_client;
		}
		wglDeleteContext(mainRC);
	}


	//Load obj data to memory
	GLboolean CFinalProject::LoadObj(GLuint *id, GLfloat scale, GLuint mode, char *path)
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


	//Load texture data to memory
	GLboolean CFinalProject::LoadTexture(GLuint *id, char *path)
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
		else if(tex.GetBPP() == 24)
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


	//Create the glut OpenGL environment window
	GLvoid CFinalProject::OnCreate(int argc, char* argv[])
	{
		instance = this;
	
		glutInit(&argc, argv);
		glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);

		glutInitWindowSize(1280, 720);
		glutInitWindowPosition(100, 100);

		glutCreateWindow("成圖技術與應用 Final Project Client");

		screenMode = GL_FALSE;

		playWAV(".\\res\\LoL - Client Login BGM (Season 2 World Championships).wav", true);

		exitGame = GL_FALSE;
		socket_client = new SocketClient();
		
		mainDC = wglGetCurrentDC();
		mainRC = wglCreateContext(mainDC);
		subRC = wglCreateContext(mainDC);
		wglShareLists(mainRC, subRC);
		wglMakeCurrent(mainDC, mainRC);
		
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		glEnable(GL_LIGHT1);
		glEnable(GL_DEPTH_TEST);

		glLightfv(GL_LIGHT1, GL_DIFFUSE, spotDif);
		glLightfv(GL_LIGHT1, GL_SPECULAR, spotSpe);
		glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 15.0);

		spotLight = GL_TRUE;

		glEnable(GL_FOG);
		{
			GLfloat fogColor[4] = {0.5, 0.5, 0.5, 1.0};
			glFogi (GL_FOG_MODE, GL_LINEAR);
			glFogfv (GL_FOG_COLOR, fogColor);
			glFogf (GL_FOG_DENSITY, 0.35);
			glHint (GL_FOG_HINT, GL_DONT_CARE);
			glFogf (GL_FOG_START, 500.0);
			glFogf (GL_FOG_END, 1000.0);
			fogMode = GL_TRUE;
		}

		gameState = GameStart;

		LoadTexture(&startTex, ".\\res\\Texture\\start.jpg");
		LoadTexture(&startNTex, ".\\res\\Texture\\startN.png");

		LoadTexture(&titleTex, ".\\res\\Texture\\LeagueOfLegends.png");
		LoadTexture(&loadingTex, ".\\res\\Texture\\loading.png");
		LoadTexture(&connectingTex, ".\\res\\Texture\\connecting.png");
		LoadTexture(&connectArrowTex, ".\\res\\Texture\\connect_arrow.png");
		LoadTexture(&loadingIconTex, ".\\res\\Texture\\loading_icon.png");
		LoadTexture(&selectLeftTex, ".\\res\\Texture\\selectLeft.png");
		LoadTexture(&selectRightTex, ".\\res\\Texture\\selectRight.png");

		LoadTexture(&shenTex, ".\\res\\Characters\\Shen\\Shen.jpg");
		LoadTexture(&akaliTex, ".\\res\\Characters\\Akali\\Akali.jpg");
		LoadTexture(&alistarTex, ".\\res\\Characters\\Alistar\\Alistar.jpg");
		LoadTexture(&dianaTex, ".\\res\\Characters\\Diana\\Diana.jpg");
		LoadTexture(&leonaTex, ".\\res\\Characters\\Leona\\Leona.jpg");
		LoadTexture(&malphiteTex, ".\\res\\Characters\\Malphite\\Malphite.jpg");

		LoadTexture(&shenNTex, ".\\res\\Characters\\Shen\\ShenN.png");
		LoadTexture(&akaliNTex, ".\\res\\Characters\\Akali\\AkaliN.png");
		LoadTexture(&alistarNTex, ".\\res\\Characters\\Alistar\\AlistarN.png");
		LoadTexture(&dianaNTex, ".\\res\\Characters\\Diana\\DianaN.png");
		LoadTexture(&leonaNTex, ".\\res\\Characters\\Leona\\LeonaN.png");
		LoadTexture(&malphiteNTex, ".\\res\\Characters\\Malphite\\MalphiteN.png");

		LoadTexture(&vicTex, ".\\res\\Texture\\victory.png");
		LoadTexture(&defTex, ".\\res\\Texture\\defeat.png");

		mouseLock = GL_FALSE;

		glutCloseFunc(SetClose);
		glutTimerFunc(33, SetTimer, 0);
		glutReshapeFunc(SetReshape);
		glutDisplayFunc(SetDisplay);
		glutMouseFunc(SetMouseFunc);
		glutPassiveMotionFunc(SetMousePassiveMotion);
		glutKeyboardFunc(SetKeyboardFunc);
		glutKeyboardUpFunc(SetKeyboardUpFunc);

		glutMainLoop();
	}


	//OnClose event
	GLvoid CFinalProject::OnClose(GLvoid)
	{
		this->~CFinalProject();
	}


	//OnTimer event2
	GLvoid CFinalProject::OnTimer(int id)
	{
		switch(id) {
		case 0:
			switch(gameState) {
			case GamePlay: {
				if(Distance(*server->getSelectCharacter()->GetLocation(), *client->getSelectCharacter()->GetLocation()) < 200 && abs(ComputeAngleY(*client->getSelectCharacter()->GetLocation() - *server->getSelectCharacter()->GetLocation()) - server->getSelectCharacter()->GetLocAngle() - server->getSelectCharacter()->GetmoveAngle()) < 45.0) {
					if(server->getSelectCharacter()->GetAttackTiming())
						client->getSelectCharacter()->OnAttacked(server->getSelectCharacter()->GetAttackPower());
				}
				if(Distance(*client->getSelectCharacter()->GetLocation(), *server->getSelectCharacter()->GetLocation()) < 200 && abs(ComputeAngleY(*server->getSelectCharacter()->GetLocation() - *client->getSelectCharacter()->GetLocation()) - client->getSelectCharacter()->GetLocAngle() - client->getSelectCharacter()->GetmoveAngle()) < 45.0) {
					if(client->getSelectCharacter()->GetAttackTiming())
						server->getSelectCharacter()->OnAttacked(client->getSelectCharacter()->GetAttackPower());
				}
				
			break;
						   }
			}

			glutPostRedisplay();
			
			static int lastUpdate = 0;	//FPS printer
			static int frames = 0;
			char title[52];

			int currentTime = glutGet(GLUT_ELAPSED_TIME);
			frames++;

			if ((currentTime - lastUpdate) >= 1000 ) {
				sprintf_s(title, "成圖技術與應用 Final Project Client   FPS: %d", frames);
				glutSetWindowTitle(title);
				frames = 0;
				lastUpdate = currentTime;
			}

			glutTimerFunc(33, SetTimer, 0);
			break;
		}
	}


	//OnDisplay event
	GLvoid CFinalProject::OnDisplay(GLvoid)
	{
		glMatrixMode(GL_MODELVIEW);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		switch(gameState) {
		case GameStart: {
			glPushAttrib(GL_ENABLE_BIT);
			
			glDisable(GL_BLEND); 
			glDisable(GL_LIGHTING);
			glEnable(GL_TEXTURE_2D);

			glMatrixMode(GL_PROJECTION);
				glPushMatrix();
					glLoadIdentity();
					glOrtho(0.0, 1920.0, 0.0, 1080.0, -100.0, 100.0);
		
					glMatrixMode(GL_MODELVIEW);
					glPushMatrix();
						glLoadIdentity();

						glPushMatrix();
							glBindTexture(GL_TEXTURE_2D, startTex);
							glColor3f(1.0, 1.0, 1.0);
							glBegin(GL_QUADS);
								glTexCoord2f(0.0, 0.0); glVertex3f(0.0, 0.0, 0.0);
								glTexCoord2f(1.0, 0.0); glVertex3f(1920.0, 0.0, 0.0);
								glTexCoord2f(1.0, 1.0); glVertex3f(1920.0, 1080.0, 0.0);
								glTexCoord2f(0.0, 1.0); glVertex3f(0.0, 1080.0, 0.0);
							glEnd();
						glPopMatrix();

						glEnable(GL_BLEND); 
						glPushMatrix();
							glBindTexture(GL_TEXTURE_2D, startNTex);
							glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
							static GLuint startCount = 0;
							if(startCount < 15) {
								glColor3f(1.0, 1.0, 1.0);
								glBegin(GL_QUADS);
									glTexCoord2f(0.0, 0.0); glVertex3f(360.0, 50.0, 1.0);
									glTexCoord2f(1.0, 0.0); glVertex3f(1520.0, 50.0, 1.0);
									glTexCoord2f(1.0, 1.0); glVertex3f(1520.0, 250.0, 1.0);
									glTexCoord2f(0.0, 1.0); glVertex3f(360.0, 250.0, 1.0);
								glEnd();
							}
							startCount = (startCount + 1) % 30;
						glPopMatrix();
					glPopMatrix();
					glMatrixMode(GL_PROJECTION);
				glPopMatrix();
			glPopAttrib();
			break;
			}

		case GameSelect: {
			glPushAttrib(GL_ENABLE_BIT);
			
			glDisable(GL_BLEND); 
			glDisable(GL_LIGHTING);
			glEnable(GL_TEXTURE_2D);

			glMatrixMode(GL_PROJECTION);
				glPushMatrix();
					glLoadIdentity();
					glOrtho(0.0, 1920.0, 0.0, 1080.0, -100.0, 100.0);
		
					glMatrixMode(GL_MODELVIEW);
					glPushMatrix();
						glLoadIdentity();

						glPushMatrix();
							switch(charSelect) {
							case SelectAkali:
								glBindTexture(GL_TEXTURE_2D, akaliTex);
								break;

							case SelectAlistar:
								glBindTexture(GL_TEXTURE_2D, alistarTex);
								break;

							case SelectShen:
								glBindTexture(GL_TEXTURE_2D, shenTex);
								break;

							case SelectDiana:
								glBindTexture(GL_TEXTURE_2D, dianaTex);
								break;

							case SelectLeona:
								glBindTexture(GL_TEXTURE_2D, leonaTex);
								break;
							case SelectMalphite:
								glBindTexture(GL_TEXTURE_2D, malphiteTex);
								break;
							}
							glColor3f(1.0, 1.0, 1.0);
							glBegin(GL_QUADS);
								glTexCoord2f(0.0, 0.0); glVertex3f(0.0, 0.0, 0.0);
								glTexCoord2f(1.0, 0.0); glVertex3f(1920.0, 0.0, 0.0);
								glTexCoord2f(1.0, 1.0); glVertex3f(1920.0, 1080.0, 0.0);
								glTexCoord2f(0.0, 1.0); glVertex3f(0.0, 1080.0, 0.0);
							glEnd();
						glPopMatrix();

						glEnable(GL_BLEND); 
						glPushMatrix();
							switch(charSelect) {
							case SelectAkali:
								glBindTexture(GL_TEXTURE_2D, akaliNTex);
								break;

							case SelectAlistar:
								glBindTexture(GL_TEXTURE_2D, alistarNTex);
								break;

							case SelectShen:
								glBindTexture(GL_TEXTURE_2D, shenNTex);
								break;

							case SelectDiana:
								glBindTexture(GL_TEXTURE_2D, dianaNTex);
								break;

							case SelectLeona:
								glBindTexture(GL_TEXTURE_2D, leonaNTex);
								break;
							case SelectMalphite:
								glBindTexture(GL_TEXTURE_2D, malphiteNTex);
								break;
							}
							glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
							glColor3f(1.0, 1.0, 1.0);
							static GLuint selectCount = 0;
							if(selectCount < 15) {
								glBegin(GL_QUADS);
									glTexCoord2f(0.0, 0.0); glVertex3f(660.0, 0.0, 1.0);
									glTexCoord2f(1.0, 0.0); glVertex3f(1260.0, 0.0, 1.0);
									glTexCoord2f(1.0, 1.0); glVertex3f(1260.0, 300.0, 1.0);
									glTexCoord2f(0.0, 1.0); glVertex3f(660.0, 300.0, 1.0);
								glEnd();
							}
							selectCount = (selectCount + 1) % 30;
						glPopMatrix();

						glPushMatrix();
							glBindTexture(GL_TEXTURE_2D, selectLeftTex);
							glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
							glColor3f(1.0, 1.0, 1.0);
							glBegin(GL_QUADS);
								glTexCoord2f(0.0, 0.0); glVertex3f(0.0, 0.0, 1.0);
								glTexCoord2f(1.0, 0.0); glVertex3f(300.0, 0.0, 1.0);
								glTexCoord2f(1.0, 1.0); glVertex3f(300.0, 300.0, 1.0);
								glTexCoord2f(0.0, 1.0); glVertex3f(0.0, 300.0, 1.0);
							glEnd();
						glPopMatrix();

						glPushMatrix();
							glBindTexture(GL_TEXTURE_2D, selectRightTex);
							glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
							glColor3f(1.0, 1.0, 1.0);
							glBegin(GL_QUADS);
								glTexCoord2f(0.0, 0.0); glVertex3f(1620.0, 0.0, 1.0);
								glTexCoord2f(1.0, 0.0); glVertex3f(1920.0, 0.0, 1.0);
								glTexCoord2f(1.0, 1.0); glVertex3f(1920.0, 300.0, 1.0);
								glTexCoord2f(0.0, 1.0); glVertex3f(1620.0, 300.0, 1.0);
							glEnd();
						glPopMatrix();

					glPopMatrix();
					glMatrixMode(GL_PROJECTION);
				glPopMatrix();

			glPopAttrib();
			break;
			}

		case GameLoading: {
			glPushAttrib(GL_ENABLE_BIT);
	
			glDisable(GL_LIGHTING);
			glEnable(GL_TEXTURE_2D);
			glEnable(GL_BLEND); 

			glMatrixMode(GL_PROJECTION);
				glPushMatrix();
					glLoadIdentity();
					glOrtho(0.0, 1280.0, 0.0, 720.0, -100.0, 100.0);
		
					glMatrixMode(GL_MODELVIEW);
					glPushMatrix();
						glLoadIdentity();

						glPushMatrix();
							glBindTexture(GL_TEXTURE_2D, titleTex);
							glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
							glColor3f(1.0, 1.0, 1.0);
							glBegin(GL_QUADS);
								glTexCoord2f(0.0, 0.0); glVertex3f(390.0, 110.0, 0.0);
								glTexCoord2f(1.0, 0.0); glVertex3f(890.0, 110.0, 0.0);
								glTexCoord2f(1.0, 1.0); glVertex3f(890.0, 610.0, 0.0);
								glTexCoord2f(0.0, 1.0); glVertex3f(390.0, 610.0, 0.0);
							glEnd();
						glPopMatrix();

						glPushMatrix();
							glBindTexture(GL_TEXTURE_2D, loadingTex);
							glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
							glColor3f(1.0, 1.0, 1.0);
							static GLuint loadCount = 0;
							if(loadCount < 15) {
								glBegin(GL_QUADS);
									glTexCoord2f(0.0, 0.0); glVertex3f(640.0, 200.0, 1.0);
									glTexCoord2f(1.0, 0.0); glVertex3f(890.0, 200.0, 1.0);
									glTexCoord2f(1.0, 1.0); glVertex3f(890.0, 300.0, 1.0);
									glTexCoord2f(0.0, 1.0); glVertex3f(640.0, 300.0, 1.0);
								glEnd();
							}
							loadCount = (loadCount + 1) % 30;
						glPopMatrix();

						glPushMatrix();
							static GLfloat loadingAng = 0;
							glTranslatef(1180.0, 100.0, 0.0);
							glRotatef(loadingAng, 0.0, 0.0, 1.0);
							loadingAng = (GLuint)(loadingAng + 10) % 360;
							glBindTexture(GL_TEXTURE_2D, loadingIconTex);
							glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
							glColor3f(1.0, 1.0, 1.0);
							glBegin(GL_QUADS);
								glTexCoord2f(0.0, 0.0); glVertex3f(-50.0, -50.0, 0.0);
								glTexCoord2f(1.0, 0.0); glVertex3f(50.0, -50.0, 0.0);
								glTexCoord2f(1.0, 1.0); glVertex3f(50.0, 50.0, 0.0);
								glTexCoord2f(0.0, 1.0); glVertex3f(-50.0, 50.0, 0.0);
							glEnd();
						glPopMatrix();
					glPopMatrix();
					glMatrixMode(GL_PROJECTION);
				glPopMatrix();

			glPopAttrib();
			break;
			}

		case GameConnect: {
			glPushAttrib(GL_ENABLE_BIT);
	
			glDisable(GL_LIGHTING);
			glEnable(GL_TEXTURE_2D);
			glEnable(GL_BLEND); 

			glMatrixMode(GL_PROJECTION);
				glPushMatrix();
					glLoadIdentity();
					glOrtho(0.0, 1280.0, 0.0, 720.0, -100.0, 100.0);
		
					glMatrixMode(GL_MODELVIEW);
					glPushMatrix();
						glLoadIdentity();

						glPushMatrix();
							glBindTexture(GL_TEXTURE_2D, titleTex);
							glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
							glColor3f(1.0, 1.0, 1.0);
							glBegin(GL_QUADS);
								glTexCoord2f(0.0, 0.0); glVertex3f(390.0, 110.0, 0.0);
								glTexCoord2f(1.0, 0.0); glVertex3f(890.0, 110.0, 0.0);
								glTexCoord2f(1.0, 1.0); glVertex3f(890.0, 610.0, 0.0);
								glTexCoord2f(0.0, 1.0); glVertex3f(390.0, 610.0, 0.0);
							glEnd();
						glPopMatrix();

						glPushMatrix();
							glBindTexture(GL_TEXTURE_2D, connectingTex);
							glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
							glColor3f(1.0, 1.0, 1.0);
							static GLuint loadCount = 0;
							if(loadCount < 15) {
								glBegin(GL_QUADS);
									glTexCoord2f(0.0, 0.0); glVertex3f(640.0, 200.0, 1.0);
									glTexCoord2f(1.0, 0.0); glVertex3f(940.0, 200.0, 1.0);
									glTexCoord2f(1.0, 1.0); glVertex3f(940.0, 300.0, 1.0);
									glTexCoord2f(0.0, 1.0); glVertex3f(640.0, 300.0, 1.0);
								glEnd();
							}
							loadCount = (loadCount + 1) % 30;
						glPopMatrix();
							
						static GLuint arrowloc = 0;
						glPushMatrix();
							glTranslatef(1160.0 + arrowloc, 115.0, 0.0);
							glBindTexture(GL_TEXTURE_2D, connectArrowTex);
							glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
							glColor3f(1.0, 1.0, 1.0);
							glBegin(GL_QUADS);
								glTexCoord2f(0.0, 0.0); glVertex3f(-50.0, -50.0, 0.0);
								glTexCoord2f(1.0, 0.0); glVertex3f(50.0, -50.0, 0.0);
								glTexCoord2f(1.0, 1.0); glVertex3f(50.0, 50.0, 0.0);
								glTexCoord2f(0.0, 1.0); glVertex3f(-50.0, 50.0, 0.0);
							glEnd();
						glPopMatrix();

						glPushMatrix();
							glTranslatef(1200.0 - arrowloc, 65.0, 0.0);
							glRotatef(180.0, 0.0, 0.0, 1.0);
							glBindTexture(GL_TEXTURE_2D, connectArrowTex);
							glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
							glColor3f(1.0, 1.0, 1.0);
							glBegin(GL_QUADS);
								glTexCoord2f(0.0, 0.0); glVertex3f(-50.0, -50.0, 1.0);
								glTexCoord2f(1.0, 0.0); glVertex3f(50.0, -50.0, 1.0);
								glTexCoord2f(1.0, 1.0); glVertex3f(50.0, 50.0, 1.0);
								glTexCoord2f(0.0, 1.0); glVertex3f(-50.0, 50.0, 1.0);
							glEnd();
						glPopMatrix();
						arrowloc = (arrowloc + 2) % 40;
					glPopMatrix();
					glMatrixMode(GL_PROJECTION);
				glPopMatrix();

			glPopAttrib();
			break;
			}

		case GamePlay: {
			
			//static thread SocketThread(SetSocketClientThread);
			
			if(mouseLock)
				glutWarpPointer(glutGet(GLUT_WINDOW_WIDTH) / 2, glutGet(GLUT_WINDOW_HEIGHT) / 2);

			camera->LookAt();

			glPushMatrix();	//Locate Light0
				glLightfv(GL_LIGHT0, GL_POSITION, LightPos); 
			glPopMatrix();

			GLfloat spotPos[] = { camera->GetEye().x, camera->GetEye().y, camera->GetEye().z, 1.0 };
			glPushMatrix();
				glPushMatrix();	//Rotate spotlight
					glRotatef(camera->GetAngleY(), 0.0, 1.0, 0.0);
					glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spotDir);
				glPopMatrix();
				glPushMatrix();	//Move spotlight
					glLightfv(GL_LIGHT1, GL_POSITION, spotPos);
				glPopMatrix();
			glPopMatrix();

			sceneMap->ShowMap();
			
			server->getSelectCharacter()->OnDisplay();
			server->getSelectCharacter()->OnTimer();

			client->getSelectCharacter()->OnDisplay();
			client->getSelectCharacter()->OnTimer();
			
			server->getSelectCharacter()->OnDisplayHeadLife(camera->GetAngleY());

			client->getSelectCharacter()->OnDisplayLife();
			
			glMatrixMode(GL_PROJECTION);	//Orthographic projection to render the aerial map
			glPushMatrix();
				glLoadIdentity();
				if(glutGet(GLUT_WINDOW_WIDTH) >= glutGet(GLUT_WINDOW_HEIGHT))
					glOrtho((-600 * 5) + 300, 300, -300.0, (600 * 5) * glutGet(GLUT_WINDOW_HEIGHT) / glutGet(GLUT_WINDOW_WIDTH) - 300, -300.0, 300);
				else
					glOrtho(-600 * 5 * glutGet(GLUT_WINDOW_WIDTH) / glutGet(GLUT_WINDOW_HEIGHT) + 300, 300, -300.0, (600 * 5) - 300, -300.0, 300);
		
				glMatrixMode(GL_MODELVIEW);
				glPushMatrix();
					glLoadIdentity();	//Render the aerial map
					sceneMap->ShowAerialMap();
					sceneMap->SetMonAerialLoc(*server->getSelectCharacter()->GetLocation(), server->getSelectCharacter()->GetLocAngle() +server->getSelectCharacter()->GetmoveAngle());	//Render monster's location on aerial map
					sceneMap->SetMasAerialLoc(*client->getSelectCharacter()->GetLocation(), client->getSelectCharacter()->GetLocAngle() + client->getSelectCharacter()->GetmoveAngle());	//Render character's location on aerial map
				glPopMatrix();
				glMatrixMode(GL_PROJECTION);
			glPopMatrix();

			if(client->getSelectCharacter()->GetDeath()) {
				gameState = GameDefeat;
				while(ShowCursor(TRUE) < 0)
					ShowCursor(TRUE);
				playWAV(".\\res\\Defeat.wav", false);
				break;
			}
			if(server->getSelectCharacter()->GetDeath()) {
				gameState = GameVictory;
				while(ShowCursor(TRUE) < 0)
					ShowCursor(TRUE);
				playWAV(".\\res\\Victory.wav", false);
				break;
			}
			break;
			}
			case GameVictory:
			glPushMatrix();	//Locate Light0
				glLightfv(GL_LIGHT0, GL_POSITION, LightPos); 
			glPopMatrix();

			sceneMap->ShowMap();
			
			server->getSelectCharacter()->OnDisplay();
			server->getSelectCharacter()->OnTimer();

			client->getSelectCharacter()->OnDisplay();
			client->getSelectCharacter()->OnTimer();
			
			server->getSelectCharacter()->OnDisplayHeadLife(camera->GetAngleY());

			camera->LookAt();

			client->getSelectCharacter()->OnDisplayLife();
			

			glMatrixMode(GL_MODELVIEW);
			glPushAttrib(GL_ENABLE_BIT);
	
			glDisable(GL_LIGHTING);
			glEnable(GL_TEXTURE_2D);
			glEnable(GL_BLEND); 

			glMatrixMode(GL_PROJECTION);
				glPushMatrix();
					glLoadIdentity();
						glOrtho(0, 1280, 0.0, 720, -1, 1);
		
					glMatrixMode(GL_MODELVIEW);
					glPushMatrix();
						glLoadIdentity();

						glPushMatrix();
							glBindTexture(GL_TEXTURE_2D, vicTex);
							glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
							glColor3f(1.0, 1.0, 1.0);
							glBegin(GL_QUADS);
								glTexCoord2f(0.0, 0.0); glVertex3f(190.0, 135.0, 0.0);
								glTexCoord2f(1.0, 0.0); glVertex3f(1090.0, 135.0, 0.0);
								glTexCoord2f(1.0, 1.0); glVertex3f(1090.0, 585.0, 0.0);
								glTexCoord2f(0.0, 1.0); glVertex3f(190.0, 585.0, 0.0);
							glEnd();
						glPopMatrix();
					glPopMatrix();
					glMatrixMode(GL_PROJECTION);
				glPopMatrix();

			glPopAttrib();
			
			break;

		case GameDefeat:
			glPushMatrix();	//Locate Light0
				glLightfv(GL_LIGHT0, GL_POSITION, LightPos); 
			glPopMatrix();

			sceneMap->ShowMap();
			
			server->getSelectCharacter()->OnDisplay();
			server->getSelectCharacter()->OnTimer();

			client->getSelectCharacter()->OnDisplay();
			client->getSelectCharacter()->OnTimer();
			
			server->getSelectCharacter()->OnDisplayHeadLife(camera->GetAngleY());

			camera->LookAt();

			client->getSelectCharacter()->OnDisplayLife();
			
			glMatrixMode(GL_MODELVIEW);
			glPushAttrib(GL_ENABLE_BIT);
	
			glDisable(GL_LIGHTING);
			glEnable(GL_TEXTURE_2D);
			glEnable(GL_BLEND); 

			glMatrixMode(GL_PROJECTION);
				glPushMatrix();
					glLoadIdentity();
						glOrtho(0, 1280, 0.0, 720, -1, 1);
		
					glMatrixMode(GL_MODELVIEW);
					glPushMatrix();
						glLoadIdentity();

						glPushMatrix();
							glBindTexture(GL_TEXTURE_2D, defTex);
							glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
							glColor3f(1.0, 1.0, 1.0);
							glBegin(GL_QUADS);
								glTexCoord2f(0.0, 0.0); glVertex3f(190.0, 60.0, 0.0);
								glTexCoord2f(1.0, 0.0); glVertex3f(1090.0, 60.0, 0.0);
								glTexCoord2f(1.0, 1.0); glVertex3f(1090.0, 660.0, 0.0);
								glTexCoord2f(0.0, 1.0); glVertex3f(190.0, 660.0, 0.0);
							glEnd();
						glPopMatrix();
					glPopMatrix();
					glMatrixMode(GL_PROJECTION);
				glPopMatrix();

			glPopAttrib();
			break;
		}

		glutSwapBuffers();
	}


	//OnReshap event
	GLvoid CFinalProject::OnReshape(int width, int height)
	{
		glViewport(0, 0, width, height);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(60.0, (GLfloat) width/(GLfloat) height, 1, 10000.0);
	}


	//OnMouse event
	GLvoid CFinalProject::OnMouseFunc(int button, int state, int x, int y) 
	{
		switch(gameState) {
		case GameStart:
			break;

		case GameLoading:
			break;

		case GamePlay:
			if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
			{
				client->getSelectCharacter()->Attack();
			}
			else if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
			{
				client->getSelectCharacter()->Avoid();
			}
			else if(button == GLUT_WHEEL_UP)
			{
				camera->EyeFront(10);
			}
			else if(button == GLUT_WHEEL_DOWN)
			{
				camera->EyeBack(10);
			}
			break;
		}
	}


	void  CFinalProject::OnMousePassiveMotion(int x, int y)
	{
		switch(gameState) {
		case GameStart:
			break;

		case GameLoading:
			break;

		case GamePlay:
			if(mouseLock) {
				camera->TurnAround((glutGet(GLUT_WINDOW_WIDTH) / 2 - (float)x) / 5.0);
				camera->TurnUpDown((glutGet(GLUT_WINDOW_HEIGHT) / 2 - (float)y) / 5.0);
   				if(client->getSelectCharacter()->GetState() > Akali_Idle1) {
					client->getSelectCharacter()->SetLocAngle(camera->GetAngleY());
				}
			}
			break;
		}
	}


	//OnKeyboard event
	GLvoid CFinalProject::OnKeyboardFunc(unsigned char key, int x, int y) 
	{
		switch(gameState) {
		case GameStart:
			switch(key)
			{
			case 13:	//Key Enter
				charSelect = SelectAkali;
				playWAV(".\\res\\Characters\\Akali\\Akali.wav", false);
				gameState = GameSelect;
				break;

			case 'f': case 'F':
				screenMode = !screenMode;
				switch(screenMode) {
				case GL_TRUE:
					glutFullScreen();
					break;

				case GL_FALSE:
					glutPositionWindow(100, 100);
					glutReshapeWindow(1280, 720);
					break;
				}
				break;
			}
			break;

		case GameSelect:
			switch(key)
			{
			case 'd': case'D':
				charSelect = (charSelect + 1) % CharacterNum;
				switch(charSelect) {
				case SelectAkali:
					playWAV(".\\res\\Characters\\Akali\\Akali.wav", false);
					break;
				case SelectAlistar:
					playWAV(".\\res\\Characters\\Alistar\\Alistar.wav", false);
					break;
				case SelectShen:
					playWAV(".\\res\\Characters\\Shen\\Shen.wav", false);
					break;
				case SelectDiana:
					playWAV(".\\res\\Characters\\Diana\\Diana.wav", false);
					break;
				case SelectLeona:
					playWAV(".\\res\\Characters\\Leona\\Leona.wav", false);
					break;
				case SelectMalphite:
					playWAV(".\\res\\Characters\\Malphite\\Malphite.wav", false);
					break;
				}
				break;

			case 'a': case'A':
				if((charSelect - 1) < 0)
					charSelect += 6;
				charSelect = (charSelect - 1) % CharacterNum;
				switch(charSelect) {
				case SelectAkali:
					playWAV(".\\res\\Characters\\Akali\\Akali.wav", false);
					break;
				case SelectAlistar:
					playWAV(".\\res\\Characters\\Alistar\\Alistar.wav", false);
					break;
				case SelectShen:
					playWAV(".\\res\\Characters\\Shen\\Shen.wav", false);
					break;
				case SelectDiana:
					playWAV(".\\res\\Characters\\Diana\\Diana.wav", false);
					break;
				case SelectLeona:
					playWAV(".\\res\\Characters\\Leona\\Leona.wav", false);
					break;
				case SelectMalphite:
					playWAV(".\\res\\Characters\\Malphite\\Malphite.wav", false);
					break;
				}
				break;

			case 13:	//Key Enter
				gameState = GameLoading;
				//static thread loadThread(SetLoadThread);
				loadThread = thread(SetLoadThread);
				break;

			case 0x1B:	//Key Esc
				gameState = GameStart;
				playWAV(".\\res\\LoL - Client Login BGM (Season 2 World Championships).wav", true);
				break;

			case 'f': case 'F':
				screenMode = !screenMode;
				switch(screenMode) {
				case GL_TRUE:
					glutFullScreen();
					break;

				case GL_FALSE:
					glutPositionWindow(100, 100);
					glutReshapeWindow(1280, 720);
					break;
				}
				break;
			}
			break;

		case GameLoading:
			switch(key) { 
			case 'f': case 'F': case 0x1B:
				screenMode = !screenMode;
				switch(screenMode) {
				case GL_TRUE:
					glutFullScreen();
					break;

				case GL_FALSE:
					glutPositionWindow(100, 100);
					glutReshapeWindow(1280, 720);
					break;
				}
				break;
			}
			break;

		case GamePlay:
			switch(key) 
			{
			case 'w':case 'W':
				client->getSelectCharacter()->MoveFront(GL_TRUE);
				break;

			case 'a':case 'A':
				client->getSelectCharacter()->MoveLeft(GL_TRUE);
				break;

			case 's':case 'S':
				client->getSelectCharacter()->MoveBack(GL_TRUE);
				break;

			case 'd':case 'D':
				client->getSelectCharacter()->MoveRight(GL_TRUE);
				break;

			case 'g':
			case 'G':	//Switch fog mode
				fogMode = !fogMode;
				switch(fogMode) {
				case GL_TRUE:
					glEnable(GL_FOG);
					break;

				case GL_FALSE:
					glDisable(GL_FOG);
					break;
				}
				break;

			case 'l':
			case 'L': {
				spotLight = !spotLight;
				switch(spotLight) {
				case GL_TRUE:
					glEnable(GL_LIGHT1);
					break;

				case GL_FALSE:
					glDisable(GL_LIGHT1);
					break;
				}
				break;
					  }

			case 't':
			case 'T':	//Switch fog mode
				mouseLock = !mouseLock;
				switch(mouseLock) {
				case GL_TRUE:
					while(ShowCursor(FALSE) >= 0)
						ShowCursor(FALSE);
					break;

				case GL_FALSE:
					while(ShowCursor(TRUE) < 0)
						ShowCursor(TRUE);
					break;
				}
				break;

			case ' ':
				while(ShowCursor(FALSE) >= 0)
					ShowCursor(FALSE);
				mouseLock = GL_TRUE;
				break;

			case 'f': case 'F': case 0x1B:
				screenMode = !screenMode;
				switch(screenMode) {
				case GL_TRUE:
					glutFullScreen();
					break;

				case GL_FALSE:
					glutPositionWindow(100, 100);
					glutReshapeWindow(1280, 720);
					break;
				}
				break;
			}
			break;
		case GameVictory:
			switch(key) {
			case 'f': case 'F': case 0x1B:
				screenMode = !screenMode;
				switch(screenMode) {
				case GL_TRUE:
					glutFullScreen();
					break;

				case GL_FALSE:
					glutPositionWindow(100, 100);
					glutReshapeWindow(1280, 720);
					break;
				}
				break;
			}
			break;
		case GameDefeat:
			switch(key) {
			case 'f': case 'F': case 0x1B:
				screenMode = !screenMode;
				switch(screenMode) {
				case GL_TRUE:
					glutFullScreen();
					break;

				case GL_FALSE:
					glutPositionWindow(100, 100);
					glutReshapeWindow(1280, 720);
					break;
				}
				break;
			}
			break;
		}
	}


	//OnKeyboardUp event
	GLvoid CFinalProject::OnKeyboardUpFunc(unsigned char key, int x, int y) 
	{
		switch(gameState) {
		case GameStart:
			break;

		case GameLoading:
			break;

		case GamePlay:
			switch(key)
			{
			case 'w':case 'W':
				client->getSelectCharacter()->MoveFront(GL_FALSE);
				break;
			case 'a':case 'A':
				client->getSelectCharacter()->MoveLeft(GL_FALSE);
				break;
			case 's':case 'S':
				client->getSelectCharacter()->MoveBack(GL_FALSE);
				break;
			case 'd':case 'D':
				client->getSelectCharacter()->MoveRight(GL_FALSE);
				break;
			case ' ':
				//client->getSelectCharacter()->SetLocAngle(camera->GetAngleY());
				while(ShowCursor(TRUE) < 0)
					ShowCursor(TRUE);
				mouseLock = GL_FALSE;
				break;
			}
			break;
		}
	}


	GLvoid CFinalProject::LoadThreadFunc(GLvoid)
	{
		CreateWindowEx(NULL, NULL, NULL, NULL, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, GetModuleHandle(NULL), NULL);
		wglMakeCurrent(mainDC, subRC);

		playWAV(".\\res\\LoL_World_Championship_music.wav", true);
		
		client=new Client(charSelect);
		client->getSelectCharacter()->init_Character();
		client->getSelectCharacter()->SetLocation(0.0,0.0,-250.0);
		client->getSelectCharacter()->SetLocAngle(270.0);

		if(exitGame)
			return;

		sceneMap = new CSceneMap();
		sceneMap->LoadAerialMap();
		
		client->getSelectCharacter()->SetScene(sceneMap);
		
		camera = new CCamera();
		camera->SetCamera(0.0 , 60.0, -430.0, 0.0, 50.0, -250.0, 0.0, 1.0, 0.0);
		camera->SetFollow(client->getSelectCharacter()->GetLocation());
		
		gameState = GameConnect;

		//等待兩端loading完第一隻
			while(!socket_client->connectServer()) {
				;
			}

			gameState = GameConnect;

			socket_client->sendCharacter(charSelect);
			socket_client->receiveCharacter();

			server = new Server(socket_client->character);
			server->getSelectCharacter()->init_Character();
			server->getSelectCharacter()->SetLocation(0.0,0.0,250.0);
			server->getSelectCharacter()->SetScene(sceneMap);

			if(exitGame)
				return;

			//等待兩端loading完第二隻
			socket_client->getReady();
			SocketThread = thread(SetSocketClientThread);
			gameState = GamePlay;
			playWAV(".\\res\\SAOBGM.wav", true);
	}

	

	GLvoid CFinalProject::SocketClientThreadFunc(GLvoid){
		while(!exitGame){
			
			socket_client->receiveCoordinate();
			server->getSelectCharacter()->SetLocation(socket_client->x,socket_client->y,socket_client->z);
			server->getSelectCharacter()->SetState(socket_client->state);
			server->getSelectCharacter()->setControl_Mode(false);
			server->getSelectCharacter()->SetLocAngle(socket_client->locAngle);
			server->getSelectCharacter()->SetmoveAngle(socket_client->moveAngle);
			client->getSelectCharacter()->SetHealth(socket_client->health);		
			client->getSelectCharacter()->SetDeath(socket_client->death);

		    socket_client->sendCoordinate(client->getSelectCharacter()->GetLocation()->x
				,client->getSelectCharacter()->GetLocation()->y
				,client->getSelectCharacter()->GetLocation()->z
				,client->getSelectCharacter()->GetState()
				,client->getSelectCharacter()->GetLocAngle()
				,client->getSelectCharacter()->GetmoveAngle()
				,client->getSelectCharacter()->GetMod()->GetCurrentFrame()
				,server->getSelectCharacter()->GetHealth()
				,server->getSelectCharacter()->GetDeath());
		}
	}

	//Set close event
	GLvoid CFinalProject::SetClose(GLvoid)
	{
		instance->OnClose();
	}


	//Set timer event
	GLvoid CFinalProject::SetTimer(int id) 
	{
		instance->OnTimer(id);
	}


	//Set display event
	GLvoid CFinalProject::SetDisplay(GLvoid) 
	{
		instance->OnDisplay(); 
	}


	//Set reshape event
	GLvoid CFinalProject::SetReshape(int width, int height) 
	{
		instance->OnReshape(width, height);
	}


	//Set mouse event
	GLvoid CFinalProject::SetMouseFunc(int button, int state, int x, int y)
	{
		instance->OnMouseFunc(button, state, x, y);
	}


	GLvoid CFinalProject::SetMousePassiveMotion(int x, int y)
	{
		instance->OnMousePassiveMotion(x, y);
	}


	//Set keyboard event
	GLvoid CFinalProject::SetKeyboardFunc(unsigned char key, int x, int y) 
	{
		instance->OnKeyboardFunc(key, x, y);
	}


	//Set keyboardUp event
	GLvoid CFinalProject::SetKeyboardUpFunc(unsigned char key, int x, int y) 
	{
		instance->OnKeyboardUpFunc(key, x, y);
	}


	GLvoid CFinalProject::SetLoadThread() 
	{
		instance->LoadThreadFunc();
	}



	GLvoid CFinalProject::SetSocketClientThread() 
	{
		instance->SocketClientThreadFunc();
	}
	
	GLvoid CFinalProject::playWAV(char *path, bool loop){
		
		WCHAR wsz[64];
		swprintf(wsz, L"%S", path);
		LPCWSTR p = wsz;
		if(loop)
			PlaySound(p, NULL, SND_ASYNC|SND_LOOP);
		else
			PlaySound(p, NULL, SND_ASYNC);
	}
	

}
