#include "stdafx.h"
#include "Boss.h"
#include "Bullet.h"
#include <math.h>

CBoss::CBoss()
{
}

CBoss::~CBoss()
{
}

void CBoss::LateInit()
{
	if (m_blsInit)
		return;

	m_textID = ScnMgr::GetInstance()->m_Renderer->GenPngTexture("../Texture/BossLeft.png");
	m_textR = ScnMgr::GetInstance()->m_Renderer->GenPngTexture("../Texture/BossRight.png");

	m_iBollDead = ScnMgr::GetInstance()->m_Sound->CreateShortSound("../Sound/BossDeath.wav");
	m_iBossHit = ScnMgr::GetInstance()->m_Sound->CreateShortSound("../Sound/Blood.wav");
	m_iBossJump = ScnMgr::GetInstance()->m_Sound->CreateShortSound("../Sound/Stone.wav");

	m_blsInit = true;


}

void CBoss::Render()
{
	//cout << "���� ����" << endl;

	float x, y, z = 0;
	float sx, sy, sz = 0;
	float r, g, b, a = 0;

	x = m_posX*100.f;
	y = m_posY*100.f;
	z = m_posZ * 100.f;
	sx = m_volX * 100.f;
	sy = m_volY * 100.f;
	sz = m_volZ * 100.f;
	r = m_r;
	g = m_g;
	b = m_b;
	a = m_a;
	int TexId = m_textID;
	if (m_velX > 0.f)
		TexId = m_textR;

	if (textAdd > 3)
		textAdd = 0;

	ScnMgr::GetInstance()->m_Renderer->DrawTextureRectAnim(x, y, z, sx*1.3f, sy*1.3f, sz*1.3f, r, g, b, a, TexId, 4, 3, textAdd, 1, true);

	float persent = (m_healthPoint)*2.5f;

	// ui z�� ����
	ScnMgr::GetInstance()->m_Renderer->DrawSolidRectGauge(0.5f, -250.f, 0.f, 0, 0, 0, 500, 20, 1, 1, 0, 0, 1, persent, false);


}

void CBoss::TracePlayer()
{

	float PlayerPosX = 0.f;
	float PlayerPosY = 0.f;
	float PlayerPosZ = 0.f;
	int t;
	int tt;
	ScnMgr::GetInstance()->m_Obj[0]->GetType(&t);
	ScnMgr::GetInstance()->m_Obj[1]->GetType(&tt);
	//cout << "0�� Ÿ�� -> " << t << endl;
	//cout << "1�� Ÿ�� -> " << tt << endl;

	ScnMgr::GetInstance()->m_Obj[1]->GetPos(&PlayerPosX, &PlayerPosY, &PlayerPosZ);

	float a = PlayerPosX - m_posX;
	float b = PlayerPosY - m_posY;

	float c = sqrtf(a*a + b*b);
	float angle = acosf(a / c);

	if (PlayerPosY > m_posY)
		angle *= -1;

	m_velX = cos(angle);
	m_velY = -sin(angle);

	if (m_fSkillTime > 30.f)
	{
		CreateBullet(m_velX, m_velY);
		m_fSkillTime = 0.f;
	}
	if (m_fJumpTime > 50.f)
	{
		m_eState = JUMP;
		m_velZ = 10.f;
		m_fJumpTime = 0.f;
		ScnMgr::GetInstance()->m_Obj[1]->GetPos(&m_fJumpPosX, &m_fJumpPosY, &m_fJumpPosZ);

		for (int i = 0; i < 10; i++)
		{
			int RandX = rand() % 20 - 10;
			int RandY = rand() % 20 - 10;

			float fRandx = RandX / 10.f;
			float fRandy = RandY / 10.f;
			CreateBullet(fRandx, fRandy);

		}

	}
}

void CBoss::Jumping()
{
	if (m_eState != JUMP)
		return;
    
	float a = m_fJumpPosX - m_posX;
	float b = m_fJumpPosY - m_posY;

	float c = sqrtf(a*a + b*b);
	float angle = acosf(a / c);

	if (m_fJumpPosX > m_posY)
		angle *= -1;

	m_velX = cos(angle);
	m_velY = -sin(angle);

	if (m_posZ <= 0.f)
	{

		m_eState = IDLE;
		ScnMgr::GetInstance()->ShakeCamara();
		ScnMgr::GetInstance()->m_Sound->PlayShortSound(m_iBossJump, false, 0.5);
	}
    
}


void CBoss::CreateBullet(float X, float Y)
{
	float bulletVel = 4.f;
	float vBulletX, vBulletY, vBulletZ;
	vBulletX = vBulletY = vBulletZ = 0.f;

	vBulletX += X;
	vBulletY += Y;

	float vBulletSize = sqrtf(vBulletX*vBulletX + vBulletY * vBulletY + vBulletZ * vBulletZ);

	if (vBulletSize > 0.000001f)
	{
		vBulletX /= vBulletSize;
		vBulletY /= vBulletSize;
		vBulletZ /= vBulletSize;

		vBulletX *= bulletVel;
		vBulletY *= bulletVel;
		vBulletZ *= bulletVel;


		CBullet* pObj = new CBullet;

		int id = ScnMgr::GetInstance()->AddObject(m_posX, m_posY, m_posZ + 0.4f,
			0.25f, 0.25f, 0.25f,
			1, 0, 0, 1,
			vBulletX, vBulletY, vBulletZ,
			0.1f, 0.2f, TYPE_MONSTER_BULLET, 2.f, pObj);

		ScnMgr::GetInstance()->m_Obj[id]->AddForce(vBulletX, vBulletY, vBulletZ, 0.1f);
		ScnMgr::GetInstance()->m_Obj[id]->SetParentObj(this);


	}
}

int CBoss::Update(float elapsedInSec)
{

	LateInit();

	Object::Update(elapsedInSec);

	Jumping();

	if (m_eState == IDLE)
	{
		m_fJumpTime += 0.2f;
		m_fSkillTime += 0.2f;
		TracePlayer();
	}

	
	Collision();

	if (m_healthPoint <= 0)
	{
		ScnMgr::GetInstance()->m_Sound->PlayShortSound(m_iBollDead, false, 1.0);
		ScnMgr::GetInstance()->Ending();
	}


	ScnMgr::GetInstance()->AddRenderGroup(ScnMgr::OBJECT, this);

	return m_blsDead;
}

void CBoss::Collision()
{

	for (int src = 2; src < MAX_OBJ_COUNT; src++)
	{
		if (ScnMgr::GetInstance()->m_Obj[src] != NULL)
		{
			if (ScnMgr::GetInstance()->m_Physics->IsOverlap(this, ScnMgr::GetInstance()->m_Obj[src]))
			{
				int Type = 0;
				ScnMgr::GetInstance()->m_Obj[src]->GetType(&Type);

				

				if (Type == TYPE_BULLET && ScnMgr::GetInstance()->m_Obj[src]->GetDead() == false)
				{
					ScnMgr::GetInstance()->m_Physics->processCollision(this, ScnMgr::GetInstance()->m_Obj[src]);
					m_healthPoint -= 1;
					textAdd++;
					ScnMgr::GetInstance()->m_Obj[src]->lsDead();
					ScnMgr::GetInstance()->m_Sound->PlayShortSound(m_iBossHit, false, 0.5);
				}
			}

		}
	}


}
