#pragma once
#pragma pack (push, 1)

constexpr char SC_PACKET_MOVE = 0;
constexpr char SC_PACKET_ENTER = 1;
constexpr char SC_PACKET_LOGIN = 2;
constexpr char SC_PACKET_LEAVE = 3;

// Logout
struct CS_Client_Logout_Packet
{
	char size;
	char type;
	char id;
};

// Login
struct CS_Client_Login_Packet
{
	char size;
	char type;
	char nickname;
};

struct SC_Client_LoginOK_Packet
{
	char size;
	char type;
};

struct SC_All_Client_Packet
{
	char size;
	char type;
	string nickname[2];
};

// Lobby
struct CS_Client_Ready_Packet
{
	char size;
	char type;
	char id;
};

struct SC_Client_RedayOK_Packet
{
	char size;
	char type;
	char id[2];
	string nickname[2];
};

// Move
struct CS_Move_Packet
{
	char size;
	STATE type;
	int id;
	float x, y;
	float elapsedInSec;
	float velx, vely;
	float mass;
};

struct SC_Move_Packet
{
	char size;
	char type;
	int id;
	float x, y;
};

// Attack
struct CS_Attack_Packet
{
	char size;
	char type;
};

struct SC_Attack_Packet
{
	char size;
	char type;
	int id;
	float x, y;
};

// Boss
struct CS_Boss_Move_Packet
{
	char size;
	char type;
};

struct CS_Boss_Attack_Packet
{
	char size;
	char type;
};

struct SC_Boss_Move_Packet
{
	char size;
	char type;
	int id;
	float hp;
	float x, y;
};

struct SC_Boss_Attack_Packet
{
	char size;
	char type;
	int id;
	float x, y;
};

#pragma pack (pop)