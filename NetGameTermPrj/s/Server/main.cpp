#include "stdafx.h"
#include "ServerFrame.h"

ServerFrame server;

int main()
{
	server.InitTCPServer();

	server.CreateMoveThread();

	server.LoginServer();

	//server.InGameServer();

	return 0;
}