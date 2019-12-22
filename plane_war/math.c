#include <math.h>

int MATH_Abs(int a) 
{
	if (a < 0) 
	{
		a = -a;
	}
	return a;
}

int MATH_Max(int a, int b) 
{
	if (a > b)
	{
		return a;
	}
	return b;
}

int MATH_Min(int a, int b)
{
	if (a > b)
	{
		return b;
	}
	return a;
}