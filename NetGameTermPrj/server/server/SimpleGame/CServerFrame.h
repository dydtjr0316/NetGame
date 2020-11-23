#pragma once
#include "stdafx.h"
#include "CError.h"
#include "CClient.h"
#include "Player.h"
#include "Protocol.h"
#include <unordered_map>

class CError;
class CPlayer;
class CServerFrame
{
public:
	// 생성 소멸자
	CServerFrame();
	~CServerFrame();

public:
	int InitTCPServer(); // 리턴값 1
	int InitUDPServer(); // 리턴값 1

	void LoginServer();

	void UDP_Socket();
	void UpdateMovePos();

	static DWORD WINAPI Process(LPVOID arg);

	void SaveNickName();
	void sendNickName();

	void UpdateStatus();
	void SendAllStatus();

	void IsAllReady();

	void BulletRoute();
	
	void Collision();

	void UpdatePosition();

private:
	SOCKET m_ListenSock;
	static SOCKET m_UDP_Sock;
	static CError* m_Error;

	static std::unordered_map<int, CClient> m_mClients;

	HANDLE m_hClientsThreads[2];
};

