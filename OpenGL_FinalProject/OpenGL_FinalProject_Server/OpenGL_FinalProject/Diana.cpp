/***************************
4099056008 陳昱全 期末Project 6/18
4099056030 黃湋縜 期末Project 6/18
***************************/
#include "stdafx.h"
#include "Diana.h"
#define _USE_MATH_DEFINES
#include <cmath>


CDiana::CDiana(GLvoid): m_speed(5.0), locAngle(90.0), moveAngle(0.0), enableAngle(GL_TRUE), m_Avoid(0.0), sceneData(nullptr), health(MAXHEALTH), death(GL_FALSE)
{
	MoveReset();
	location = new CVector();

	LoadTexture(&lifeTex, ".\\res\\Characters\\ico_health.png");

	for(int i = 0; i < Diana_Animation; i++) {
		switch(i) {
		case Diana_Idle1:
			diana[Diana_Idle1] = new CFbxMod();
			diana[Diana_Idle1]->LoadMod(".\\res\\Characters\\Diana\\Diana_Idle1.fbx");
			diana[Diana_Idle1]->EnableDisplayList();
			break;

		case Diana_Run:
			diana[Diana_Run] = new CFbxMod();
			diana[Diana_Run]->LoadMod(".\\res\\Characters\\Diana\\Diana_Run.fbx");
			diana[Diana_Run]->EnableDisplayList();
			break;

		case Diana_Haunt:
			diana[Diana_Haunt] = new CFbxMod();
			diana[Diana_Haunt]->LoadMod(".\\res\\Characters\\Diana\\Diana_Haunt.fbx");
			diana[Diana_Haunt]->EnableDisplayList();
			break;

		case Diana_Attack1:
			diana[Diana_Attack1] = new CFbxMod();
			diana[Diana_Attack1]->LoadMod(".\\res\\Characters\\Diana\\Diana_Attack1.fbx");
			diana[Diana_Attack1]->EnableDisplayList();
			diana[Diana_Attack1]->SetAnimationRate(0.5);
			break;

		case Diana_Attack2:
			diana[Diana_Attack2] = new CFbxMod();
			diana[Diana_Attack2]->LoadMod(".\\res\\Characters\\Diana\\Diana_Attack2.fbx");
			diana[Diana_Attack2]->EnableDisplayList();
			diana[Diana_Attack2]->SetAnimationRate(0.5);
			break;

		case Diana_Crit:
			diana[Diana_Crit] = new CFbxMod();
			diana[Diana_Crit]->LoadMod(".\\res\\Characters\\Diana\\Diana_Crit.fbx");
			diana[Diana_Crit]->EnableDisplayList();
			//diana[Diana_Crit]->SetAnimationRate(0.5);
			break;

		case Diana_Death:
			diana[Diana_Death] = new CFbxMod();
			diana[Diana_Death]->LoadMod(".\\res\\Characters\\Diana\\Diana_Death.fbx");
			diana[Diana_Death]->EnableDisplayList();
			diana[Diana_Death]->SetDisplayOnce(GL_TRUE);
			break;
		}
	}

	for(int i = 0; i < DianaAttackNum; i++) {
		switch(i) {
		case DianaAttack1:
			attackData[i].AttackPower = 5.0;
			break;
		case DianaAttack2:
			attackData[i].AttackPower = 7.0;
			break;
		case DianaAttackCrit:
			attackData[i].AttackPower = 10.0;
			break;
		}
		attackData[i].AttackTiming = GL_FALSE;
		attackData[i].Attacking = GL_FALSE;
	}

	state = Diana_Idle1;
}


CDiana::~CDiana(GLvoid)
{
	delete location;

	for(int i = 0; i < Diana_Animation; i++)
		delete diana[i];
}


//Load texture data to memory
GLboolean CDiana::LoadTexture(GLuint *id, char *path)
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


GLvoid CDiana::OnDisplay(GLvoid)
{
	glPushMatrix();
		glTranslatef(location->x, location->y, location->z);
		glRotatef(locAngle + 90.0, 0.0, 1.0, 0.0);
		glRotatef(moveAngle, 0.0, 1.0, 0.0);
		glScalef(0.5, 0.5, 0.5);
		diana[state]->OnDisplay();
	glPopMatrix();
}


//Rendering the character's life by OpenGL function
GLvoid CDiana::OnDisplayLife(GLvoid)
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
GLvoid CDiana::OnDisplayHeadLife(GLfloat lookAngle)
{
	if(!death) {
		glPushMatrix();
			glTranslatef(location->x, location->y + 20, location->z);

			if(state < Diana_Death) {
				glPushAttrib(GL_ENABLE_BIT);
				glDisable(GL_LIGHTING);
				glPushMatrix();
					glRotatef(lookAngle + 90, 0.0, 1.0, 0.0);
					glTranslatef(0.0, 90.0, 0.0);
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


GLvoid CDiana::OnTimer(GLvoid)
{
	diana[state]->OnTimerClick();
	
	switch(state)
	{
	case Diana_Idle1:
		if(GetMoveState() && !((m_front && m_back) || (m_left && m_right)))
			state = Diana_Run;
		break;

	case Diana_Run:
		if(!GetMoveState() && control_mode)
			state = Diana_Idle1;
		else
			Movement(m_speed);
		break;

	case Diana_Attack1:
		if((diana[state]->GetCurrentFrame()) > (diana[state]->GetAnimationTime() / 5 * 3)) {
			attackData[DianaAttack1].AttackTiming = GL_TRUE;
		}
		if(diana[state]->AnimationDone()) {
			state = Diana_Idle1;
			enableAngle = GL_TRUE;
			attackData[DianaAttack1].AttackTiming = GL_FALSE;
			attackData[DianaAttack1].Attacking = GL_FALSE;
		}
		break;

	case Diana_Attack2:
		if((diana[state]->GetCurrentFrame()) > (diana[state]->GetAnimationTime() / 5 * 3)) {
			attackData[DianaAttack2].AttackTiming = GL_TRUE;
		}
		if(diana[state]->AnimationDone()) {
			state = Diana_Idle1;
			enableAngle = GL_TRUE;
			attackData[DianaAttack2].AttackTiming = GL_FALSE;
			attackData[DianaAttack2].Attacking = GL_FALSE;
		}
		break;

	case Diana_Crit:
		if((diana[state]->GetCurrentFrame()) > (diana[state]->GetAnimationTime() / 5 * 3)) {
			attackData[DianaAttackCrit].AttackTiming = GL_TRUE;
		}
		if(diana[state]->AnimationDone()) {
			state = Diana_Idle1;
			enableAngle = GL_TRUE;
			attackData[DianaAttackCrit].AttackTiming = GL_FALSE;
			attackData[DianaAttackCrit].Attacking = GL_FALSE;
		}
		break;

	case Diana_Haunt:
		if(m_Avoid < 5) {
			m_Avoid++;
			Movement(40.0);
		}
		else {
			state = Diana_Run;
			MoveReset();
		}
		break;

	case Diana_Death:
		if(diana[state]->AnimationDone())
			death = GL_TRUE;
		break;
	}
}


//Attacked event
GLvoid CDiana::OnAttacked(GLfloat damage)
{
	health -= damage;
	if(health < 0.0) { //No life
		health = 0.0;
		for(int i = 0; i < DianaAttackNum; i++)
			attackData[i].AttackTiming = GL_FALSE;
		MoveReset();
		state = Diana_Death;
	}
}


GLvoid CDiana::Movement(GLfloat speed)
{
	if(m_front && m_back) {
		moveAngle = 0.0;
		state = Diana_Idle1;
		return;
	}

	if(m_left && m_right) {
		moveAngle = 0.0;
		state = Diana_Idle1;
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


GLvoid CDiana::MoveFront(GLboolean m_enable)
{
	if(m_front != m_enable && state <= Diana_Run)
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


GLvoid CDiana::MoveBack(GLboolean m_enable)
{
	if(m_back != m_enable && state <= Diana_Run)
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


GLvoid CDiana::MoveLeft(GLboolean m_enable)
{
	if(m_left != m_enable && state <= Diana_Run)
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


GLvoid CDiana::MoveRight(GLboolean m_enable)
{
	if(m_right != m_enable && state <= Diana_Run)
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


GLvoid CDiana::Avoid(GLvoid)
{
	if(state <= Diana_Haunt && GetMoveState()) {
		m_Avoid = 0.0;
		state = Diana_Haunt;
	}
}


GLvoid CDiana::Attack(GLvoid)
{
	if(state < Diana_Attack1) {
		enableAngle = GL_FALSE;
		MoveReset();
		state = Diana_Attack1;
		attackData[DianaAttack1].AttackTiming = GL_FALSE;
		attackData[DianaAttack1].Attacking = GL_TRUE;
	}
	else if(state == Diana_Attack1) {
		if((diana[state]->GetCurrentFrame()) > (diana[state]->GetAnimationTime() / 5 * 4)) {
			diana[Diana_Attack1]->ResetCurrentTime();
			state = Diana_Attack2;
			attackData[DianaAttack2].AttackTiming = GL_FALSE;
			attackData[DianaAttack2].Attacking = GL_TRUE;
		}
	}
	else if(state == Diana_Attack2) {
		if((diana[state]->GetCurrentFrame()) > (diana[state]->GetAnimationTime() / 5 * 4)) {
			diana[Diana_Attack2]->ResetCurrentTime();
			state = Diana_Crit;
			attackData[DianaAttackCrit].AttackTiming = GL_FALSE;
			attackData[DianaAttackCrit].Attacking = GL_TRUE;
		}
	}
}



