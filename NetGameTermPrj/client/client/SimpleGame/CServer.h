#pragma once

class SERVER {
// server
private:
	char m_cServerip[15];

	mutex m_lock;

// tcp
private:
	WSAData m_wsaData;
	static SOCKET m_Socket;
	sockaddr_in m_Serveraddr;
	int m_id = 0;


public:
	// server
	SERVER();
	virtual ~SERVER();
	int ConnectServer();

	SOCKET GetSock() { return m_Socket; }
	int GetID() { return m_id; }

	// tcp
	static void err_quit(const char*);

	int ConnectTCP(const char*);
	int RecvMyID();

	void SendLoginPacket(int id, char nickname[]);
	SC_Client_Enter_Packet RecvEnterPacket();

	void SendMovePacket(int id, float x, float y, STATE type, float elapsedInSec, float velx, float vely, float mass);
	SC_Move_Packet RecvMovePacket();

	void SendAttackPacket(int id, STATE type);
	SC_Attack_Packet RecvAttackPacket();
};

