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

ScnMgr* g_ScnMgr = NULL; 
ScnMgr* g_ScnMgr_other = NULL;
int g_PrevTime = 0;
int my_id;
SERVER server;

void RenderScene(int temp)
{
	int currTime = glutGet(GLUT_ELAPSED_TIME);
	int elapsedTime = currTime - g_PrevTime;
	float elapsedTimeInSec = (float)elapsedTime / 1000.f;
	g_PrevTime = currTime;

	//std::cout << "elased time : " << elapsedTime << std::endl;

	g_ScnMgr->Update(elapsedTimeInSec);
	g_ScnMgr->RenderScene();
	g_ScnMgr->DoGarbageCollect();

	if (g_ScnMgr_other != NULL) {
		g_ScnMgr_other->Update(elapsedTimeInSec);
		g_ScnMgr_other->RenderScene();
		g_ScnMgr_other->DoGarbageCollect();
	}

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
	g_ScnMgr->KeyDownInput(key, x, y);
	if (g_ScnMgr_other != NULL) g_ScnMgr_other->KeyDownInput(key, x, y);
}

void KeyUpInput(unsigned char key, int x, int y)
{
	g_ScnMgr->KeyUpInput(key, x, y);
	if (g_ScnMgr_other != NULL) g_ScnMgr_other->KeyUpInput(key, x, y);
}

void SpecialKeyDownInput(int key, int x, int y)
{
	g_ScnMgr->SpecialKeyDownInput(key, x, y);
	if (g_ScnMgr_other != NULL) g_ScnMgr_other->SpecialKeyDownInput(key, x, y);
}

void SpecialKeyUpInput(int key, int x, int y)
{
	g_ScnMgr->SpecialKeyUpInput(key, x, y);
	if (g_ScnMgr_other != NULL) g_ScnMgr_other->SpecialKeyUpInput(key, x, y);
}


int main(int argc, char **argv)
{
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
	
	my_id = server.ConnectServer();

	char nick[32];
	cin >> nick;
	server.SendLoginPacket(my_id, nick);

	g_ScnMgr = ScnMgr::GetInstance();
	g_ScnMgr->SetID(my_id);

	SC_Client_Enter_Packet packet;
	int retval = recv((SOCKET)server.GetSock(), (char*)&packet, sizeof(SC_Client_Enter_Packet), 0);

	if (packet.type == ENTER_USER) {
		cout << packet.nickname;
		g_ScnMgr_other = ScnMgr::GetInstance();
		g_ScnMgr_other->SetID(packet.id);
	}

	glutDisplayFunc(Display);
	glutIdleFunc(Idle);

	glutKeyboardFunc(KeyDownInput); // key down event callback
	glutKeyboardUpFunc(KeyUpInput); // key up event callback

	glutMouseFunc(MouseInput);

	glutSpecialFunc(SpecialKeyDownInput);
	glutSpecialUpFunc(SpecialKeyUpInput);

	g_PrevTime = glutGet(GLUT_ELAPSED_TIME);
	glutTimerFunc(10, RenderScene, 0);

	glutMainLoop();
		
	ScnMgr::GetInstance()->DestroyInstance();

    return 0;
}

