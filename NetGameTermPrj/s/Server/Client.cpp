#include "Client.h"

Client::Client()
{
	m_ready = false;
	strcpy_s(m_nick, "NONE");
}

Client::Client(SOCKET s, int id, SOCKADDR_IN addr)
{
	m_ready = false;
	strcpy_s(m_nick, "NONE");
	m_sock = s;
	m_id = id;
	m_addr = addr;
}

Client::~Client()
{

}

void Client::SetSock_TCP(SOCKET sock)
{
	m_sock = sock;
	//DWORD recvTimeout = 100;  // 0.1ÃÊ.
	//setsockopt(m_sock, SOL_SOCKET, SO_RCVTIMEO, (char*)&recvTimeout, sizeof(recvTimeout));

}

void Client::SetAddr(SOCKADDR_IN addr)
{
	m_addr = addr;
}

void Client::SetNickname(char nick[])
{
	strcpy_s(m_nick, nick);
}

void Client::SetID(int id)
{
	m_id = id;
}

void Client::SetHP(int hp)
{
	m_hp = hp;
}

void Client::SetPos(float x, float y)
{
	m_PosX = x;
	m_PosY = y;
}

void Client::SetReady(bool ready)
{
	m_ready = ready;
}

void Client::SetDamege(bool damage)
{
	m_damage = damage;
}

void Client::SetDead(bool dead)
{
	m_dead = dead;
}
CS_Move_Packet Client::AddForce(CS_Move_Packet& move_packet, float fX, float fY)
{
	float fAmount = 20.f;
	float fSize = 0.f;

	fSize = sqrtf(fX * fX + fY * fY);

	if (fSize > FLT_EPSILON)
	{
		fX /= fSize;
		fY /= fSize;
		fX *= fAmount;
		fY *= fAmount;

		float accX, accY, accZ;
		accX = accY = accZ = 0.f;

		accX = fX / move_packet.mass;
		accY = fY / move_packet.mass;

		move_packet.velx = move_packet.velx + accX * move_packet.elapsedInSec;
		move_packet.vely = move_packet.vely + accY * move_packet.elapsedInSec;

	}
	CS_Move_Packet temp = move_packet;

	return temp;

}