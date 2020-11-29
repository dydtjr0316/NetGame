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
	static void Send_enter_packet(int to, int id);
	static void Send_pakcet(int id, void* p);

	void LobbyServer(int id);
	void InGameServer();

	static DWORD WINAPI Process(LPVOID arg);

	void CreateMoveThread(int id);
	static DWORD WINAPI MOVEThread(LPVOID arg);
	static void UpdateMove(int id);
	static void UpdateAttack(int id);

	void UpdateStatus();
	void SendAllStatus();

	void IsAllReady();

	void SendBulletRoute();
	void UpdateCollision();
	void UpdateBoss();
	CS_Move_Packet AddForce(CS_Move_Packet& move_packet);

private:
	SOCKET m_sock;
	
	HANDLE m_hCThreads[2];
	HANDLE m_MOVEThread;

	static unordered_map<int, Client> m_Clients;

	static int m_id;
	static SC_Client_RedayOK_Packet m_ready;
};