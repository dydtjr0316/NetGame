#include "stdafx.h"
#include "Bullet.h"

CBullet::CBullet()
{
}

CBullet::~CBullet()
{
}

void CBullet::LateInit()
{
	if (m_blsInit == true)
		return;
	m_textID = ScnMgr::GetInstance()->m_BulletID;
	m_blsInit = true;
}

void CBullet::Render()
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

    ScnMgr::GetInstance()->m_Renderer->DrawTextureRect(x, y, z, sx, sy, sz, r, g, b, a, m_textID);
}

int CBullet::Update(float elapsedInSec)
{
	LateInit();
	Object::Update(elapsedInSec);

	if (m_posZ <= 0.f)
		m_blsDead = true;

	if(MapCollisionCheck(&m_posX, &m_posY))
		m_blsDead=true;

	if (m_blsDead)
	{
			ScnMgr::GetInstance()->m_Sound->PlayShortSound(ScnMgr::GetInstance()->m_iBulletSound, false, 0.5);
	}

	if (!m_blsDead)
	{
			ScnMgr::GetInstance()->AddRenderGroup(ScnMgr::OBJECT, this);
	}

	return m_blsDead;
}


