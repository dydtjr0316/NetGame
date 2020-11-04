#pragma once

static bool MapCollisionCheck(float* x, float* y)
{
	bool blsCollision = false;
	*x = *x * 100.f;
	*y = *y * 100.f;

	if (*x > 320)
	{
		*x = 319;
		blsCollision = true;
	}
	if (*x < -320)
	{
		blsCollision = true;
		*x = -319;
	}
	if (*y > 120)
	{
		blsCollision = true;
		*y = 119;
	}
	if (*y < -235)
	{
		blsCollision = true;
		*y = -234;
	}



	*x = *x /100.f;
	*y = *y / 100.f;

	return blsCollision;

}