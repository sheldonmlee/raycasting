#include "maths.h"

float maths_modulo(float a, float b)
{
	float r = fmod(a, b);
	return r < 0? r + b : r;
}
