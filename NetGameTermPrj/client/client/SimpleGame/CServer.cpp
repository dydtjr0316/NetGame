#include "stdafx.h"
#include "CServer.h"

SERVER::SERVER()
{

}

SERVER::~SERVER()
{
}

int SERVER::ConnectServer()
{
	int id = ConnectTCP("127.0.0.1");

	return id;
}


void SERVER::err_quit(const char* msg)
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

int SERVER::recvn(unsigned int s, char* buf, int len, int flags)
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

int SERVER::ConnectTCP(const char* ip)
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


	m_id = RecvMyID();

	cout << "ID : " << m_id << endl << endl;

	return m_id;
}

int SERVER::RecvMyID()
{

	int retval = recvn((SOCKET)m_Socket, (char*)&m_id, sizeof(int), 0);
	if (retval <= 0) err_quit(" SERVER::RecvMyID()");

	return m_id;
}