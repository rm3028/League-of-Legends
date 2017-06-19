/***************************
4099056008 陳昱全 期末Project 6/18
4099056030 黃湋縜 期末Project 6/18
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