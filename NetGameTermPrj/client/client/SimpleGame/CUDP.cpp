#include "stdafx.h"
#include "CUDP.h"

// 지우기
// 지우기

SOCKET CUDP::m_Socket;
//#include "Protocol.h"
CUDP::CUDP()
{
	m_wsaData = new WSAData;
	m_Serveraddr = new sockaddr_in;
	
}

CUDP::~CUDP()
{
	delete m_wsaData;
	delete m_Serveraddr;
}

void CUDP::err_quit(const char* msg)
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

void CUDP::ConnectUDP(const char* ip)
{
	if (WSAStartup(MAKEWORD(2, 2), &(*m_wsaData)) != 0)
		return;

	m_Socket = socket(AF_INET, SOCK_DGRAM, 0);
	if (m_Socket == INVALID_SOCKET)	err_quit("[UDP Socket Error]");

	ZeroMemory(&(*m_Serveraddr), sizeof((*m_Serveraddr)));
	m_Serveraddr->sin_family = AF_INET;
	m_Serveraddr->sin_addr.s_addr = inet_addr(ip);
	m_Serveraddr->sin_port = htons(UDP_SERVERPORT);
	int retval = connect(m_Socket, (SOCKADDR*)&(*m_Serveraddr), sizeof((*m_Serveraddr)));
	if (retval == SOCKET_ERROR)		err_quit("[UDP Connect Error]");


	cout << "UDP소켓 : " << m_Socket << endl;
	printf("서버 연결: IP 주소=%s, 포트 번호=%d \n",
		inet_ntoa(m_Serveraddr->sin_addr), ntohs(m_Serveraddr->sin_port));

	cout << endl;

}

void CUDP::Move(char id, STATE state, STATE head)
{
	CS_Move_Packet packet;

	packet.size = sizeof(CS_Move_Packet);
	packet.id = id;
	packet.state = state;
	packet.head = head;
	// 여기하던중
	int retval = send(m_Socket, (char*)&packet, sizeof(CS_Move_Packet), 0);
	if (retval == SOCKET_ERROR)		
		err_quit("[여기니?]");

	cout << "보낸다?" << endl;
}
