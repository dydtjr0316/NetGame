#include "stdafx.h"
#include "Player.h"
#include <math.h>
#include "Bullet.h"
#include "CServer.h"

CPlayer::CPlayer()
{
	m_server = new SERVER;
	//m_id = m_server->GetID();
}

CPlayer::~CPlayer()
{

}

int CPlayer::Update(float elapsedInSec)
{
	if (m_velX > 5.0f)
		m_velX = 5.0f;
	if (m_velY > 5.0f)
		m_velY = 5.0f;
	if (m_velY < -5.0f)
		m_velY = -5.0f;
	if (m_velX < -5.0f)
		m_velX = -5.0f;
	LateInit();
	Object::Update(elapsedInSec);

	// recv


	char buf[50];
	ZeroMemory(&buf, sizeof(buf));

	KeyInput(elapsedInSec);

	int ret = 0;
	ret = m_server->recvn(m_server->GetSock(), buf, 50, 0);

	if (ret!=0)
	{
		switch (buf[1])
		{
		case SC_PACKET_MOVE:
		{
			SC_Move_Packet* packet = reinterpret_cast<SC_Move_Packet*>(buf);
			if (m_id == packet->id)
			{
				m_velX = packet->x;
				m_velY = packet->y;
				m_Head = packet->head;
				m_CurState = packet->curstate;
			}
		}
			break;
		case SC_PACKET_ATTACK:
		{
			SC_Attack_Packet* attack_packet = reinterpret_cast<SC_Attack_Packet*>(buf);

			if (attack_packet->bulletsize > 0.000001f)
			{
				ScnMgr::GetInstance()->m_Sound->PlayShortSound(m_iBulletSound, false, 2);
				CBullet* pObj = new CBullet;

				int id = ScnMgr::GetInstance()->AddObject(m_posX, m_posY, m_posZ + 0.4f,
					0.2f, 0.2f, 0.2f,
					1, 1, 1, 1,
					attack_packet->bulletx, attack_packet->bullety, attack_packet->bulletz,
					0.1f, 0.2f, TYPE_BULLET, 2.f, pObj);

				ScnMgr::GetInstance()->m_Obj[id]->AddForce
				(attack_packet->bulletx, attack_packet->bullety, attack_packet->bulletz, 0.1f);

				ScnMgr::GetInstance()->m_Obj[id]->SetParentObj(this);

			}
		}
			break;
		default:
			break;
		}
	}


	if(m_blsDamaged==false)
	CollisionCheck();
	int Collision=MapCollisionCheck(&m_posX, &m_posY);

	if (Collision)
	{
		m_velX = 0.f;
		m_velY = 0.f;
	}


	ScnMgr::GetInstance()->AddRenderGroup(ScnMgr::RENDERGROUP::OBJECT, this);

	return m_blsDead;


}

void CPlayer::Render()
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


	static float temp = 0;
	if (m_CurState != LEFT)			temp += 0.2f;
	else
	temp -= 0.2f;
	int TexId;

	int iX = (int)temp % 10;
	int iY = 0;


	if (m_CurState == RIGHT) {
		TexId = m_texR;
	}

	else if (m_CurState == UP) {
		TexId = m_texUD;
	}
	else if (m_CurState == LEFT) 
	{
		TexId = m_texL;
	}
	else
	{
		TexId = m_texUD;
	}
	float ySize = 0.75f;
	if (TexId == m_texUD)
		ySize = 0.9f;
	if (m_CurState == IDLE && m_velX==0.f && m_velY ==0.f)
	{
		temp = 0.f;
		TexId = m_texUD;
	}

	if (m_blsDamaged)
	{
		m_DamageCooltime += 0.05f;
		m_g += m_DamageCooltime;
		m_b += m_DamageCooltime;

		if (m_g > 1)
		{
			m_g = 0;
			m_iFlickCount += 1;
			m_DamageCooltime = 0.f;
		}
		if (m_b > 1)
		{
			m_b = 0;
			m_iFlickCount += 1;
			m_DamageCooltime = 0.f;
		}
	}

	if (m_iFlickCount > 18)
	{
		m_b = 1;
		m_g = 1;
		m_iFlickCount = 0;
		m_blsDamaged = 0;
		m_DamageCooltime = 0;
	}
	ScnMgr::GetInstance()->m_Renderer->DrawTextureRectAnim(
		x, y, z,
		sx*0.7f, sy*ySize, sz*0.7f,
		m_r, m_g, m_b, a,
		TexId,
		10,
		1,
		temp,
		1,true);

	DrawHead();
	

}

void CPlayer::DrawHead()
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

	int iY = 0;
	if (m_Head == DOWN) 	m_MaxFrame = 0.f;

	else if (m_Head == UP) 	m_MaxFrame = 4.f;

	else if (m_Head == LEFT) m_MaxFrame = 6.f;

	else 	m_MaxFrame = 2.f;


	ScnMgr::GetInstance()->m_Renderer->DrawTextureRectAnim(
		x, y, z+20.f,
		sx, sy, sz,
		m_r, m_g, m_b, a,
		m_textID,
		8,
		1,
		m_MaxFrame,
		1,false);
}

void CPlayer::Shooting()
{


	/*
	SC_Attack_Packet& attack_packet = m_server->RecvAttackPacket();*/

	//cout << attack_packet.bulletsize << endl;


}

void CPlayer::Move(bool shoot, float elapsedInSec)
{
}

void CPlayer::KeyInput(float elapsedInSec)
{

	m_blsCanShoot = false;

	bool attack_key_input = false;
	m_CurState = IDLE;
	m_Head = DOWN;
	m_Dir = DIR::NONE;

	if (is_owner)
	{
		if (ScnMgr::GetInstance()->m_KeyW)
		{
			m_Dir = DIR::UP;
		}
		if (ScnMgr::GetInstance()->m_KeyS)
		{
			m_Dir = DIR::DOWN;
		}
		if (ScnMgr::GetInstance()->m_KeyA)
		{
			m_Dir = DIR::LEFT;
		}
		if (ScnMgr::GetInstance()->m_KeyD)
		{
			m_Dir = DIR::RIGHT;
		}
		if (m_Dir != DIR::NONE)
		{
			m_server->SendMovePacket(m_id, CS_PACKET_MOVE, m_Dir, m_Head, elapsedInSec, m_velX, m_velY, m_mass);
			return;
		}
		
		if (Shoot)
		{
			if (m_CurrentCoolTIme == 0)
				m_blsCanShoot = true;

			m_CurrentCoolTIme += elapsedInSec;
			if (m_CurrentCoolTIme > 0.3)
				m_CurrentCoolTIme = 0;

		}

		 Shoot = true;

		 if (ScnMgr::GetInstance()->m_KeyRight) {
			 m_Head = RIGHT; attack_key_input = true;
		 }
		 else if (ScnMgr::GetInstance()->m_KeyLeft) {
			 m_Head = LEFT; attack_key_input = true;
		 }
		 else if (ScnMgr::GetInstance()->m_KeyUp) {
			 m_Head = UP; attack_key_input = true;
		 }
		 else if (ScnMgr::GetInstance()->m_KeyDown) {
			 m_Head = DOWN; attack_key_input = true;
		 }
		 else
		 {
			 Shoot = false;
		 }

		 if (attack_key_input == true)
		 {
			 if (m_blsCanShoot == false)
				 return;

			 cout << "shooting" << endl;

			 float bulletVel = 4.f;

			 if (is_owner)
			 {
				 m_server->SendAttackPacket(m_id, CS_PACKET_ATTACK, m_Dir, m_Head, bulletVel);
			 }
		 }
	}

	//SC_Move_Packet& packet = m_server->RecvMovePacket();
}

void CPlayer::LateInit()
{
	if (m_blsInit == true)
		return;
	m_texR = ScnMgr::GetInstance()->m_Renderer->GenPngTexture("../Texture/Right.png");
	m_texL = ScnMgr::GetInstance()->m_Renderer->GenPngTexture("../Texture/Left.png");
	m_texUD = ScnMgr::GetInstance()->m_Renderer->GenPngTexture("../Texture/UpDown.png");
	m_textID = ScnMgr::GetInstance()->m_Renderer->GenPngTexture("../Texture/Head.png");
	m_iBulletSound = ScnMgr::GetInstance()->m_Sound->CreateShortSound("../Sound/Bullet.wav");
	m_iDamageSound = ScnMgr::GetInstance()->m_Sound->CreateShortSound("../Sound/Damage.wav");
	m_blsInit = true;

}

void CPlayer::CollisionCheck()
{
	for (int src = 0; src < MAX_OBJ_COUNT; src++)
	{
		if (ScnMgr::GetInstance()->m_Obj[src] != NULL)
		{
			if (ScnMgr::GetInstance()->m_Physics->IsOverlap(this, ScnMgr::GetInstance()->m_Obj[src],0))
			{
				int Type = 0;
				ScnMgr::GetInstance()->m_Obj[src]->GetType(&Type);

				if (Type == TYPE_MONSTER)
				{
					ScnMgr::GetInstance()->m_Physics->processCollision(this, ScnMgr::GetInstance()->m_Obj[src]);
					if (m_blsDamaged == false)
					{
						m_healthPoint -= 1;
						m_blsDamaged = true;
						ScnMgr::GetInstance()->m_Sound->PlayShortSound(m_iDamageSound, false, 1.f);
					}
					if (m_healthPoint <= 0)
						m_healthPoint = 1.f;

				}
			}

		}

	}


}
