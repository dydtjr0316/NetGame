#pragma once

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
constexpr char CS_PACKET_ATTACK = 101;


// Login
constexpr char ENTER_USER = 5;
constexpr char ID_USE = 6;

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
};

struct SC_Client_LoginOK_Packet
{
	char size;
	char type;
};

struct SC_Client_Enter_Packet
{
	char size;
	char type;
	int id;
	float posX, posY;
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
};

// Attack
struct CS_Attack_Packet
{
	char size;
	char type;
	int id;
	DIR dir;
	STATE head;
	float bulletvel;
};

struct SC_Attack_Packet
{
	char size;
	char type;
	int id;
	float bulletx, bullety, bulletz, bulletsize;
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