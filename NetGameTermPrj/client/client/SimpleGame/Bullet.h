#pragma 
#include "Object.h"

class CBullet : public Object
{
public:
	CBullet();
	virtual ~CBullet();

	void LateInit();
	void Render();

	int Update(float elapsedInSec);

private:
	bool m_blsInit = false;
};