#pragma once
#pragma pack (push, 1)

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
	char nickname[32];
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
	char nickname[2][32];
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
	char nickname[2][32];
};

// Move
struct CS_Move_Packet
{
	char size;
	char type;
	int id;
	char dir;
};

struct SC_Move_Packet
{
	char size;
	char type;
	int id;
	float hp;
	short x, y;
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
	short x, y;
};

// Boss
struct CS_Boss_Move_Packet
{
	char size;
	char type;
	float hp;
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
	short x, y;
};

struct SC_Boss_Attack_Packet
{
	char size;
	char type;
	int id;
	short x, y;
};

#pragma pack (pop)