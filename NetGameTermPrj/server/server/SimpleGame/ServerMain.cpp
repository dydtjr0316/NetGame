#include "stdafx.h"
#include "CServerFrame.h"

CServerFrame ServerFrame;

int main()
{
	ServerFrame.InitTCPServer();
	ServerFrame.InitUDPServer();


	return 0;
}