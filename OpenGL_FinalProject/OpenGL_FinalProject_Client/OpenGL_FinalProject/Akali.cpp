/***************************
4099056008 陳昱全 期末Project 6/18
4099056030 黃湋縜 期末Project 6/18
***************************/
#include "stdafx.h"
#include "Akali.h"
#define _USE_MATH_DEFINES
#include <cmath>


CAkali::CAkali(GLvoid): m_speed(5.0), locAngle(90.0), moveAngle(0.0), enableAngle(GL_TRUE), m_Avoid(0.0), sceneData(nullptr),control_mode(GL_TRUE), health(MAXHEALTH), death(GL_FALSE)
{
	MoveReset();
	location = new CVector();

	LoadTexture(&lifeTex, ".\\res\\Characters\\ico_health.png");
	
	for(int i = 0; i < Akali_Animation; i++) {
		switch(i) {
		case Akali_Idle1:
			akali[Akali_Idle1] = new CFbxMod();
			akali[Akali_Idle1]->LoadMod(".\\res\\Characters\\Akali\\Akali_Idle1.fbx");
			akali[Akali_Idle1]->EnableDisplayList();
			break;

		case Akali_Run:
			akali[Akali_Run] = new CFbxMod();
			akali[Akali_Run]->LoadMod(".\\res\\Characters\\Akali\\Akali_Run.fbx");
			akali[Akali_Run]->EnableDisplayList();
			break;

		case Akali_Channel_Windup:
			akali[Akali_Channel_Windup] = new CFbxMod();
			akali[Akali_Channel_Windup]->LoadMod(".\\res\\Characters\\Akali\\Akali_Channel_Windup.fbx");
			akali[Akali_Channel_Windup]->EnableDisplayList();
			break;

		case Akali_Attack1:
			akali[Akali_Attack1] = new CFbxMod();
			akali[Akali_Attack1]->LoadMod(".\\res\\Characters\\Akali\\Akali_Attack1.fbx");
			akali[Akali_Attack1]->EnableDisplayList();
			akali[Akali_Attack1]->SetAnimationRate(0.5);
			break;

		case Akali_Attack2:
			akali[Akali_Attack2] = new CFbxMod();
			akali[Akali_Attack2]->LoadMod(".\\res\\Characters\\Akali\\Akali_Attack2.fbx");
			akali[Akali_Attack2]->EnableDisplayList();
			akali[Akali_Attack2]->SetAnimationRate(0.5);
			break;

		case Akali_Crit:
			akali[Akali_Crit] = new CFbxMod();
			akali[Akali_Crit]->LoadMod(".\\res\\Characters\\Akali\\Akali_Crit.fbx");
			akali[Akali_Crit]->EnableDisplayList();
			akali[Akali_Crit]->SetAnimationRate(0.5);
			break;

		case Akali_Death:
			akali[Akali_Death] = new CFbxMod();
			akali[Akali_Death]->LoadMod(".\\res\\Characters\\Akali\\Akali_Death.fbx");
			akali[Akali_Death]->EnableDisplayList();
			akali[Akali_Death]->SetDisplayOnce(GL_TRUE);
			break;
		}
	}
	
	for(int i = 0; i < AkaliAttackNum; i++) {
		switch(i) {
		case AkaliAttack1:
			attackData[i].AttackPower = 5.0;
			break;
		case AkaliAttack2:
			attackData[i].AttackPower = 7.0;
			break;
		case AkaliAttackCrit:
			attackData[i].AttackPower = 10.0;
			break;
		}
		attackData[i].AttackTiming = GL_FALSE;
		attackData[i].Attacking = GL_FALSE;
	}

	state = Akali_Idle1;
}


CAkali::~CAkali(GLvoid)
{
	delete location;

	for(int i = 0; i < Akali_Animation; i++)
		delete akali[i];
}


//Load texture data to memory
GLboolean CAkali::LoadTexture(GLuint *id, char *path)
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



GLvoid CAkali::OnDisplay(GLvoid)
{
	glPushMatrix();
		glTranslatef(location->x, location->y, location->z);
		glRotatef(locAngle + 90.0, 0.0, 1.0, 0.0);
		glRotatef(moveAngle, 0.0, 1.0, 0.0);
		glScalef(0.5, 0.5, 0.5);
		akali[state]->OnDisplay();
	glPopMatrix();
}


//Rendering the character's life by OpenGL function
GLvoid CAkali::OnDisplayLife(GLvoid)
{
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
					glBindTexture(GL_TEXTURE_2D, lifeTex);
					glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
					glColor3f(1.0, 1.0, 1.0);
					glBegin(GL_QUADS);
						glTexCoord2f(0.0, 0.0); glVertex3f(50.0, 640.0, 0.0);
						glTexCoord2f(1.0, 0.0); glVertex3f(120.0, 640.0, 0.0);
						glTexCoord2f(1.0, 1.0); glVertex3f(120.0, 710.0, 0.0);
						glTexCoord2f(0.0, 1.0); glVertex3f(50.0, 710.0, 0.0);
					glEnd();
				glPopMatrix();

				glPushMatrix();
				glDisable(GL_TEXTURE_2D);
					glColor3f(0.0, 1.0, 0.0);
					glBegin(GL_QUADS);
						glVertex3f(160.0, 660.0, 0.0);
						glVertex3f(health + 160.0, 660.0, 0.0);
						glVertex3f(health + 160.0, 690.0, 0.0);
						glVertex3f(160.0, 690.0, 0.0);
					glEnd();
				glPopMatrix();
			glPopMatrix();
			glMatrixMode(GL_PROJECTION);
		glPopMatrix();

	glPopAttrib();
}


//Rendering the monster's life by OpenGL function
GLvoid CAkali::OnDisplayHeadLife(GLfloat lookAngle)
{
	if(!death) {
		glPushMatrix();
			glTranslatef(location->x, location->y + 20, location->z);

			if(state < Akali_Death) {
				glPushAttrib(GL_ENABLE_BIT);
				glDisable(GL_LIGHTING);
				glPushMatrix();
					glRotatef(lookAngle + 90, 0.0, 1.0, 0.0);
					glTranslatef(0.0, 50.0, 0.0);
					glScalef(health / MAXHEALTH, 1.0, 1.0);
					glScalef(15.0, 1.0, 1.0);
					glColor3f(1.0, 0.0, 0.0);
					glutSolidCube(4.0);	//Render life point
					glColor3f(1.0, 1.0, 1.0);
				glPopMatrix();
				glPopAttrib();
			}
		glPopMatrix();
	}
}


GLvoid CAkali::OnTimer(GLvoid)
{
	akali[state]->OnTimerClick();

	switch(state)
	{
	case Akali_Idle1:
		if(GetMoveState() && !((m_front && m_back) || (m_left && m_right)))
			state = Akali_Run;
		break;

	case Akali_Run:
		if(!GetMoveState() && control_mode)
			state = Akali_Idle1;
		else
			Movement(m_speed);
		break;

	case Akali_Attack1:
		if((akali[state]->GetCurrentFrame()) > (akali[state]->GetAnimationTime() / 5 * 3)) {
			attackData[AkaliAttack1].AttackTiming = GL_TRUE;
		}
		if(akali[state]->AnimationDone()) {
			state = Akali_Idle1;
			enableAngle = GL_TRUE;
			attackData[AkaliAttack1].AttackTiming = GL_FALSE;
			attackData[AkaliAttack1].Attacking = GL_FALSE;
		}
		break;

	case Akali_Attack2:
		if((akali[state]->GetCurrentFrame()) > (akali[state]->GetAnimationTime() / 5 * 3)) {
			attackData[AkaliAttack2].AttackTiming = GL_TRUE;
		}
		if(akali[state]->AnimationDone()) {
			state = Akali_Idle1;
			enableAngle = GL_TRUE;
			attackData[AkaliAttack2].AttackTiming = GL_FALSE;
			attackData[AkaliAttack2].Attacking = GL_FALSE;
		}
		break;

	case Akali_Crit:
		if((akali[state]->GetCurrentFrame()) > (akali[state]->GetAnimationTime() / 5 * 3)) {
			attackData[AkaliAttackCrit].AttackTiming = GL_TRUE;
		}
		if(akali[state]->AnimationDone()) {
			state = Akali_Idle1;
			enableAngle = GL_TRUE;
			attackData[AkaliAttackCrit].AttackTiming = GL_FALSE;
			attackData[AkaliAttackCrit].Attacking = GL_FALSE;
		}
		break;

	case Akali_Channel_Windup:
		if(m_Avoid < 5) {
			m_Avoid++;
			Movement(40.0);
		}
		else {
			state = Akali_Run;
			MoveReset();
		}
		break;

	case Akali_Death:
		if(akali[state]->AnimationDone())
			death = GL_TRUE;
		break;

	}
}

//Attacked event
GLvoid CAkali::OnAttacked(GLfloat damage)
{
	health -= damage;
	if(health < 0.0) { //No life
		health = 0.0;
		for(int i = 0; i < AkaliAttackNum; i++)
			attackData[i].AttackTiming = GL_FALSE;
		MoveReset();
		state = Akali_Death;
	}
}


GLvoid CAkali::Movement(GLfloat speed)
{
	if(m_front && m_back) {
		moveAngle = 0.0;
		state = Akali_Idle1;
		return;
	}

	if(m_left && m_right) {
		moveAngle = 0.0;
		state = Akali_Idle1;
		return;
	}

	if(m_front) {
		location->SetVector(location->x + (float)cos(M_PI * locAngle / 180.0) * speed, 
							location->y, 
							location->z - (float)sin(M_PI * locAngle / 180.0) * speed);
		if(sceneData != nullptr)	//Collision detection
		if(sceneData->CollisionTest(location->x, location->y, location->z)) {
			location->SetVector(location->x - (float)cos(M_PI * locAngle / 180.0) * speed, 
							location->y, 
							location->z + (float)sin(M_PI * locAngle / 180.0) * speed); 
		}
		moveAngle = 0.0;
	}

	if(m_back) {
		location->SetVector(location->x - (float)cos(M_PI * locAngle / 180.0) * speed, 
							location->y, 
							location->z + (float)sin(M_PI * locAngle / 180.0) * speed); 
		if(sceneData != nullptr)	//Collision detection
		if(sceneData->CollisionTest(location->x, location->y, location->z)) {
			location->SetVector(location->x + (float)cos(M_PI * locAngle / 180.0) * speed, 
							location->y, 
							location->z - (float)sin(M_PI * locAngle / 180.0) * speed);
		}
		moveAngle = 180.0;
	}

	if(m_left) {
		location->SetVector(location->x - (float)sin(M_PI * locAngle / 180.0) * speed, 
							location->y, 
							location->z - (float)cos(M_PI * locAngle / 180.0) * speed); 
		if(sceneData != nullptr)	//Collision detection
		if(sceneData->CollisionTest(location->x, location->y, location->z)) {
			location->SetVector(location->x + (float)sin(M_PI * locAngle / 180.0) * speed, 
							location->y, 
							location->z + (float)cos(M_PI * locAngle / 180.0) * speed);
		}
		moveAngle = 90;
	}

	if(m_right) {
		location->SetVector(location->x + (float)sin(M_PI * locAngle / 180.0) * speed, 
							location->y, 
							location->z + (float)cos(M_PI * locAngle / 180.0) * speed); 
		if(sceneData != nullptr)	//Collision detection
		if(sceneData->CollisionTest(location->x, location->y, location->z)) {
			location->SetVector(location->x - (float)sin(M_PI * locAngle / 180.0) * speed, 
							location->y, 
							location->z - (float)cos(M_PI * locAngle / 180.0) * speed);
		}
		moveAngle = -90;
	}

	if(m_front && m_left)
		moveAngle = 45.0;

	if(m_front && m_right)
		moveAngle = -45.0;

	if(m_left && m_back)
		moveAngle = 135.0;

	if(m_right && m_back)
		moveAngle = -135.0;
}


GLvoid CAkali::MoveFront(GLboolean m_enable)
{
	if(m_front != m_enable && state <= Akali_Run)
		switch(m_enable)
		{
		case GL_TRUE:
			m_front = GL_TRUE;
			break;

		case GL_FALSE:
			m_front = GL_FALSE;
			break;
		}
	else
		return;
}


GLvoid CAkali::MoveBack(GLboolean m_enable)
{
	if(m_back != m_enable && state <= Akali_Run)
		switch(m_enable)
		{
		case GL_TRUE:
			m_back = GL_TRUE;
			break;

		case GL_FALSE:
			m_back = GL_FALSE;
			break;
		}
	else
		return;
}


GLvoid CAkali::MoveLeft(GLboolean m_enable)
{
	if(m_left != m_enable && state <= Akali_Run)
		switch(m_enable)
		{
		case GL_TRUE:
			m_left = GL_TRUE;
			break;

		case GL_FALSE:
			m_left = GL_FALSE;
			break;
		}
	else
		return;
}


GLvoid CAkali::MoveRight(GLboolean m_enable)
{
	if(m_right != m_enable && state <= Akali_Run)
		switch(m_enable)
		{
		case GL_TRUE:
			m_right = GL_TRUE;
			break;

		case GL_FALSE:
			m_right = GL_FALSE;
			break;
		}
	else
		return;
}


GLvoid CAkali::Avoid(GLvoid)
{
	if(state <= Akali_Channel_Windup && GetMoveState()) {
		m_Avoid = 0.0;
		state = Akali_Channel_Windup;
	}
}


GLvoid CAkali::Attack(GLvoid)
{
	if(state < Akali_Attack1) {
		enableAngle = GL_FALSE;
		MoveReset();
		state = Akali_Attack1;
		attackData[AkaliAttack1].Attacking = GL_TRUE;
		attackData[AkaliAttack1].AttackTiming = GL_FALSE;
	}
	else if(state == Akali_Attack1) {
		if((akali[state]->GetCurrentFrame()) > (akali[state]->GetAnimationTime() / 5 * 4)) {
			akali[Akali_Attack1]->ResetCurrentTime();
			state = Akali_Attack2;
			attackData[AkaliAttack2].Attacking = GL_TRUE;
			attackData[AkaliAttack2].AttackTiming = GL_FALSE;
		}
	}
	else if(state == Akali_Attack2) {
		if((akali[state]->GetCurrentFrame()) > (akali[state]->GetAnimationTime() / 5 * 4)) {
			akali[Akali_Attack2]->ResetCurrentTime();
			state = Akali_Crit;
			attackData[AkaliAttackCrit].Attacking = GL_TRUE;
			attackData[AkaliAttackCrit].AttackTiming = GL_FALSE;
		}
	}

}










