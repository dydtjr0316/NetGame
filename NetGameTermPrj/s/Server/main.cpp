#include "stdafx.h"
#include "ServerFrame.h"

ServerFrame server;

int main()
{
	server.InitTCPServer();
	server.LoginServer();

	return 0;
}