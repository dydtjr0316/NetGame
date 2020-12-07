/*
Copyright 2017 Lee Taek Hee (Korea Polytech University)

This program is free software: you can redistribute it and/or modify
it under the terms of the What The Hell License. Do it plz.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY.
*/

#include "stdafx.h"
#include <iostream>
#include "Dependencies\glew.h"
#include "Dependencies\freeglut.h"

#include "Renderer.h"
#include "ScnMgr.h"
#include "CServer.h"
#include "Player.h"

//ScnMgr* g_ScnMgr = NULL;
int g_PrevTime = 0;
SERVER server;

HANDLE event;

int recvn(SOCKET s, char* buf, int len, int flags)
{
	int received;
	char* ptr = buf;
	int left = len;

	while (left > 0) {
		received = recv(s, ptr, left, flags);

		if (received == SOCKET_ERROR)
			return SOCKET_ERROR;
		else if (received == 0)
			break;

		left -= received;
		ptr += received;
	}

	return (len - left);
}

void RenderScene(int temp)
{
	int currTime = glutGet(GLUT_ELAPSED_TIME);
	int elapsedTime = currTime - g_PrevTime;
	float elapsedTimeInSec = (float)elapsedTime / 1000.f;
	g_PrevTime = currTime;

	//std::cout << "elased time : " << elapsedTime << std::endl;

	ScnMgr::GetInstance()->Update(elapsedTimeInSec);
	ScnMgr::GetInstance()->RenderScene();
	ScnMgr::GetInstance()->DoGarbageCollect();

	glutSwapBuffers();

	glutTimerFunc(10, RenderScene, 0);
}

void Display(void)
{

}

void Idle(void)
{
	
}

void MouseInput(int button, int state, int x, int y)
{
	
}

void KeyDownInput(unsigned char key, int x, int y)
{
	ScnMgr::GetInstance()->KeyDownInput(key, x, y);
}

void KeyUpInput(unsigned char key, int x, int y)
{
	ScnMgr::GetInstance()->KeyUpInput(key, x, y);
}

void SpecialKeyDownInput(int key, int x, int y)
{
	ScnMgr::GetInstance()->SpecialKeyDownInput(key, x, y);
}

void SpecialKeyUpInput(int key, int x, int y)
{
	ScnMgr::GetInstance()->SpecialKeyUpInput(key, x, y);
}


int main(int argc, char **argv)
{
	int id = server.ConnectServer();

	cout << "닉네임 입력: ";
	char nick[32];
	cin >> nick;
	SetEvent(event);
	server.SendLoginPacket(id, nick);

	// Initialize GL things
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Game Software Engineering KPU");

	glewInit();
	if (glewIsSupported("GL_VERSION_3_0"))
	{
		std::cout << " GLEW Version is 3.0\n ";
	}
	else
	{
		std::cout << "GLEW 3.0 not supported\n ";
	}

	SC_Client_LoginOK_Packet loginok_packet;
	ZeroMemory(&loginok_packet, sizeof(SC_Client_LoginOK_Packet));
	int ret = recvn(server.GetSock(), (char*)&loginok_packet, sizeof(loginok_packet), 0);

	cout << loginok_packet.nickname << endl;
	{
		glutDisplayFunc(Display);
		glutIdleFunc(Idle);

		glutKeyboardFunc(KeyDownInput); // key down event callback
		glutKeyboardUpFunc(KeyUpInput); // key up event callback

		glutMouseFunc(MouseInput);

		glutSpecialFunc(SpecialKeyDownInput);
		glutSpecialUpFunc(SpecialKeyUpInput);

		g_PrevTime = glutGet(GLUT_ELAPSED_TIME);
		glutTimerFunc(10, RenderScene, 0);
	}

	if (loginok_packet.type == NICKNAME_USE) {
		WaitForSingleObject(event, INFINITE);

		cout << "waiting for other client to enter" << endl;
		SC_Client_Enter_Packet enter_packet;
		ZeroMemory(&enter_packet, sizeof(SC_Client_Enter_Packet));
		ret = recvn(server.GetSock(), (char*)&enter_packet, sizeof(enter_packet), 0);

		for (int i = 0; i < 2; ++i)
		{
			CPlayer* Pobj = new CPlayer;
			static float a = -0.3f;
			ScnMgr::GetInstance()->AddObject(0.f, a, 0.f, 0.5f, 0.5f, 0.5f, 1.f, 1.f, 1.f, 1.f, 0.f, 0.f, 0.f, 1.f, 0.9f, TYPE_NORMAL, 6.f, Pobj);
			a+=0.3f;
			ScnMgr::GetInstance()->m_Obj[i]->SetID(i);

			// owner 지정
			if (id == i)
				Pobj->is_owner = true;
		}
	}

	glutMainLoop();
		
	ScnMgr::GetInstance()->DestroyInstance();

	return 0;
}