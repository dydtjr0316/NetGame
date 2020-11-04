#pragma once

#include <string>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include "Dependencies\glew.h"

#include "Renderer.h"
#include "Object.h"
#include "Globals.h"
#include "Physics.h"
#include "Sound.h"

#include <list>
class ScnMgr
{
public:
	enum STAGENUM{START,BOSS};
public:
	enum RENDERGROUP{MAP,OBJECT,UI,END};

private:
	static ScnMgr* m_pInstance;
public:
	static ScnMgr* GetInstance();
	static void DestroyInstance(void);

private:
	explicit ScnMgr();
	virtual ~ScnMgr();
public:
	void RenderScene(void);
	void DoGarbageCollect();

	int AddObject(float x, float y, float z,
					float sx, float sy, float sz,
					float r, float g, float b, float a,
		float vx, float vy, float vz,
		float mass, float fricCoef, float type, float hp,Object* pObj);
	void DeleteObject(int idx);


	void AddRenderGroup(RENDERGROUP Group,Object* pObj);


	void Update(float elapsedInSec);
	void SpecialKeyDownInput(int key, int x, int y);
	void SpecialKeyUpInput(int key, int x, int y);
	void KeyDownInput(unsigned char key, int x, int y);
	void KeyUpInput(unsigned char key, int x, int y);

	void CreateStart();
	void CreateBoss();
	void ResetObject();
	void ShakeCamara();
	void Ending();
public:
	Renderer* m_Renderer = NULL;
	Sound* m_Sound = NULL;
	Object* m_Obj[MAX_OBJ_COUNT];
	vector<list<Object*>> m_RenderGroup;
	Physics* m_Physics = NULL;
	int m_TestChar[10];

public:
	bool m_KeyW = false;
	bool m_KeyA = false;
	bool m_KeyS = false;
	bool m_KeyD = false;
	bool m_KeySP = false;

	bool m_KeyUp = false;
	bool m_KeyLeft = false;
	bool m_KeyDown = false;
	bool m_KeyRight = false;

public:
	bool m_blsStage1 = false;
	bool m_blsStage2 = false;
	bool m_blsStage3 = false;
	bool m_blsBoss = false;
public:
	bool m_blsShake = false;
	float m_fShakeTime = 0.f;
public:
	int m_BulletID = -1;
	STAGENUM m_eCurStage = START;
	int m_iBloodPaticle = -1;
	int m_iBulletSound = -1;

	int m_iMonsterDead = -1;


};