#pragma once

class SERVER {
// server
private:
	char m_cServerip[15];

	mutex m_lock;

// tcp
private:
	WSAData* m_wsaData;
	static SOCKET m_Socket;
	sockaddr_in* m_Serveraddr;
	int m_id = 0;


public:
	// server
	SERVER();
	virtual ~SERVER();
	int ConnectServer();
	int GetID() { return m_id; }

	// tcp
	static void err_quit(const char*);
	static int recvn(unsigned int, char*, int, int);

	int ConnectTCP(const char*);
	int RecvMyID();


	void SendMovePacket(char id, float x, float y, char type);
	CS_Move_Packet RecvMovePacket();
};

