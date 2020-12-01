#include "stdafx.h"
#include "CServer.h"

SOCKET SERVER::m_Socket;

SERVER::SERVER()
{

}

SERVER::~SERVER()
{
	closesocket(m_Socket);
	WSACleanup();
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

int SERVER::ConnectTCP(const char* ip)
{
	if (WSAStartup(MAKEWORD(2, 2), &(m_wsaData)) != 0)
		return -1;

	m_Socket = socket(AF_INET, SOCK_STREAM, 0);
	if (m_Socket == INVALID_SOCKET)	err_quit("socket()");

	ZeroMemory(&(m_Serveraddr), sizeof((m_Serveraddr)));
	m_Serveraddr.sin_family = AF_INET;
	m_Serveraddr.sin_addr.s_addr = inet_addr(ip);
	m_Serveraddr.sin_port = htons(TCP_SERVERPORT);

	int retval = connect(m_Socket, (SOCKADDR*)&(m_Serveraddr), sizeof((m_Serveraddr)));
	if (retval == SOCKET_ERROR)
		err_quit("connect()");

	cout << "TCP소켓 : " << m_Socket << endl;

	printf("서버 연결: IP 주소=%s, 포트 번호=%d \n",
		inet_ntoa(m_Serveraddr.sin_addr), ntohs(m_Serveraddr.sin_port));

	m_id = RecvMyID();

	cout << m_id << endl;

	cout << "ID : " << m_id << endl << endl;

	return m_id;
}

int SERVER::RecvMyID()
{
	int retval = recv((SOCKET)m_Socket, (char*)&m_id, sizeof(int), 0);
	if (retval <= 0) err_quit(" SERVER::RecvMyID()");

	return m_id;
}

void SERVER::SendLoginPacket(int id, char nickname[])
{
	CS_Client_Login_Packet packet;

	packet.size = sizeof(CS_Client_Login_Packet);
	packet.type = ENTER_USER;
	packet.id = id;
	strcpy_s(packet.nickname, nickname);

	int retval = send(m_Socket, (char*)&packet, sizeof(CS_Client_Login_Packet), 0);

	if (retval == SOCKET_ERROR)err_quit(" SERVER::SendEnterPacket");

	cout << "send enter packet (" << packet.id << ", " << packet.nickname << ")" << endl;
}

SC_Client_Enter_Packet SERVER::RecvEnterPacket()
{
	SC_Client_Enter_Packet packet;

	int retval = recv(m_Socket, (char*)&packet, sizeof(packet), 0);
	if (retval <= 0) err_quit(" SERVER::SendEnterPacket");

	return packet;
}

void SERVER::SendMovePacket(int id, float x, float y, STATE type, float elapsedInSec, float velx, float vely, float mass)
{
	CS_Move_Packet packet;

	packet.size = sizeof(CS_Move_Packet);
	packet.id = id;
	packet.x = x;
	packet.y = y;
	packet.type = type;
	packet.elapsedInSec = elapsedInSec;
	packet.velx = velx;
	packet.vely = vely;
	packet.mass = mass;

	int retval = send(m_Socket, (char*)&packet, sizeof(CS_Move_Packet), 0);

	if (retval == SOCKET_ERROR)err_quit(" SERVER::SendMovePacket");

	//cout << "send move packet " << packet.id << endl;
}

SC_Move_Packet SERVER::RecvMovePacket()
{
	SC_Move_Packet packet;

	int retval = recv(m_Socket, (char*)&packet, sizeof(packet), 0);
	if (retval <= 0) err_quit(" SERVER::RecvMovePacket");

	return packet;
}

void SERVER::SendAttackPacket(int id, STATE type)
{
	CS_Attack_Packet packet;

	packet.size = sizeof(CS_Attack_Packet);
	packet.type = type;
	packet.id = id;



	int retval = send(m_Socket, (char*)&packet, sizeof(CS_Attack_Packet), 0);

	if (retval == SOCKET_ERROR)err_quit(" SERVER::SendMovePacket");
}

SC_Attack_Packet SERVER::RecvAttackPacket()
{
	SC_Attack_Packet packet;

	int retval = recv(m_Socket, (char*)&packet, sizeof(SC_Attack_Packet), 0);
	if (retval <= 0) err_quit(" SERVER::RecvMovePacket");

	cout << "받을 때 : "<< packet.velx << " , " << packet.vely << " , " << packet.velz << endl;
	
	return packet;
}