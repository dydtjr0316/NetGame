#pragma once

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

