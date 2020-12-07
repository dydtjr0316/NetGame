#include "stdafx.h"
#include "Hp.h"

CHp::CHp()
{
}

CHp::~CHp()
{
}

void CHp::LateInit()
{
	if (m_blsInit == true)
		return;
	m_textID = ScnMgr::GetInstance()->m_Renderer->GenPngTexture("../Texture/HPUI.png");
	m_blsInit = true;
}

void CHp::Render()
{
	float x, y, z = 0;
	float sx, sy, sz = 0;
	float r, g, b, a = 0;

	x = 250.f;
	y = 200.f;
	z = m_posZ * 100.f;
	sx = m_volX * 100.f;
	sy = m_volY * 100.f;
	sz = m_volZ * 100.f;
	r = m_r;
	g = m_g;
	b = m_b;
	a = m_a;

	ScnMgr::GetInstance()->m_Renderer->DrawTextureRectAnim(x, y, z, sx, sy, sz, r, g, b, a, m_textID,1,6,0,6- m_iHpCount,false);
}

int CHp::Update(float elapsedInSec)
{
	LateInit();
	
	//ScnMgr::GetInstance()->m_Obj[HERO_ID]->GetHP(&m_iHpCount);
 
	ScnMgr::GetInstance()->AddRenderGroup(ScnMgr::OBJECT, this);

	return m_blsDead;

}
