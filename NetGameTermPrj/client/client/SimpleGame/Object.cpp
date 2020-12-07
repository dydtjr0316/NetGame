#include "stdafx.h"	
#include "Object.h"
#include <iostream>
#include "Globals.h"
#include <math.h>
#include <float.h>

Object::Object()
{
	InitPhysics();
}


Object::~Object()
{
}

int Object::Update(float elapsedInSec)
{
	// reduce remaining cool
	m_remainingCoolTime -= elapsedInSec;
	/////////////////////Apply friction////////////////////
	float nForce = m_mass * GRAVITY; // scalar
	float fForce = m_fricCoef * nForce; // scalar
	float velSize = sqrt(m_velX * m_velX + m_velY * m_velY + m_velZ * m_velZ);

	if (velSize > FLT_EPSILON)
	{
		float fDirX = m_velX / velSize;
		float fDirY = m_velY / velSize;
		float frictionX = -fDirX * fForce;
		float frictionY = -fDirY * fForce;
		float fAccX = frictionX / m_mass;
		float fAccY = frictionY / m_mass;
		float newVelX = m_velX + fAccX * elapsedInSec;
		float newVelY = m_velY + fAccY * elapsedInSec;

		if (newVelX * m_velX < 0.02f) newVelX = 0.0f;
		if (newVelY * m_velY < 0.02f) newVelY = 0.0f;

		m_velX = newVelX;
		m_velY = newVelY;

		m_velZ = m_velZ - GRAVITY * elapsedInSec;
	}
	
	///////////////////////////////////////////////////////
	m_posX = m_posX + m_velX * elapsedInSec;
	m_posY = m_posY + m_velY * elapsedInSec;
	m_posZ = m_posZ + m_velZ * elapsedInSec;

	if (m_posZ < FLT_EPSILON)
	{
		m_posZ = 0.f;
		m_velZ = 0.f;
	}

	return 0;
}

void Object::Render()
{
}

void Object::AddForce(float x, float y, float z, float eTime)
{
	float accX, accY, accZ;
	accX = accY = accZ = 0.f;

	accX = x / m_mass;
	accY = y / m_mass;
	accZ = z / m_mass;

	m_velX = m_velX + accX * eTime;
	m_velY = m_velY + accY * eTime;
	m_velZ = m_velZ + accZ * eTime;

}

void Object::InitPhysics()
{
	float m_r= 0.f, m_g = 0.f, m_b = 0.f, m_a = 0.f; // color
	float m_posX = 0.f, m_posY = 0.f, m_posZ = 0.f; // Position
	float m_mass = 0.f; // size
	float m_velX = 0.f, m_velY = 0.f, m_velZ = 0.f; // Velocity
	float m_accX = 0.f, m_accY = 0.f, m_accZ = 0.f; // Acc
	float m_volX = 0.f, m_volY = 0.f, m_volZ = 0.f; // Volume
	m_fricCoef = 0.f;
}

void Object::SetColor(float r, float g, float b, float a)
{
	
	m_r = r;
	m_g = g;
	m_b = b;
	m_a = a;
}

void Object::GetColor(float *r, float *g, float *b, float *a)
{
	*r = m_r;
	*g = m_g;
	*b = m_b;
	*a = m_a;
}

void Object::SetPos(float posx, float posy, float posz)
{
	m_posX = posx;
	m_posY = posy;
	m_posZ = posz;
}
void Object::SetMass(float mass)
{
	m_mass = mass;
}

void Object::SetVel(float velx, float vely, float velz)
{
	m_velX = velx;
	m_velY = vely;
	m_velZ = velz;
}

void Object::SetVol(float volx, float voly, float volz)
{
	m_volX = volx;
	m_volY = voly;
	m_volZ = volz;
}

void Object::SetFricCoef(float coef)
{
	m_fricCoef = coef;
}

void Object::SetType(int type)
{
	m_type = type;
}

void Object::SetTexID(int id)
{
	m_textID = id;
}

void Object::GetPos(float *posx, float *posy, float *posz)
{
	*posx = m_posX;
	*posy = m_posY;
	*posz = m_posZ;
}
void Object::GetMass(float *mass)
{
	*mass = m_mass;
}

void Object::GetVel(float *velx, float *vely, float *velz)
{
	*velx = m_velX;
	*vely = m_velY;
	*velz = m_velZ;
}

void Object::GetVol(float *volx, float *voly, float *volz)
{
	*volx = m_volX;
	*voly = m_volY;
	*volz = m_volZ;
}

void Object::GetFricCoef(float * coef)
{
	*coef = m_fricCoef;
}

void Object::GetType(int * type)
{
	*type = m_type;
}

void Object::GetTexID(int *id)
{
	*id = m_textID;
}

void Object::SetParentObj(Object* parent)
{
	m_parent = parent;
}

void Object::GetHP(float* hp)
{
	*hp = m_healthPoint;
}

void Object::SetHP(float hp)
{
	m_healthPoint = hp;
}
