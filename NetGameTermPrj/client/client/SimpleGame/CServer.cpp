#include "stdafx.h"
#include "CServer.h"
#include "ScnMgr.h"
#include "Player.h"

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
	int id = ConnectTCP(SERVERIP);

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

	//DWORD recvTimeout = 100;  // 0.1초.
	//setsockopt(m_Socket, SOL_SOCKET, SO_RCVTIMEO, (char*)&recvTimeout, sizeof(recvTimeout));

	//u_long socket_switch = 1;
	//int retunvel = ioctlsocket(m_Socket, FIONBIO, &socket_switch);
	//if (retunvel == SOCKET_ERROR)
	//	err_quit("ioctl");

	ZeroMemory(&(m_Serveraddr), sizeof((m_Serveraddr)));
	m_Serveraddr.sin_family = AF_INET;
	m_Serveraddr.sin_addr.s_addr = inet_addr(ip);
	m_Serveraddr.sin_port = htons(TCP_SERVERPORT);
	cout << m_Socket << endl;
	int retval = connect(m_Socket, (SOCKADDR*)&(m_Serveraddr), sizeof((m_Serveraddr)));
	if (retval == SOCKET_ERROR)
		err_quit("connect()");

	cout << "TCP소켓 : " << m_Socket << endl;

	printf("서버 연결: IP 주소=%s, 포트 번호=%d \n",
		inet_ntoa(m_Serveraddr.sin_addr), ntohs(m_Serveraddr.sin_port));


	return RecvMyID();
}

int SERVER::RecvMyID()
{
	int retval = recvn((SOCKET)m_Socket, (char*)&m_id, sizeof(int), 0);
	if (retval <= 0) err_quit(" SERVER::RecvMyID()");

	return m_id;
}

int SERVER::recvn(SOCKET s, char* buf, int len, int flags)
{
	int received;
	char* ptr = buf;
	int left = len;
	while (left > 0) {
		received = recv(s, ptr, left, flags);
		if (received == SOCKET_ERROR)
			return SOCKET_ERROR;
		else if (received == 0)
			break;
		left -= received;
		ptr += received;
	}

	return (len - left);
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

	int retval = recvn(m_Socket, (char*)&packet, sizeof(packet), 0);
	if (retval <= 0) err_quit(" SERVER::SendEnterPacket");

	return packet;
}

void SERVER::SendMovePacket(int id, char packettype, DIR dir, STATE head,float elapsedInSec, float velx, float vely, float mass)
{
	CS_Move_Packet packet;

	packet.size = sizeof(CS_Move_Packet);
	packet.type = CS_PACKET_MOVE;
	packet.id = id;
	packet.dir = dir;
	packet.head = head;
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

	int retval = recvn(m_Socket, (char*)&packet, sizeof(packet), 0);
	if (retval <= 0) err_quit(" SERVER::RecvMovePacket");

	return packet;
}


void SERVER::SendAttackPacket(int id, char type, DIR dir, STATE head_state, float bulltvel)
{
	CS_Attack_Packet attack_packet;

	attack_packet.id = id;
	attack_packet.size = sizeof(CS_Attack_Packet);
	attack_packet.type = type;
	attack_packet.dir = dir;
	attack_packet.head = head_state;
	attack_packet.bulletvel = bulltvel;

	int retval = send(m_Socket, (char*)&attack_packet, sizeof(CS_Attack_Packet), 0);

	if (retval == SOCKET_ERROR)err_quit(" SERVER::SendMovePacket");

	cout << attack_packet.bulletvel << endl;
}

SC_Attack_Packet SERVER::RecvAttackPacket()
{
	SC_Attack_Packet packet;

	int retval = recvn(m_Socket, (char*)&packet, sizeof(SC_Attack_Packet), 0);
	if (retval <= 0) err_quit(" SERVER::RecvMovePacket");

	return packet;
}