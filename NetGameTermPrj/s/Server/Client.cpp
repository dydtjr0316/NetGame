#include "Client.h"

Client::Client()
{
	m_ready = false;
	m_nick = "";
}

Client::Client(SOCKET s, int id, SOCKADDR_IN addr)
{
	m_ready = false;
	m_nick = "";
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
}

void Client::SetAddr(SOCKADDR_IN addr)
{
	m_addr = addr;
}

void Client::SetNickname(string nick)
{
	m_nick = nick;
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
