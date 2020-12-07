#pragma Once 
#include "Object.h"

class CBoss : public Object
{
public:
	enum STATE {IDLE,ATTACK,JUMP,DEAD};

public:
	CBoss();
	virtual ~CBoss();

	void LateInit();
	void Render();
	void CreateBullet(float X, float Y);

	void Jumping();
	void TracePlayer();
	int Update(float elapsedInSec);
	void Collision();

private:
	float m_fSkillTime = 0.f;
	float m_fJumpTime = 0.f;
	bool m_blsInit = false;
	int m_textR = -1;
	int textAdd = 0;

	int m_iBossHit = -1;
	int m_iBossJump = -1;
	int m_iBollDead = -1;

	STATE m_eState = IDLE;

	//server
	float m_fJumpPosX = 0.f;
	float m_fJumpPosY = 0.f;
	float m_fJumpPosZ = 0.f;
};