#include "stdafx.h"
#include "CServerFrame.h"

CServerFrame ServerFrame;

int main()
{
	ServerFrame.InitTCPServer();
	ServerFrame.InitUDPServer();

	ServerFrame.UDP_Socket();
	//ServerFrame.LoginServer();

	return 0;
}