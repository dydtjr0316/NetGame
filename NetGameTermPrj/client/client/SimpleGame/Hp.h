#pragma 
#include "Object.h"

class CHp : public Object
{
public:
	CHp();
	virtual ~CHp();

	void LateInit();
	void Render();
	int Update(float elapsedInSec);

private:
	bool m_blsInit = false;
	float m_iHpCount = 6;
};