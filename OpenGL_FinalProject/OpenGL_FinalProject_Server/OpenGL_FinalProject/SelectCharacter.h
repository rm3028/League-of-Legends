/***************************
4099056008 陳昱全 期末Project 6/18
4099056030 黃湋縜 期末Project 6/18
***************************/
#pragma once
#include "Akali.h"
#include "Alistar.h"
#include "Diana.h"
#include "Leona.h"
#include "Shen.h"
#include "Malphite.h"

enum GameCharacter {
		SelectAkali, 
		SelectShen, 
		SelectAlistar,
		SelectDiana, 
		SelectLeona, 
		SelectMalphite,
		CharacterNum
};

class SelectCharacter{
private:
	CAkali *akali;
	CAlistar *alistar;
	CDiana *diana;
	CLeona *leona;
	CShen *shen;
	CMalphite *malphite;
	
	//記錄所選角色
	int character;
	
public:
	SelectCharacter::SelectCharacter(int str){ character=str; }

	//初始化
	void SelectCharacter::init_Akali(){akali=new CAkali();}
	void SelectCharacter::init_Alistar(){alistar=new CAlistar();}
	void SelectCharacter::init_Diana(){diana=new CDiana();}
	void SelectCharacter::init_Leona(){leona=new CLeona();}
	void SelectCharacter::init_Shen(){shen=new CShen();}
	void SelectCharacter::init_Malphite(){malphite=new CMalphite();}

	CAkali* SelectCharacter::getAkali(){return akali;}
	CAlistar* SelectCharacter::getAlistar(){return alistar;}
	CDiana* SelectCharacter::getDiana(){return diana;}
	CLeona* SelectCharacter::getLeona(){return leona;}
	CShen* SelectCharacter::getShen(){return shen;}
	CMalphite* SelectCharacter::getMalphite(){return malphite;}

	GLvoid SelectCharacter::init_Character();
	GLvoid SelectCharacter::OnDisplay();
	GLvoid SelectCharacter::OnTimer();

	GLuint SelectCharacter::GetState();
	CVector* SelectCharacter::GetLocation();
	GLfloat SelectCharacter::GetLocAngle();
	GLfloat SelectCharacter::GetmoveAngle();
	CFbxMod* SelectCharacter::GetMod();

	GLvoid SelectCharacter::Avoid();
	GLvoid SelectCharacter::Attack();

	GLvoid SelectCharacter::MoveFront(GLboolean m_enable);
	GLvoid SelectCharacter::MoveLeft(GLboolean	m_enable);
	GLvoid SelectCharacter::MoveBack(GLboolean	 m_enable);
	GLvoid SelectCharacter::MoveRight(GLboolean m_enable);

	GLvoid SelectCharacter::SetScene(CSceneMap *scene);
	GLvoid SelectCharacter::setControl_Mode(GLboolean mode);
	GLvoid SelectCharacter::SetState(GLuint State);
	GLvoid SelectCharacter::SetLocation(GLfloat x,GLfloat y,GLfloat z);
	GLvoid SelectCharacter::SetLocAngle(GLfloat angle);
	GLvoid SelectCharacter::SetmoveAngle(GLfloat angle);

	//Get the attack power
	GLvoid SelectCharacter::SetAttackPower(GLuint id, GLfloat power);
	GLvoid SelectCharacter::SetAttacking(GLuint id, GLboolean attacking);
	GLvoid SelectCharacter::SetAttackTiming(GLuint id, GLboolean attacktiming);

	//Get the attack power
	GLfloat SelectCharacter::GetAttackPower();
	GLboolean SelectCharacter::GetAttackState();
	GLboolean SelectCharacter::GetAttackTiming();

	//Attacked event
	GLvoid SelectCharacter::OnAttacked(GLfloat damage);
	GLvoid SelectCharacter::OnDisplayLife(GLvoid);
	GLvoid SelectCharacter::OnDisplayHeadLife(GLfloat lookAngle);

	GLfloat SelectCharacter::GetHealth();
	GLvoid SelectCharacter::SetHealth(GLfloat h);

	GLboolean SelectCharacter::GetDeath();
	GLvoid SelectCharacter::SetDeath(GLboolean d);
};