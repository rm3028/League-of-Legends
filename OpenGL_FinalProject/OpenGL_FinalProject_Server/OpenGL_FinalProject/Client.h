/***************************
4099056008 ���R�� ����Project 6/18
4099056030 ������ ����Project 6/18
***************************/
#pragma once
#include "SelectCharacter.h"

class Client{
private:
	SelectCharacter* selectCharacter;


public:
	Client::Client(int str){
		selectCharacter = new SelectCharacter(str);

	}

	SelectCharacter* Client::getSelectCharacter(){return selectCharacter; }

};