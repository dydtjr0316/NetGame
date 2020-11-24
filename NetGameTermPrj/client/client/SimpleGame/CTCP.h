#pragma once


class CTCP {
	WSAData* m_wsaData;
	static SOCKET m_Socket;
	sockaddr_in* m_Serveraddr;
	int m_iid =-1;



	//static PacketFrameState framepacket;
	//static PacketReadyStateServerToClient lobbypacket;
	//static PacketLoginOK loginpacket;

	//long long remainingCoolTime;
	//long long defaultCoolTime;

public:
	CTCP();
	~CTCP();

	static void err_quit(const char*);
	static int recvn(unsigned int, char*, int, int);

	int ConnectTCP(const char*);
	int RecvMyID();

	void ClickMouse(char, float, float);

	void CreateRecvThread();
	static unsigned long __stdcall RecvDataThread(void*);

	static void UpdateServerFrame();
	static void UpdateLobby();
	static void UpdateLogin();

	// Lobby
	void SendReadyClientToServer(int);
	void SendUnReadyClientToServer(int);
	static void SendReadyClientsOK();

	// Login
	void SendTCPCanUseNicknameClientToServer(const char*);
	//PacketLoginOK RecvNickOK();
	//PacketALLNick RecvTCPClientsNicknameClientToServer();

	// InGame
	static void AddParticle(float, float, float, float, float, float);
	static int FindSlotParticleObject();
};
