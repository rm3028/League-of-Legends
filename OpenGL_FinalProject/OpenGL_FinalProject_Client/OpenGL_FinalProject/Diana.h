/***************************
4099056008 陳昱全 期末Project 6/18
4099056030 黃湋縜 期末Project 6/18
***************************/
#pragma once
#include "FBX_Loader\FbxMod.h"
#include "SceneMap.h"
#include "Camera.h"


#define MAXHEALTH 1000.0	//Define character's max life point


enum animationDiana
	{
		Diana_Idle1, 
		Diana_Run, 
		Diana_Haunt, 
		Diana_Attack1, 
		Diana_Attack2, 
		Diana_Crit, 
		Diana_Death, 
		Diana_Animation
	};

//ID of Attack animation
enum DianaAttackAnm {
	DianaAttack1, 
	DianaAttack2, 
	DianaAttackCrit, 
	DianaAttackNum
};

class CDiana
{
private:
	//Data of attack
	struct AttackData {
		GLfloat AttackPower;	//Power of attack
		GLboolean Attacking;
		GLboolean AttackTiming;	//Timing of the attack has damage
	};

private:
	CFbxMod* diana[Diana_Animation];
	GLuint state;

	GLboolean enableAngle;
	GLfloat locAngle, moveAngle;
	CVector* location;

	GLboolean m_front, m_back, m_left, m_right;
	GLfloat m_speed;

	GLfloat m_Avoid;

	//ture 是鍵盤控制，false是外部操作
	GLboolean control_mode; 

	CSceneMap *sceneData;	//Point to scene data (for collision detection)

	AttackData attackData[DianaAttackNum];	//Attack data

	GLfloat health;	//Life point
	GLboolean death;

	GLuint lifeTex;

public:
	CDiana(GLvoid);
	~CDiana(GLvoid);
	
	
	GLuint GetState(GLvoid) { return state; }
	GLvoid SetState(GLuint State) { state = State; }
	CFbxMod* GetMod(GLvoid) { return diana[state]; }
	GLvoid SetLocation(GLfloat x, GLfloat y, GLfloat z) { location->SetVector(x, y, z); }
	CVector* GetLocation(GLvoid) { return location; }
	GLuint GetFrameTime(GLvoid) { return diana[state]->GetFrameTime(); }
	GLboolean GetMoveState(GLvoid) { return m_front || m_back || m_left || m_right; }
	
	GLfloat GetLocAngle(GLvoid){return locAngle;}
	GLvoid SetLocAngle(GLfloat angle) { if(enableAngle)locAngle = angle; }
	GLfloat GetmoveAngle(GLvoid){return moveAngle;}
	GLvoid SetmoveAngle(GLfloat angle) { if(enableAngle)moveAngle = angle; }

	//Set scene data
	GLvoid SetScene(CSceneMap *scene) { sceneData = scene; }
	//Get scene data
	CSceneMap* GetScene(GLvoid) { return sceneData; }

	//Get the attack power
	GLvoid SetAttackPower(GLuint id, GLfloat power) { attackData[id].AttackPower = power; }
	GLvoid SetAttacking(GLuint id, GLboolean attacking) { attackData[id].Attacking = attacking; }
	GLvoid SetAttackTiming(GLuint id, GLboolean attacktiming) { attackData[id].AttackTiming = attacktiming; }

	//Get the attack power
	GLfloat GetAttackPower() { if(state == Diana_Attack1) { return attackData[DianaAttack1].AttackPower;} if(state == Diana_Attack2) { return attackData[DianaAttack2].AttackPower;} if(state == Diana_Crit) { return attackData[DianaAttackCrit].AttackPower;} return 0.0; }
	GLboolean GetAttackState() { if(state == Diana_Attack1) { return attackData[DianaAttack1].Attacking;} if(state == Diana_Attack2) { return attackData[DianaAttack2].Attacking;} if(state == Diana_Crit) { return attackData[DianaAttackCrit].Attacking;} return GL_FALSE; }
	GLboolean GetAttackTiming() { if(state == Diana_Attack1) { return attackData[DianaAttack1].AttackTiming;} if(state == Diana_Attack2) { return attackData[DianaAttack2].AttackTiming;} if(state == Diana_Crit) { return attackData[DianaAttackCrit].AttackTiming;} return GL_FALSE; }

	//Load texture data to memory
	GLboolean LoadTexture(GLuint *id, char *path);

	GLvoid OnDisplay(GLvoid);
	GLvoid OnDisplayLife(GLvoid);
	GLvoid OnDisplayHeadLife(GLfloat lookAngle);
	GLvoid OnTimer(GLvoid);

	//Attacked event
	GLvoid OnAttacked(GLfloat damage);

	GLvoid OnKeyDown(unsigned char key, int x, int y);
	GLvoid OnKeyUp(unsigned char key, int x, int y);

	GLvoid Movement(GLfloat speed);
	GLvoid MoveFront(GLboolean m_enable);
	GLvoid MoveBack(GLboolean m_enable);
	GLvoid MoveLeft(GLboolean m_enable);
	GLvoid MoveRight(GLboolean m_enable);
	GLvoid MoveReset(GLvoid) { m_front = GL_FALSE;	m_back = GL_FALSE;	m_left = GL_FALSE;	m_right = GL_FALSE; }

	GLvoid Avoid(GLvoid);
	GLvoid Attack(GLvoid);

	GLvoid setControl_Mode(GLboolean mode){control_mode=mode;}

	GLfloat GetHealth(){return health;}
	GLvoid SetHealth(GLfloat h){health=h;}

	GLboolean GetDeath(){ return death ;} 
	GLvoid SetDeath(GLboolean d){ death = d;}
};

