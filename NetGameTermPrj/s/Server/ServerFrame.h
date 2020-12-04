#pragma once
#include "stdafx.h"
#include "Client.h"

class ServerFrame
{
public:
	ServerFrame();
	~ServerFrame();

public:
	static void err_display(const char* msg);
	static int recvn(SOCKET s, char* buf, int len, int flags);

	int InitTCPServer();

	void LoginServer();
	static void Send_enter_packet(int to, int id);

	void LobbyServer(int id);
	void InGameServer();

	static DWORD WINAPI Process(LPVOID arg);

	void CreateMoveThread(int id);
	static DWORD WINAPI MOVEThread(LPVOID arg);
	static void UpdateMove(int id);

	void CreateAttackThread(int id);
	static DWORD WINAPI AttackThread(LPVOID arg);
	static void UpdateAttack(int id);

	void UpdateStatus();
	void SendAllStatus();
	void IsAllReady();

	void UpdateCollision();
	void UpdateBoss();
	CS_Move_Packet AddForce(CS_Move_Packet& move_packet);

private:
	SOCKET m_sock;
	
	HANDLE m_hCThreads[2];
	HANDLE m_MOVEThread[2];
	HANDLE m_AttackThread[2];

	static unordered_map<int, Client> m_Clients;
	static int m_id;
};