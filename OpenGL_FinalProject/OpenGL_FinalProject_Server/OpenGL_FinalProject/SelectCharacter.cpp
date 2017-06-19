/***************************
4099056008 陳昱全 期末Project 6/18
4099056030 黃湋縜 期末Project 6/18
***************************/
#include "stdafx.h"
#include "SelectCharacter.h"

GLvoid SelectCharacter::init_Character(){

	if (character==SelectAkali)
		akali=new CAkali();
	else if(character==SelectAlistar)
		alistar=new CAlistar();
	else if(character==SelectDiana)
		diana=new CDiana();
	else if(character==SelectLeona)
		leona=new CLeona();
	else if(character==SelectShen)
		shen=new CShen();
	else if(character==SelectMalphite)
		malphite=new CMalphite();
}

GLvoid SelectCharacter::OnDisplay(){

	if (character==SelectAkali)
		akali->OnDisplay();
	else if(character==SelectAlistar)
		alistar->OnDisplay();
	else if(character==SelectDiana)
		diana->OnDisplay();
	else if(character==SelectLeona)
		leona->OnDisplay();
	else if(character==SelectShen)
		shen->OnDisplay();
	else if(character==SelectMalphite)
		malphite->OnDisplay();
}

GLvoid SelectCharacter::OnTimer(){

	if (character==SelectAkali)
		akali->OnTimer();
	else if(character==SelectAlistar)
		alistar->OnTimer();
	else if(character==SelectDiana)
		diana->OnTimer();
	else if(character==SelectLeona)
		leona->OnTimer();
	else if(character==SelectShen)
		shen->OnTimer();
	else if(character==SelectMalphite)
		malphite->OnTimer();
}

CVector* SelectCharacter::GetLocation(){

	CVector *vector;
	if (character==SelectAkali)
		vector=akali->GetLocation();
	else if(character==SelectAlistar)
		vector=alistar->GetLocation();
	else if(character==SelectDiana)
		vector=diana->GetLocation();
	else if(character==SelectLeona)
		vector=leona->GetLocation();
	else if(character==SelectShen)
		vector=shen->GetLocation();
	else if(character==SelectMalphite)
		vector=malphite->GetLocation();

	return vector;
}

GLfloat SelectCharacter::GetLocAngle(){

	GLfloat temp;
	if (character==SelectAkali)
		temp=akali->GetLocAngle();
	else if(character==SelectAlistar)
		temp=alistar->GetLocAngle();
	else if(character==SelectDiana)
		temp=diana->GetLocAngle();
	else if(character==SelectLeona)
		temp=leona->GetLocAngle();
	else if(character==SelectShen)
		temp=shen->GetLocAngle();
	else if(character==SelectMalphite)
		temp=malphite->GetLocAngle();

	return temp;
}

GLfloat SelectCharacter::GetmoveAngle(){

	GLfloat temp;
	if (character==SelectAkali)
		temp=akali->GetmoveAngle();
	else if(character==SelectAlistar)
		temp=alistar->GetmoveAngle();
	else if(character==SelectDiana)
		temp=diana->GetmoveAngle();
	else if(character==SelectLeona)
		temp=leona->GetmoveAngle();
	else if(character==SelectShen)
		temp=shen->GetmoveAngle();
	else if(character==SelectMalphite)
		temp=malphite->GetmoveAngle();

	return temp;
}

GLvoid SelectCharacter::Avoid(){

	if (character==SelectAkali)
		akali->Avoid();
	else if(character==SelectAlistar)
		alistar->Avoid();
	else if(character==SelectDiana)
		diana->Avoid();
	else if(character==SelectLeona)
		leona->Avoid();
	else if(character==SelectShen)
		shen->Avoid();
	else if(character==SelectMalphite)
		malphite->Avoid();
}

GLvoid SelectCharacter::Attack(){

	if (character==SelectAkali)
		akali->Attack();
	else if(character==SelectAlistar)
		alistar->Attack();
	else if(character==SelectDiana)
		diana->Attack();
	else if(character==SelectLeona)
		leona->Attack();
	else if(character==SelectShen)
		shen->Attack();
	else if(character==SelectMalphite)
		malphite->Attack();
}

GLuint SelectCharacter::GetState(){

	GLuint temp;
	if (character==SelectAkali)
		temp=akali->GetState();
	else if(character==SelectAlistar)
		temp=alistar->GetState();
	else if(character==SelectDiana)
		temp=diana->GetState();
	else if(character==SelectLeona)
		temp=leona->GetState();
	else if(character==SelectShen)
		temp=shen->GetState();
	else if(character==SelectMalphite)
		temp=malphite->GetState();

	return temp;
}

GLvoid SelectCharacter::SetLocAngle(GLfloat angle){

	if (character==SelectAkali)
		akali->SetLocAngle(angle);
	else if(character==SelectAlistar)
		alistar->SetLocAngle(angle);
	else if(character==SelectDiana)
		diana->SetLocAngle(angle);
	else if(character==SelectLeona)
		leona->SetLocAngle(angle);
	else if(character==SelectShen)
		shen->SetLocAngle(angle);
	else if(character==SelectMalphite)
		malphite->SetLocAngle(angle);
}

GLvoid SelectCharacter::MoveFront(GLboolean m_enable){

	if (character==SelectAkali)
		akali->MoveFront(m_enable);
	else if(character==SelectAlistar)
		alistar->MoveFront(m_enable);
	else if(character==SelectDiana)
		diana->MoveFront(m_enable);
	else if(character==SelectLeona)
		leona->MoveFront(m_enable);
	else if(character==SelectShen)
		shen->MoveFront(m_enable);
	else if(character==SelectMalphite)
		malphite->MoveFront(m_enable);
}

GLvoid SelectCharacter::MoveLeft(GLboolean m_enable){

	if (character==SelectAkali)
		akali->MoveLeft(m_enable);
	else if(character==SelectAlistar)
		alistar->MoveLeft(m_enable);
	else if(character==SelectDiana)
		diana->MoveLeft(m_enable);
	else if(character==SelectLeona)
		leona->MoveLeft(m_enable);
	else if(character==SelectShen)
		shen->MoveLeft(m_enable);
	else if(character==SelectMalphite)
		malphite->MoveLeft(m_enable);
}

GLvoid SelectCharacter::MoveBack(GLboolean m_enable){

	if (character==SelectAkali)
		akali->MoveBack(m_enable);
	else if(character==SelectAlistar)
		alistar->MoveBack(m_enable);
	else if(character==SelectDiana)
		diana->MoveBack(m_enable);
	else if(character==SelectLeona)
		leona->MoveBack(m_enable);
	else if(character==SelectShen)
		shen->MoveBack(m_enable);
	else if(character==SelectMalphite)
		malphite->MoveBack(m_enable);
}

GLvoid SelectCharacter::MoveRight(GLboolean m_enable){

	if (character==SelectAkali)
		akali->MoveRight(m_enable);
	else if(character==SelectAlistar)
		alistar->MoveRight(m_enable);
	else if(character==SelectDiana)
		diana->MoveRight(m_enable);
	else if(character==SelectLeona)
		leona->MoveRight(m_enable);
	else if(character==SelectShen)
		shen->MoveRight(m_enable);
	else if(character==SelectMalphite)
		malphite->MoveRight(m_enable);
}

GLvoid SelectCharacter::SetLocation(GLfloat x,GLfloat y,GLfloat z){

	if (character==SelectAkali)
		akali->SetLocation(x,y,z);
	else if(character==SelectAlistar)
		alistar->SetLocation(x,y,z);
	else if(character==SelectDiana)
		diana->SetLocation(x,y,z);
	else if(character==SelectLeona)
		leona->SetLocation(x,y,z);
	else if(character==SelectShen)
		shen->SetLocation(x,y,z);
	else if(character==SelectMalphite)
		malphite->SetLocation(x,y,z);
}

GLvoid SelectCharacter::SetScene(CSceneMap *scene){

	if (character==SelectAkali)
		akali->SetScene(scene);
	else if(character==SelectAlistar)
		alistar->SetScene(scene);
	else if(character==SelectDiana)
		diana->SetScene(scene);
	else if(character==SelectLeona)
		leona->SetScene(scene);
	else if(character==SelectShen)
		shen->SetScene(scene);
	else if(character==SelectMalphite)
		malphite->SetScene(scene);
}

GLvoid SelectCharacter::SetState(GLuint State){

	if (character==SelectAkali)
		akali->SetState(State);
	else if(character==SelectAlistar)
		alistar->SetState(State);
	else if(character==SelectDiana)
		diana->SetState(State);
	else if(character==SelectLeona)
		leona->SetState(State);
	else if(character==SelectShen)
		shen->SetState(State);
	else if(character==SelectMalphite)
		malphite->SetState(State);
}

GLvoid SelectCharacter::setControl_Mode(GLboolean mode){

	if (character==SelectAkali)
		akali->setControl_Mode(mode);
	else if(character==SelectAlistar)
		alistar->setControl_Mode(mode);
	else if(character==SelectDiana)
		diana->setControl_Mode(mode);
	else if(character==SelectLeona)
		leona->setControl_Mode(mode);
	else if(character==SelectShen)
		shen->setControl_Mode(mode);
	else if(character==SelectMalphite)
		malphite->setControl_Mode(mode);
}

GLvoid SelectCharacter::SetmoveAngle(GLfloat angle){

	if (character==SelectAkali)
		akali->SetmoveAngle(angle);
	else if(character==SelectAlistar)
		alistar->SetmoveAngle(angle);
	else if(character==SelectDiana)
		diana->SetmoveAngle(angle);
	else if(character==SelectLeona)
		leona->SetmoveAngle(angle);
	else if(character==SelectShen)
		shen->SetmoveAngle(angle);
	else if(character==SelectMalphite)
		malphite->SetmoveAngle(angle);
}

CFbxMod* SelectCharacter::GetMod(){

	CFbxMod* temp;
	if (character==SelectAkali)
		temp=akali->GetMod();
	else if(character==SelectAlistar)
		temp=alistar->GetMod();
	else if(character==SelectDiana)
		temp=diana->GetMod();
	else if(character==SelectLeona)
		temp=leona->GetMod();
	else if(character==SelectShen)
		temp=shen->GetMod();
	else if(character==SelectMalphite)
		temp=malphite->GetMod();

	return temp;
}

//Get the attack power
GLvoid SelectCharacter::SetAttackPower(GLuint id, GLfloat power){

	if (character==SelectAkali)
		akali->SetAttackPower(id, power);
	else if(character==SelectAlistar)
		alistar->SetAttackPower(id, power);
	else if(character==SelectDiana)
		diana->SetAttackPower(id, power);
	else if(character==SelectLeona)
		leona->SetAttackPower(id, power);
	else if(character==SelectShen)
		shen->SetAttackPower(id, power);
	else if(character==SelectMalphite)
		malphite->SetAttackPower(id,power);
}
//Get the attack power
GLfloat SelectCharacter::GetAttackPower(){

	if (character==SelectAkali)
		return akali->GetAttackPower();
	else if(character==SelectAlistar)
		return alistar->GetAttackPower();
	else if(character==SelectDiana)
		return diana->GetAttackPower();
	else if(character==SelectLeona)
		return leona->GetAttackPower();
	else if(character==SelectShen)
		return shen->GetAttackPower();
	else if(character==SelectMalphite)
		return malphite->GetAttackPower();
}
//Attacked event
GLvoid SelectCharacter::OnAttacked(GLfloat damage){

	if (character==SelectAkali)
		akali->OnAttacked(damage);
	else if(character==SelectAlistar)
		alistar->OnAttacked(damage);
	else if(character==SelectDiana)
		diana->OnAttacked(damage);
	else if(character==SelectLeona)
		leona->OnAttacked(damage);
	else if(character==SelectShen)
		shen->OnAttacked(damage);
	else if(character==SelectMalphite)
		malphite->OnAttacked(damage);
}
GLvoid SelectCharacter::OnDisplayLife(GLvoid){

	if (character==SelectAkali)
		akali->OnDisplayLife();
	else if(character==SelectAlistar)
		alistar->OnDisplayLife();
	else if(character==SelectDiana)
		diana->OnDisplayLife();
	else if(character==SelectLeona)
		leona->OnDisplayLife();
	else if(character==SelectShen)
		shen->OnDisplayLife();
	else if(character==SelectMalphite)
		malphite->OnDisplayLife();
}

GLvoid SelectCharacter::OnDisplayHeadLife(GLfloat lookAngle){

	if (character==SelectAkali)
		akali->OnDisplayHeadLife(lookAngle);
	else if(character==SelectAlistar)
		alistar->OnDisplayHeadLife(lookAngle);
	else if(character==SelectDiana)
		diana->OnDisplayHeadLife(lookAngle);
	else if(character==SelectLeona)
		leona->OnDisplayHeadLife(lookAngle);
	else if(character==SelectShen)
		shen->OnDisplayHeadLife(lookAngle);
	else if(character==SelectMalphite)
		malphite->OnDisplayHeadLife(lookAngle);
}

GLboolean SelectCharacter::GetAttackState() {
	if (character==SelectAkali)
		return akali->GetAttackState();
	else if(character==SelectAlistar)
		return alistar->GetAttackState();
	else if(character==SelectDiana)
		return diana->GetAttackState();
	else if(character==SelectLeona)
		return leona->GetAttackState();
	else if(character==SelectShen)
		return shen->GetAttackState();
	else if(character==SelectMalphite)
		return malphite->GetAttackState();
}

GLboolean SelectCharacter::GetAttackTiming(){
	if (character==SelectAkali)
		return akali->GetAttackTiming();
	else if(character==SelectAlistar)
		return alistar->GetAttackTiming();
	else if(character==SelectDiana)
		return diana->GetAttackTiming();
	else if(character==SelectLeona)
		return leona->GetAttackTiming();
	else if(character==SelectShen)
		return shen->GetAttackTiming();
	else if(character==SelectMalphite)
		return malphite->GetAttackTiming();
}

GLvoid SelectCharacter::SetAttacking(GLuint id, GLboolean attacking){

	if (character==SelectAkali)
		akali->SetAttacking(id,attacking);
	else if(character==SelectAlistar)
		alistar->SetAttacking(id,attacking);
	else if(character==SelectDiana)
		diana->SetAttacking(id,attacking);
	else if(character==SelectLeona)
		leona->SetAttacking(id,attacking);
	else if(character==SelectShen)
		shen->SetAttacking(id,attacking);
	else if(character==SelectMalphite)
		malphite->SetAttacking(id,attacking);

}

GLvoid SelectCharacter::SetAttackTiming(GLuint id, GLboolean attacktiming){

	if (character==SelectAkali)
		akali->SetAttackTiming(id,attacktiming);
	else if(character==SelectAlistar)
		alistar->SetAttackTiming(id,attacktiming);
	else if(character==SelectDiana)
		diana->SetAttackTiming(id,attacktiming);
	else if(character==SelectLeona)
		leona->SetAttackTiming(id,attacktiming);
	else if(character==SelectShen)
		shen->SetAttackTiming(id,attacktiming);
	else if(character==SelectMalphite)
		malphite->SetAttackTiming(id,attacktiming);

}

GLfloat SelectCharacter::GetHealth(){

	if (character==SelectAkali)
		return akali->GetHealth();
	else if(character==SelectAlistar)
		return alistar->GetHealth();
	else if(character==SelectDiana)
		return diana->GetHealth();
	else if(character==SelectLeona)
		return leona->GetHealth();
	else if(character==SelectShen)
		return shen->GetHealth();
	else if(character==SelectMalphite)
		return malphite->GetHealth();


}
GLvoid SelectCharacter::SetHealth(GLfloat h){

	if (character==SelectAkali)
		akali->SetHealth(h);
	else if(character==SelectAlistar)
		alistar->SetHealth(h);
	else if(character==SelectDiana)
		diana->SetHealth(h);
	else if(character==SelectLeona)
		leona->SetHealth(h);
	else if(character==SelectShen)
		shen->SetHealth(h);
	else if(character==SelectMalphite)
		malphite->SetHealth(h);


}

GLboolean SelectCharacter::GetDeath(){

	if (character==SelectAkali)
		return akali->GetDeath();
	else if(character==SelectAlistar)
		return alistar->GetDeath();
	else if(character==SelectDiana)
		return diana->GetDeath();
	else if(character==SelectLeona)
		return leona->GetDeath();
	else if(character==SelectShen)
		return shen->GetDeath();
	else if(character==SelectMalphite)
		return malphite->GetDeath();


}
GLvoid SelectCharacter::SetDeath(GLboolean d){

	if (character==SelectAkali)
		akali->SetDeath(d);
	else if(character==SelectAlistar)
		alistar->SetDeath(d);
	else if(character==SelectDiana)
		diana->SetDeath(d);
	else if(character==SelectLeona)
		leona->SetDeath(d);
	else if(character==SelectShen)
		shen->SetDeath(d);
	else if(character==SelectMalphite)
		malphite->SetDeath(d);




}