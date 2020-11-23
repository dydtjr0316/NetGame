#include "stdafx.h"
#include "CClient.h"



CClient::CClient()
{
	m_bisReady = false;
	m_cNickName = "";
}

CClient::CClient(SOCKET s, int id, SOCKADDR_IN addr)
{
	m_bisReady = false;
	m_cNickName = "NONE";
	m_TcpSocket = s;
	m_addr = addr;
	m_PlayerId = id;
}

CClient::~CClient()
{

}

void CClient::SetSocket_TCP(SOCKET Socket)
{
	m_TcpSocket = Socket;
}

void CClient::SetSocket_UDP(SOCKET Socket)
{
	m_UdpSocket = Socket;
}

void CClient::SetNickName(char* NickName)
{
	for (int i = 0; i < 32; i++)
	{
		m_cNickName[i] = NickName[i];
	}
}

void CClient::SetPlayerHp(int hp)
{
	m_iPlayerHp = hp;
}

void CClient::SetPos(float x, float y)
{
	m_fPosX = x;
	m_fPosY = y;
}

void CClient::SetIsReady(bool ready)
{
	m_bisReady = ready;
}

void CClient::SetIsDamaged(bool damaged)
{
	m_bisDamaged = damaged;
}

void CClient::SetIsPlayerDead(bool dead)
{
	m_bisPlayerDead = dead;
}

void CClient::SetIsBossDead(bool dead)
{
	m_bisBossDead = dead;
}