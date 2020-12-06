#pragma once
#include"Object.h"
class CDoor : public Object
{
public:
	CDoor();
	virtual ~CDoor();
	int bcnt = 0;

	void LateInit();
	void Render();
	int Update(float elapsedInSec);
	void DrawDoor();
	void CheckDoor();
private:
	bool m_blsInit = false;
	int  m_iMiniMap = 0;
	bool m_blsCheckMonster = false;
	float m_fEndingTime = 0;
};