#include "stdafx.h"
#include "Door.h"

CDoor::CDoor()
{
}

CDoor::~CDoor()
{
}

void CDoor::LateInit()
{
	m_blsInit = true;
	m_textID = ScnMgr::GetInstance()->m_Renderer->GenPngTexture("../Texture/Door.png");
	m_iMiniMap= ScnMgr::GetInstance()->m_Renderer->GenPngTexture("../Texture/MiniMap.png");
}

void CDoor::Render()
{

	float x, y, z = 0;
	float sx, sy, sz = 0;
	float r, g, b, a = 0;

	x = m_posX * 100.f;
	y = m_posY * 100.f;
	z = m_posZ * 100.f;
	sx = m_volX * 100.f;
	sy = m_volY * 100.f;
	sz = m_volZ * 100.f;
	r = m_r;
	g = m_g;
	b = m_b;
	a = m_a;
	int yy = 0;
	if (ScnMgr::GetInstance()->m_eCurStage == ScnMgr::START)
		yy = 5;
	else if (ScnMgr::GetInstance()->m_eCurStage == ScnMgr::BOSS)
		yy = 9;


	ScnMgr::GetInstance()->m_Renderer->DrawTextureRectAnim(-250, 200, 0, sx, sy, sz, r, g, b, a, m_iMiniMap, 1, 10, 0, yy, true);

	DrawDoor();
}

int CDoor::Update(float elapsedInSec)
{
	if (m_blsInit == false)
		LateInit();


	m_blsCheckMonster = true;

	for (int src = 0; src < MAX_OBJ_COUNT; src++)
	{
		if (ScnMgr::GetInstance()->m_Obj[src] != NULL)
		{
			
				int Type = 0;
				ScnMgr::GetInstance()->m_Obj[src]->GetType(&Type);

				if (Type == TYPE_MONSTER)
				{
					m_blsCheckMonster = false;

				}
			

		}
	}

	CheckDoor();



	if (m_blsDead == false)
		ScnMgr::GetInstance()->AddRenderGroup(ScnMgr::OBJECT, this);


	if (ScnMgr::GetInstance()->m_eCurStage == ScnMgr::BOSS &&m_blsCheckMonster == false)
		m_fEndingTime += 0.1f;


	return m_blsDead;
}

void CDoor::DrawDoor()
{

	float x, y, z = 0;
	float sx, sy, sz = 0;
	float r, g, b, a = 0;

	x = m_posX * 100.f;
	y = m_posY * 100.f;
	z = m_posZ * 100.f;
	sx = m_volX * 100.f;
	sy = m_volY * 100.f;
	sz = m_volZ * 100.f;
	r = m_r;
	g = m_g;
	b = m_b;
	a = m_a;
	
	if(ScnMgr::GetInstance()->m_eCurStage == ScnMgr::START)
	ScnMgr::GetInstance()->m_Renderer->DrawTextureRectAnim(0,130, z, sx, sy, sz, r, g, b, a, m_textID, 2, 4, m_blsCheckMonster, 0, true);

}

void CDoor::CheckDoor()
{
	if (m_blsCheckMonster == false)
		return;

	float PlayerX, PlayerY, PlayerZ;

	ScnMgr::GetInstance()->m_Obj[HERO_ID]->GetPos(&PlayerX, &PlayerY, &PlayerZ);

	PlayerX *= 100;
	PlayerY *= 100;
	PlayerZ *= 100;

	if (ScnMgr::GetInstance()->m_eCurStage == ScnMgr::START)
	{
		if (PlayerX > -30 && PlayerX < 30 && PlayerY>118)
		{
			ScnMgr::GetInstance()->CreateBoss();
			ScnMgr::GetInstance()->m_Obj[HERO_ID]->SetPos(PlayerX / 100, -230 / 100, PlayerZ/100);
			ScnMgr::GetInstance()->m_Obj[HERO_ID]->SetVel(0.f, 0.f, 0.f);
			m_blsCheckMonster = false;
		}
	}
}
