#include "stdafx.h"
#include "Door.h"
#include "Object.h"

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
	if(ScnMgr::GetInstance()->m_eCurStage == ScnMgr::BOSS)
		cout << "보스 -> 문 랜더" << endl;

	if (ScnMgr::GetInstance()->m_eCurStage == ScnMgr::START)
		cout << "야이 시발 -> 문 랜더" << endl;

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

	for (int src = 2; src < MAX_OBJ_COUNT; src++)
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
	if (ScnMgr::GetInstance()->m_eCurStage == ScnMgr::BOSS)
	{
		ScnMgr::GetInstance()->m_Renderer->DrawTextureRectAnim(0, 130, z, sx, sy, sz, r, g, b, a, m_textID, 2, 4, false, 0, true);

	}

}

void CDoor::CheckDoor()
{
	if (m_blsCheckMonster == false)
		return;
	//if (m_id != 0 && m_id != 1)return;

	float PlayerX, PlayerY, PlayerZ;

	////ScnMgr::GetInstance()->m_Obj[HERO_ID]->GetPos(&PlayerX, &PlayerY, &PlayerZ);
	//ScnMgr::GetInstance()->m_Obj[m_id]->GetPos(&PlayerX, &PlayerY, &PlayerZ);

	for (int i = 0; i < 2; ++i)
	{
		float PlayerX = 0.5f;
		float PlayerY = 0.5f;
		float PlayerZ = 0.5f;
		//ScnMgr::GetInstance()->m_Obj[HERO_ID]->GetPos(&PlayerX, &PlayerY, &PlayerZ);
		if (bcnt == 1 && i == 0)continue;

		ScnMgr::GetInstance()->m_Obj[i]->GetPos(&PlayerX, &PlayerY, &PlayerZ);

		if (ScnMgr::GetInstance()->m_eCurStage == ScnMgr::START)
		{
			PlayerX *= 100;
			PlayerY *= 100;
			PlayerZ *= 100;
			//if (PlayerX > -30 && PlayerX < 30 && PlayerY>118)
			//{
			//	ScnMgr::GetInstance()->CreateBoss();
			//	//ScnMgr::GetInstance()->m_Obj[HERO_ID]->SetPos(PlayerX / 100, -230 / 100, PlayerZ/100);
			//	//ScnMgr::GetInstance()->m_Obj[HERO_ID]->SetVel(0.f, 0.f, 0.f);
			//	m_blsCheckMonster = false;
			//}
			if (PlayerX > -30 && PlayerX < 30 && PlayerY>118)
			{
				bcnt++;
				if (bcnt == 2)
				{
					/*for (int j = 0; j < 2; ++j) {*/
					ScnMgr::GetInstance()->m_Obj[0]->SetPos(PlayerX / 100, -230 / 100, PlayerZ / 100);
					ScnMgr::GetInstance()->m_Obj[0]->SetVel(0.f, 0.f, 0.f);
					ScnMgr::GetInstance()->m_Obj[1]->SetPos(PlayerX / 100 + 0.2f, -230 / 100, PlayerZ / 100);
					ScnMgr::GetInstance()->m_Obj[1]->SetVel(0.f, 0.f, 0.f);
					
					//ScnMgr::GetInstance()->m_Obj[0]->SetPos(/*PlayerX / 100*/0, -230 / 100, PlayerZ / 100);
					//ScnMgr::GetInstance()->m_Obj[0]->SetVel(0.f, 0.f, 0.f);
					//ScnMgr::GetInstance()->m_Obj[1]->SetPos(/*PlayerX / 100*/0.5f, -230 / 100, PlayerZ / 100);
					//ScnMgr::GetInstance()->m_Obj[1]->SetVel(0.f, 0.f, 0.f);
					//
					ScnMgr::GetInstance()->CreateBoss();
					//}
					m_blsCheckMonster = false;
				}
			}
		}
	}
}
