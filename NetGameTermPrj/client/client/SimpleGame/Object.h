#pragma once
class Object
{
public:
	  Object();
	 ~Object();

 public:
	virtual void Render();

	void SetID(int id) { m_id = id; }

	virtual int Update(float elapsedInSec);
	void SetColor(float r, float g, float b, float a);
	void GetColor(float *r, float *g, float *b, float *a);
	void SetMass(float mass);
	void SetVol(float volx, float voly, float volz);
	void SetFricCoef(float coef);
	void SetType(int type);
	void SetTexID(int id);

	void AddForce(float x, float y, float z, float eTime);
	void SetPos(float posx, float posy, float posz);
	void SetVel(float velx, float vely, float velz);
	void SetHP(float hp);



	void GetMass(float *mass);
	void GetVol(float *volx, float *voly, float *volz);
	void GetFricCoef(float *coef);
	void GetType(int *type);
	void GetTexID(int *id);

	void GetHP(float* hp);
	void GetPos(float *posx, float *posy, float *posz);
	void GetVel(float *velx, float *vely, float *velz);

	void SetParentObj(Object* parent);
	void InitPhysics();
	bool GetDead() { return m_blsDead; }
	void lsDead() { m_blsDead = true; }

protected:
	int m_id;
	float m_posX, m_posY, m_posZ; // Position
	float m_velX, m_velY, m_velZ; // Velocity
	float m_healthPoint;

	float m_r, m_g, m_b, m_a; // color
	float m_sx, m_sy, m_sz; // size
	float m_volX, m_volY, m_volZ; // Volume
	float m_mass; // size
	float m_fricCoef;				//friction
	int m_type;				// object type
	int m_textID;
	
	Object* m_parent = NULL;

	float m_remainingCoolTime = 0.f;
	float m_CurrentCoolTIme = 0.1f;

	bool  m_blsDead = false;
};