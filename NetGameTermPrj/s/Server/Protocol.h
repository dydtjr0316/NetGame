#pragma once

// Login
#define NICKNAME_ADD 11
#define NICKNAME_USE 12
#define NICKNAME_UNUSE 13
#define MAX_USER 14

// Lobby
#define CLIENT_ISREADY 21
#define CLIENT_ISNOTREDAY 22
#define CLIENT_ALLREADY 23

// InGame
#define INGAME_RUNNIG 311
#define INGAME_CLEAR 312
#define INGAME_OVER 313

#define MOVE_UP 321
#define MOVE_DOWN 322
#define MOVE_LEFT 323
#define MOVE_RIGHT 324

#define SHOOT_UP 331
#define SHOOT_DOWN 332
#define SHOOT_LEFT 333
#define SHOOT_RIGHT 334

enum STATE { IDLE, DOWN, UP, RIGHT, LEFT };
enum ITEM { NONE, TRIPLE };

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
	char nickname[2][32];
};

// Move
struct CS_Move_Packet
{
	char size;
	char type;
	int id;
	float x, y;
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