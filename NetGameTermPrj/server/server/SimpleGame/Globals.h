#pragma once

// 최대 Obj 수
#define MAX_OBJ_COUNT 1000

// 플레이어 번호
#define HERO_ID 800

// 물리
#define GRAVITY 9.8f

// Object 속성
#define TYPE_NORMAL 0
#define TYPE_BULLET 1
#define TYPE_UI  2
#define TYPE_MONSTER 3
#define TYPE_ITEM 4

// Server 
#define TCP_SERVERPORT 53635
#define UDP_SERVERPORT 65356
#define SERVERIP "192.168.123.25"

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

// 화면 크기
#define WIDTH 800
#define HEIGHT 600