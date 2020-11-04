#pragma once

class CError;
class CPlayer;
class CServerFrame
{
public:
	// ���� �Ҹ���
	CServerFrame();
	~CServerFrame();

public:
	int InitTCPServer(); // ���ϰ� 1
	int InitUDPServer(); // ���ϰ� 1

	static DWORD WINAPI UDP_Thread(LPVOID arg);
	static void UpdateMovePos();

	void LoginServer();

private:
	SOCKET m_ListenSock;
	static SOCKET m_UDP_Sock;
	static CError* m_Error;

	unordered_map<int, CPlayer> m_mClients;

	HANDLE m_hClientsThreads[2];
};

