#pragma once


class CUDP {
	WSAData* m_wsaData;
	SOCKET m_Socket;
	sockaddr_in* m_Serveraddr;

public:
	CUDP();
	~CUDP();

	void err_quit(const char* msg);

	void ConnectUDP(const char* ip);

	void Move(char id, STATE state, STATE head);


};

