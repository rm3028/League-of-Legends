/***************************
4099056008 ���R�� ����Project 6/18
4099056030 ������ ����Project 6/18
***************************/
#pragma once
#include "SelectCharacter.h"

class Server{
private:
	SelectCharacter* selectCharacter;


public:
	Server::Server(int str){
		selectCharacter = new SelectCharacter(str);

	}

	SelectCharacter* Server::getSelectCharacter(){return selectCharacter; }

};