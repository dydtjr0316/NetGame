#pragma once
#include "Object.h"

class Physics
{
public:
	Physics();
	~Physics();

	bool IsOverlap(Object *A, Object *B, int method = 0);
	void processCollision(Object *A, Object *B);
private:
	bool BBOverlapTest(Object *A, Object *B);
};

