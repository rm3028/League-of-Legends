/***************************
4099056008 陳昱全 期末Project 6/18
4099056030 黃湋縜 期末Project 6/18
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

	//宣告 socket 位址資訊(不同的通訊,有不同的位址資訊,所以會有不同的資料結構存放這些位址資訊)
	SOCKADDR_IN addr;
	int addrlen ;

	//建立 socket
	SOCKET sListen; //listening for an incoming connection
	SOCKET sConnect; //operating if a connection was found

	//等待連線
	SOCKADDR_IN clinetAddr;

public:
	SocketClient ::SocketClient(){
		DLLVSERION = MAKEWORD(2,1);//Winsocket-DLL 版本

		//用 WSAStartup 開始 Winsocket-DLL
		r = WSAStartup(DLLVSERION, &wsaData);

		addrlen = sizeof(addr);

		//AF_INET：表示建立的 socket 屬於 internet family
		//SOCK_STREAM：表示建立的 socket 是 connection-oriented socket 
		sConnect = socket(AF_INET, SOCK_STREAM, NULL);

		//設定位址資訊的資料
		addr.sin_addr.s_addr = inet_addr("192.168.1.102");
		addr.sin_family = AF_INET;
		addr.sin_port = htons(5678);

		connect(sConnect, (SOCKADDR*)&addr, sizeof(addr));

	}

	//接收 server 端的訊息
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

		cout << "x:"<<x<<"，y:"<<y <<"，z:"<<z<<"，state:"<<state<<"，locAngle:"<<locAngle<<"，moveAngle:"<<moveAngle<<"，mCurrentList:"<<mCurrentList<<endl;
	
	}

	//傳送訊息給 server 端
	void SocketClient ::sendCoordinate(float x,float y,float z,int state,float locAngle,float moveAngle,int mCurrentList){

		string s=to_string(x)+','+to_string(y)+','+to_string(z)+','+to_string(state)+','+to_string(locAngle)+','+to_string(moveAngle)+','+to_string(mCurrentList);
		char *sendbuf = new char[s.length() + 1];
		strcpy(sendbuf, s.c_str());
		cout<<sendbuf<<endl;
		send(sConnect, sendbuf, (int)strlen(sendbuf), 0);	

	}

};
