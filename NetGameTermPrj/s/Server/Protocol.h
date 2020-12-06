#pragma once

// InGame
//#define INGAME_RUNNIG 311
//#define INGAME_CLEAR 312
//#define INGAME_OVER 313

// enum으로 사용
//#define MOVE_UP 321
//#define MOVE_DOWN 322
//#define MOVE_LEFT 323
//#define MOVE_RIGHT 324
//
//#define SHOOT_UP 331
//#define SHOOT_DOWN 332
//#define SHOOT_LEFT 333
//#define SHOOT_RIGHT 334

enum STATE { IDLE, DOWN, UP, RIGHT, LEFT };
enum ITEM { NONE, TRIPLE };

enum class DIR{NONE,LEFT,RIGHT,UP,DOWN};

#pragma pack (push, 1)

constexpr char SC_PACKET_MOVE = 0;
constexpr char SC_PACKET_ENTER = 1;
constexpr char SC_PACKET_LOGIN = 2;
constexpr char SC_PACKET_LEAVE = 3;
constexpr char SC_PACKET_ATTACK = 4;

constexpr char CS_PACKET_MOVE = 100;

// Login
constexpr char ENTER_USER = 5;
constexpr char NICKNAME_USE = 6;

// Lobby
constexpr char CLIENT_ISREADY = 7;
constexpr char CLIENT_ISNOTREDAY = 8;
constexpr char CLIENT_ALLREADY = 9;

// Login
struct CS_Client_Login_Packet
{
	char size;
	char type;
	int id;
	char nickname[32];
};

struct SC_Client_LoginOK_Packet
{
	char size;
	char type;
	char nickname[32];
};

struct SC_Client_Enter_Packet
{
	char size;
	char type;
	int id;
	float posX, posY;
	char nickname[32];
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
	char id;
};

// Move
struct CS_Move_Packet
{
	char size;
	char type;
	int id;
	DIR dir;
	STATE head;
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
	STATE curstate;
	STATE head;
	float x, y;
	//bool boss;
};

// Attack
struct CS_Attack_Packet
{
	char size;
	STATE type;
	int id;
};

struct SC_Attack_Packet
{
	char size;
	char type;
	int id;
	float velx, vely, velz;
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