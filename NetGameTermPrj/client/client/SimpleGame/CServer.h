#pragma once



class CUDP;
class SERVER {
private:
	//TCP* tcp;
	CUDP* m_pUdp;
	char m_cServerip[15];

	mutex m_lock;
public:
	SERVER();
	virtual ~SERVER();

	void SetState(int);
	int GetState();

	void AddFrame();
	long long GetFrame();

	void GetServerIP();
	int ConnectServer();

	void CreateRecvThread();

	void SendTCPClickMouse(const char& id, const float& x, const float& y);
	void SendUDPMoveMouse(const char& id, const float& x, const float& y);
	void SendTCPClientReady(int id);
	void SendTCPClientUnReady(int id);
	void CreateNick(unsigned char);
	void ClearNick();

	void SendCanUseNicknameClientToServer(const char* nickname);
	//

	void SendMovePacket(char id, STATE state, STATE head);
};

