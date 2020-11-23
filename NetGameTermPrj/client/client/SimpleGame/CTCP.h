#pragma once


class CTCP {
	WSAData* wsa;
	static unsigned int sock;
	sockaddr_in* serveraddr;
	//static PacketFrameState framepacket;
	//static PacketReadyStateServerToClient lobbypacket;
	//static PacketLoginOK loginpacket;

	long long remainingCoolTime;
	long long defaultCoolTime;

public:
	CTCP();
	~CTCP();

	static void err_quit(const char*);
	static int recvn(unsigned int, char*, int, int);

	int ConnectTCPToServer(const char*);
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
