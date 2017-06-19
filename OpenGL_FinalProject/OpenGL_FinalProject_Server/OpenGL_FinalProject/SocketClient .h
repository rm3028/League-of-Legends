/***************************
4099056008 ���R�� ����Project 6/18
4099056030 ������ ����Project 6/18
***************************/
#include "stdafx.h"

#pragma comment(lib, "Ws2_32.lib")
#include <WinSock2.h>
#include <string>
#include <iostream>


using namespace std;

class SocketClient{
public:
	int r;
	WSAData wsaData;
	WORD DLLVSERION;
	char message[50];
	float x;
	float y;
	float z;
	int state;
	float locAngle;
	float moveAngle;
	int mCurrentList;

	//�ŧi socket ��}��T(���P���q�T,�����P����}��T,�ҥH�|�����P����Ƶ��c�s��o�Ǧ�}��T)
	SOCKADDR_IN addr;
	int addrlen ;

	//�إ� socket
	SOCKET sListen; //listening for an incoming connection
	SOCKET sConnect; //operating if a connection was found

	//���ݳs�u
	SOCKADDR_IN clinetAddr;

public:
	SocketClient ::SocketClient(){
		DLLVSERION = MAKEWORD(2,1);//Winsocket-DLL ����

		//�� WSAStartup �}�l Winsocket-DLL
		r = WSAStartup(DLLVSERION, &wsaData);

		addrlen = sizeof(addr);

		//AF_INET�G��ܫإߪ� socket �ݩ� internet family
		//SOCK_STREAM�G��ܫإߪ� socket �O connection-oriented socket 
		sConnect = socket(AF_INET, SOCK_STREAM, NULL);

		//�]�w��}��T�����
		addr.sin_addr.s_addr = inet_addr("192.168.1.102");
		addr.sin_family = AF_INET;
		addr.sin_port = htons(5678);

		connect(sConnect, (SOCKADDR*)&addr, sizeof(addr));

	}

	//���� server �ݪ��T��
	void SocketClient::receiveCoordinate(){

		ZeroMemory(message, 200);
		r = recv(sConnect, message, sizeof(message), 0);
		//cout << message << endl;

		char *pch;
		pch=strtok(message,",");

		x=atof(pch);
		pch=strtok(NULL,",");
		y=atof(pch);
		pch=strtok(NULL,",");
		z=atof(pch);
		pch=strtok(NULL,",");
		state=atoi(pch);
		pch=strtok(NULL,",");
		locAngle=atof(pch);
		pch=strtok(NULL,",");
		moveAngle=atof(pch);
		pch=strtok(NULL,",");
		mCurrentList=atoi(pch);
		pch=strtok(NULL,",");

		cout << "x:"<<x<<"�Ay:"<<y <<"�Az:"<<z<<"�Astate:"<<state<<"�AlocAngle:"<<locAngle<<"�AmoveAngle:"<<moveAngle<<"�AmCurrentList:"<<mCurrentList<<endl;
	
	}

	//�ǰe�T���� server ��
	void SocketClient ::sendCoordinate(float x,float y,float z,int state,float locAngle,float moveAngle,int mCurrentList){

		string s=to_string(x)+','+to_string(y)+','+to_string(z)+','+to_string(state)+','+to_string(locAngle)+','+to_string(moveAngle)+','+to_string(mCurrentList);
		char *sendbuf = new char[s.length() + 1];
		strcpy(sendbuf, s.c_str());
		cout<<sendbuf<<endl;
		send(sConnect, sendbuf, (int)strlen(sendbuf), 0);	

	}

};
