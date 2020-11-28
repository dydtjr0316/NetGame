#include "stdafx.h"
#include "CTCP.h"
#define SERVERTCPPORT 15915
#define SERVERUDPPORT 51951
#define SERVERIP "127.0.0.1"

SOCKET CTCP::m_Socket;

CTCP::CTCP()
{
	m_wsaData = new WSAData;
	m_Serveraddr = new sockaddr_in;
}

CTCP::~CTCP()
{
	delete m_wsaData;
	delete m_Serveraddr;
}

void CTCP::err_quit(const char* msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	MessageBox(NULL, (LPCTSTR)lpMsgBuf, (LPCTSTR)msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(1);
}

int CTCP::recvn(unsigned int s, char* buf, int len, int flags)
{
	int received;
	char* ptr = buf;
	int left = len;

	while (left > 0) {
		received = recv(s, ptr, left, flags);

		if (received == SOCKET_ERROR)
			return SOCKET_ERROR;

		else if (received == 0) {
			// std::cout << "Too much Clients connecting" << std::endl;
			closesocket((m_Socket));
			return 0;
		}


		left -= received;
		ptr += received;
	}
	return (len - left);
}

int CTCP::ConnectTCP(const char* ip)
{
	if (WSAStartup(MAKEWORD(2, 2), &(*m_wsaData)) != 0)
		return -1;

	m_Socket = socket(AF_INET, SOCK_STREAM, 0);
	if (m_Socket == INVALID_SOCKET)	err_quit("socket()");

	ZeroMemory(&(*m_Serveraddr), sizeof((*m_Serveraddr)));
	m_Serveraddr->sin_family = AF_INET;
	m_Serveraddr->sin_addr.s_addr = inet_addr(ip);
	m_Serveraddr->sin_port = htons(TCP_SERVERPORT);

	int retval = connect(m_Socket, (SOCKADDR*)&(*m_Serveraddr), sizeof((*m_Serveraddr)));
	if (retval == SOCKET_ERROR)		
		err_quit("connect()");

	cout << "TCP소켓 : " << m_Socket << endl;

	printf("서버 연결: IP 주소=%s, 포트 번호=%d \n",
		inet_ntoa(m_Serveraddr->sin_addr), ntohs(m_Serveraddr->sin_port));


	m_iid = RecvMyID();


	
	cout << "ID : " << m_iid << endl << endl;


	return m_iid;
}

int CTCP::RecvMyID()
{
	if (m_iid < 0) {

		bool IsLogin = true;
		int retval = recvn((SOCKET)m_Socket, (char*)&m_iid, sizeof(int), 0);
		if (retval <= 0) err_quit("[RECV Error] ID \n");
	}

	return m_iid;
}
