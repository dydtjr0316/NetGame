#pragma 
#include "Object.h"

class CPlayer :public Object
{
public:
	enum STATE { IDLE,DOWN, UP, RIGHT, LEFT };
	enum ITEM {NONE,TRIPLE};
public:
	CPlayer();
	~CPlayer();

public:
	virtual int Update(float elapsedInSec);
	void Shooting();
	void KeyInput(float elapsedInSec);
	void CollisionCheck();

	void DrawHead();
	void LateInit();
	virtual void Render();
private:
	float m_MaxFrame = 0.f;
	float m_Frame = 0.f;
	STATE m_CurState = IDLE;
	STATE m_Head = IDLE;
private:
	bool m_blsInit = false;
	// client
	int m_texR = -1;
	int m_texL = -1;
	int m_texUD = -1;
	bool m_blsCanShoot = false;
	float m_DamageCooltime = 0.f;

	// server
	bool m_blsDamaged = false;
	int m_iFlickCount = 0;

private:
	int m_iBulletSound = -1;
	int m_iDamageSound = -1;

};