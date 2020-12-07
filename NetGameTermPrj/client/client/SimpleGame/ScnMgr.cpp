#include "stdafx.h"
#include "ScnMgr.h"
#include <iostream>
#include <time.h>
#include <math.h>
#include "Dependencies\freeglut.h"
#include <float.h>
#include "Player.h"
#include "Hp.h"

#include "Door.h"
#include "Boss.h"

int g_Stage = -1;
int g_UI = -1;
int g_BGM = -1;

ScnMgr* ScnMgr::m_pInstance = NULL;
ScnMgr* ScnMgr::GetInstance()
{
	if (NULL == m_pInstance)
	{
		m_pInstance = new ScnMgr;
	}
	return m_pInstance;
}

void ScnMgr::DestroyInstance(void) {
	if (NULL != m_pInstance) {
		delete m_pInstance;
		m_pInstance = NULL;
	}
}

ScnMgr::ScnMgr()
{
	srand(unsigned(time(nullptr)));

	m_Renderer = new Renderer(800,600);

	if (!m_Renderer->IsInitialized())
	{
		std::cout << "Renderer could not be initialized.. \n";
	}

	//init Physics
	m_Physics = new Physics;

	//init Sound
	m_Sound = new Sound;

	//initialize Objects
	for (int i = 0; i < MAX_OBJ_COUNT; ++i)
	{
		m_Obj[i] = NULL;
	}
	//Create Hero
	//m_Obj[HERO_ID] = new CPlayer();
	//m_Obj[HERO_ID]->SetID(900);
	//
	//m_Obj[HERO_ID]->SetPos(0, 0, 0);
	//m_Obj[HERO_ID]->SetVol(0.1, 0.1, 0.1);
	//m_Obj[HERO_ID]->SetColor(1, 1, 1, 1);
	//m_Obj[HERO_ID]->SetVel(0, 0, 0);
	//m_Obj[HERO_ID]->SetMass(1.0);
	//m_Obj[HERO_ID]->SetFricCoef(0.9f);
	//m_Obj[HERO_ID]->SetType(TYPE_NORMAL);
	//m_Obj[HERO_ID]->SetHP(6.f);
	g_Stage = m_Renderer->GenPngTexture("../Texture/StartRoom.Png");
	g_UI = m_Renderer->GenPngTexture("../Texture/UI.Png");

	g_BGM = m_Sound->CreateBGSound("../Sound/StageBgm.wav");
	m_iMonsterDead = m_Sound->CreateShortSound("../Sound/MonsterDeath.wav");
	
	m_Sound->PlayBGSound(g_BGM, true, 1);
	m_iBulletSound = m_Sound->CreateShortSound("../Sound/BulletEnd.wav");
	m_BulletID = m_Renderer->GenPngTexture("../Texture/Bullet.png");

	m_iBloodPaticle = m_Renderer->GenPngTexture("../Texture/BloodPaticle.png");

	CDoor* Pobj2 = new CDoor;
	AddObject(0.f, 0.f, 0.f, 0.8f, 0.8f, 0.8f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, TYPE_UI, 1.f, Pobj2);

	//CHp* Pobj = new CHp;
	//AddObject(0.f, 0.f, 0.f, 0.8f, 0.8f, 0.8f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, TYPE_HP, 1.f, Pobj);
	
	list<Object*> pList;
	for (INT i = 0; i < END; i++)
	{
		m_RenderGroup.push_back(pList);
	}

	m_iBloodPaticle = m_Renderer->CreateParticleObject(
		100,
		-8, -8,
		8, 8,
		2, 2,
		5, 5,
		-5, -5,
		5, 5
	);
}

ScnMgr::~ScnMgr()
{
	if (m_Renderer != NULL) {
		delete m_Renderer;
		m_Renderer = NULL;
	}

	if (m_Physics != NULL)
	{
		delete m_Renderer;
		m_Physics = NULL;
	}
}

void ScnMgr::Update(float elapsedInSec)
{
	//FOr all objs, call update func
	for (int i = 0; i < MAX_OBJ_COUNT; ++i) {
		if (m_Obj[i] != NULL)
			if (m_Obj[i]->Update(elapsedInSec))
				DeleteObject(i); 
	}

	float x, y, z;
	//m_Obj[HERO_ID]->GetPos(&x, &y, &z);
	m_Renderer->SetCameraPos(0.f,   0.f);

	if (m_blsShake)
	{
		m_fShakeTime += 0.3f;
		m_Renderer->SetCameraPos(0.f+(cos(m_fShakeTime)), -(sin(m_fShakeTime)));

		if (m_fShakeTime > 50.f)
		{
			m_fShakeTime = 0.f;
			m_blsShake = false;
		}
	}
}

void ScnMgr::RenderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.3f, 0.3f, 1.0f);
	//draw background first
	m_Renderer->DrawGround(0.f, 0.f, 0.f,
		800.f, 600.f, 0.f,
		1, 1, 1, 1,
		g_UI,
		1.f);
	int  StageNum = g_Stage;

	m_Renderer->DrawGround(0.f, -50.f, 0.f,
		800.f, 500.f, 0.f,
		1, 1, 1, 1,
		StageNum,
		1.f);

	// Draw all m_Objs
	for (int i = 0; i < END; ++i)
	{
		for (auto& pSrc : m_RenderGroup[i])
		{
			int t;
			pSrc->GetType(&t);
			/*switch (t)
			{
			case TYPE_NORMAL:
				cout << "플레이어" << endl;
				break;
			case TYPE_MONSTER:
				cout << "보스" << endl;
				break;
			case TYPE_UI:
				cout << "문" << endl;
				break;
			default:
				cout << "타입 : " << t << endl;
				break;
			}*/
			// 터지기전에 타입이 쓰레기값
			if (t >= 0)
				pSrc->Render(); // 여기서 터짐
			else
				cout << "trash -> i velue => " << i << endl;
		}
	}
	for (int i = 0; i < END; ++i)
	{
		m_RenderGroup[i].clear();
	}
}

void ScnMgr::DoGarbageCollect()
{
	for (int i = 0; i < MAX_OBJ_COUNT; ++i)
	{
		if (m_Obj[i] == NULL)
			continue;

		int type = -1;
		m_Obj[i]->GetType(&type);

		if (type == TYPE_BULLET)
		{
			// Check velocity size
			float vx, vy, vz;
			m_Obj[i]->GetVel(&vx, &vy, &vz);
			float vSize = sqrtf(vx * vx + vy * vy + vz * vz);
			if (vSize < FLT_EPSILON)
			{
				DeleteObject(i);
				continue;
			}
		}

		float hp;
		m_Obj[i]->GetHP(&hp);
		if (hp < FLT_EPSILON)
		{
			DeleteObject(i);
			continue;
		}
	}
}

int ScnMgr::AddObject(float x, float y, float z,
	float sx, float sy, float sz,
	float r, float g, float b, float a,
	float vx, float vy, float vz,
	float mass, float fricCoef, float type, float hp,Object * pObj)
{
	int i =0 ;
	if (type == TYPE_NORMAL)i = 0;
	else if (type == TYPE_UI) {
		m_Obj[MAX_OBJ_COUNT-1] = pObj;
		m_Obj[MAX_OBJ_COUNT-1]->SetPos(x, y, z);
		m_Obj[MAX_OBJ_COUNT-1]->SetVol(sx, sy, sz);
		m_Obj[MAX_OBJ_COUNT-1]->SetColor(r, g, b, a);
		m_Obj[MAX_OBJ_COUNT-1]->SetVel(vx, vy, vz);
		m_Obj[MAX_OBJ_COUNT-1]->SetMass(mass);
		m_Obj[MAX_OBJ_COUNT-1]->SetFricCoef(fricCoef);
		m_Obj[MAX_OBJ_COUNT-1]->SetType(type);
		m_Obj[MAX_OBJ_COUNT-1]->SetHP(hp);

		return MAX_OBJ_COUNT-1;
	}
	else if (type == TYPE_HP)
	{
		m_Obj[MAX_OBJ_COUNT - 2] = pObj;
		m_Obj[MAX_OBJ_COUNT - 2]->SetPos(x, y, z);
		m_Obj[MAX_OBJ_COUNT - 2]->SetVol(sx, sy, sz);
		m_Obj[MAX_OBJ_COUNT - 2]->SetColor(r, g, b, a);
		m_Obj[MAX_OBJ_COUNT - 2]->SetVel(vx, vy, vz);
		m_Obj[MAX_OBJ_COUNT - 2]->SetMass(mass);
		m_Obj[MAX_OBJ_COUNT - 2]->SetFricCoef(fricCoef);
		m_Obj[MAX_OBJ_COUNT - 2]->SetType(type);
		m_Obj[MAX_OBJ_COUNT - 2]->SetHP(hp);

		return MAX_OBJ_COUNT -2;
	}
	else if (type == TYPE_MONSTER)
	{
		m_Obj[MAX_OBJ_COUNT - 3] = pObj;
		m_Obj[MAX_OBJ_COUNT - 3]->SetPos(x, y, z);
		m_Obj[MAX_OBJ_COUNT - 3]->SetVol(sx, sy, sz);
		m_Obj[MAX_OBJ_COUNT - 3]->SetColor(r, g, b, a);
		m_Obj[MAX_OBJ_COUNT - 3]->SetVel(vx, vy, vz);
		m_Obj[MAX_OBJ_COUNT - 3]->SetMass(mass);
		m_Obj[MAX_OBJ_COUNT - 3]->SetFricCoef(fricCoef);
		m_Obj[MAX_OBJ_COUNT - 3]->SetType(type);
		m_Obj[MAX_OBJ_COUNT - 3]->SetHP(hp);

		return MAX_OBJ_COUNT - 3;
	}
	else i = 2;

	int idx = -1;
	for (; i < MAX_OBJ_COUNT; ++i)
	{
		if (m_Obj[i] == NULL)
		{
			m_Obj[i] = pObj;
			m_Obj[i]->SetPos(x, y, z);
			m_Obj[i]->SetVol(sx, sy, sz);
			m_Obj[i]->SetColor(r, g, b, a);
			m_Obj[i]->SetVel(vx, vy, vz);
			m_Obj[i]->SetMass(mass);
			m_Obj[i]->SetFricCoef(fricCoef);
			m_Obj[i]->SetType(type);
			m_Obj[i]->SetHP(hp);

			return i;
		}
	}

	if (idx == -1)
	{
		std::cout << "No more empty Obj slot." << std::endl;
		return -1;
	}

	return idx;
}

void ScnMgr::DeleteObject(int idx)
{
	if (idx < 0)
	{
		std::cout << "Input idx is negative : " << idx << std::endl;
		return;
	}

	if (idx >= MAX_OBJ_COUNT)
	{
		std::cout << "Input idx exceeds MAX_OBJ_COUNT" << idx << std::endl;
		return;
	}
	if (m_Obj[idx] == NULL)
	{
		std::cout << "m_Obj[ " << idx << "] is NULL" << std::endl;
		return;
	}

	delete m_Obj[idx];
	m_Obj[idx] = NULL;
}

void ScnMgr::AddRenderGroup(RENDERGROUP Group,Object * pObj)
{
	m_RenderGroup[Group].push_back(pObj);
}

void ScnMgr::KeyDownInput(unsigned char key, int x, int y)
{
	if (key == 'w' || key == 'W')
	{
		m_KeyW = true;
	}
	if (key == 'a' || key == 'A')
	{
		m_KeyA = true;
	}
	if (key == 's' || key == 'S')
	{
		m_KeyS = true;
	}
	if (key == 'd' || key == 'D')
	{
		m_KeyD = true;
	}
	if (key == ' ')
	{
		m_KeySP = true;
	}
}

void ScnMgr::KeyUpInput(unsigned char key, int x, int y)
{
	if (key == 'w' || key == 'W')
	{
		m_KeyW = false;
	}
	if (key == 'a' || key == 'A')
	{
		m_KeyA = false;
	}
	if (key == 's' || key == 'S')
	{
		m_KeyS = false;
	}
	if (key == 'd' || key == 'D')
	{
		m_KeyD = false;
	}
	if (key == ' ')
	{
		m_KeySP = false;
	}
}

void ScnMgr::CreateStart()
{
	g_Stage = m_Renderer->GenPngTexture("../Texture/Stage1.Png");
	m_eCurStage = START;
	ResetObject();

}

void ScnMgr::CreateBoss()
{
	if(m_blsBoss == false)
	{
		ResetObject();
		m_blsBoss = true;

		CBoss* pObj2 = new CBoss;
		AddObject(0.f, 0.0f, 0, 1.0, 1.0, 1.0, 1, 1, 1, 1, 1, 1, 1, 5, 1, TYPE_MONSTER, 40, pObj2);
		m_Sound->StopBGSound(g_BGM);
		g_BGM = m_Sound->CreateBGSound("../Sound/MainBGM.wav");
		m_Sound->PlayBGSound(g_BGM, true, 1);
	}

	m_eCurStage = BOSS;
}

void ScnMgr::ResetObject()
{
	for (int i = 2; i < MAX_OBJ_COUNT; i++)
	{
		if (m_Obj[i] == NULL)
			continue;
		int type = 0;
		m_Obj[i]->GetType(&type);
		if (type != TYPE_UI && type != TYPE_MONSTER )
			m_Obj[i]->lsDead();
		//if (/*i != 0&&i!=1 && */type != TYPE_MONSTER)
		//	m_Obj[i]->lsDead();
	}
}

void ScnMgr::ShakeCamara()
{
	m_blsShake = true;
}

void ScnMgr::Ending()
{
	m_Sound->StopBGSound(g_BGM);
	g_BGM = m_Sound->CreateBGSound("../Sound/CafeBene.wav");
	m_Sound->PlayBGSound(g_BGM,false,1);
}

void ScnMgr::SpecialKeyDownInput(int key, int x, int y)
{
	if (key == GLUT_KEY_F1)
	{
		//GetInstance()->CreateBoss();
		//GetInstance()->m_Obj[HERO_ID]->SetPos(10 / 100, 110 / 100, 10 / 100);
		//GetInstance()->m_Obj[HERO_ID]->SetVel(0.f, 0.f, 0.f);
	}
	if (key == GLUT_KEY_UP)
	{
		m_KeyUp = true;
	}
	if (key == GLUT_KEY_LEFT)
	{
		m_KeyLeft = true;
	}
	if (key == GLUT_KEY_DOWN)
	{
		m_KeyDown = true;
	}
	if (key == GLUT_KEY_RIGHT)
	{
		m_KeyRight = true;
	}
}

void ScnMgr::SpecialKeyUpInput(int key, int x, int y)
{
	if (key == GLUT_KEY_UP)
	{
		m_KeyUp = false;
	}
	if (key == GLUT_KEY_LEFT)
	{
		m_KeyLeft = false;
	}
	if (key == GLUT_KEY_DOWN)
	{
		m_KeyDown = false;
	}
	if (key == GLUT_KEY_RIGHT)
	{
		m_KeyRight = false;
	}
}