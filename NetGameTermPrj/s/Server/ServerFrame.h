#pragma once
#include "stdafx.h"
#include "Client.h"

class ServerFrame
{
public:
	ServerFrame();
	~ServerFrame();

public:
	static void err_quit(const char* msg);
	static void err_display(const char* msg);

	int InitTCPServer();

	void LoginServer();
	void LobbyServer(int id);
	void InGameServer();

	static DWORD WINAPI Process(LPVOID arg);

	void CreateMoveThread(int id);
	static DWORD WINAPI MOVEThread(LPVOID arg);
	static void UpdateMove(int id);

	//void SaveNickName();
	//void SendNickName();

	void UpdateStatus();
	void SendAllStatus();

	void IsAllReady();

	void SendBulletRoute();
	void UpdateCollision();
	void UpdateBoss();

private:
	SOCKET m_sock;
	
	HANDLE m_hCThreads[2];
	HANDLE m_MOVEThread;

	static unordered_map<int, Client> m_Clients;

	static int m_id;
	static set<string> m_nick;
	static SC_Client_RedayOK_Packet m_ready;
};