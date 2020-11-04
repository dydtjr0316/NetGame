#pragma once
struct Player_Packet
{
	char size;
	int id;
	float PosX, posY, PosZ;
	float hp;
};

struct Boss_Packet
{
	char size;
	int id;
	float PosX, posY, PosZ;
	float hp;
};

struct Bullet_Packet
{
	char size;
	int id;
	float PosX, posY, PosZ;
	bool bisDead;
};

struct LoginOk_Packet
{
	char size;
	char type;
};
struct Login_Packet
{
	char size;
	char type;
	char Nick[3][16];
};

struct Ready_Packet
{
	int id;
	bool bisReady;
};