#pragma once
#include "stdafx.h"

class Client
{
public:
	Client();
	Client(SOCKET, int, SOCKADDR_IN);
	~Client();

public:
	SOCKET GetSock_TCP() { return m_sock; }
	SOCKADDR_IN GetAddr() { return m_addr; }

	string GetNickname() { return m_nick; }

	int GetID() { return m_id; }
	int GetHP() { return m_hp; }
	
	float GetX() { return m_PosX; }
	float GetY() { return m_PosY; }

	bool GetReady() { return m_ready; }
	bool GetDamage() { return m_damage; }
	bool GetDead() { return m_dead; }

	void SetSock_TCP(SOCKET sock);
	void SetAddr(SOCKADDR_IN addr);
	
	void SetNickname(char nick[]);

	void SetID(int id);
	void SetHP(int hp);

	void SetPos(float x, float y);

	void SetReady(bool ready);
	void SetDamege(bool damage);
	void SetDead(bool dead);

	CS_Move_Packet AddForce(CS_Move_Packet& move_packet, float fX, float fY);

public:
	SOCKET		m_sock;
	SOCKADDR_IN m_addr;

	char		m_nick[32];

	int			m_id;
	int			m_hp;
	
	float		m_PosX;
	float		m_PosY;

	bool		m_ready;
	bool		m_damage;
	bool		m_dead;
};

