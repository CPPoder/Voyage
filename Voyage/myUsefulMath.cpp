#include "stdafx.h"
#include "myUsefulMath.hpp"
#include <cstdlib>

namespace myMath {

	float myMath::meanValue(float num1, float num2)
	{
		return ((num1 + num2) / 2.f);
	}


	int myMath::randIntervali(int small, int big)
	{
		return (rand() % (big - small + 1) + small);
	}

	float myMath::randIntervalf(int small, int big)
	{
		return static_cast<float>(myMath::randIntervali(small, big));
	}

	int myMath::sign(int value)
	{
		if (value < 0)
		{
			return (-1);
		}
		else
		{
			return 1;
		}
	}

	float myMath::sign(float value)
	{
		if (value < 0)
		{
			return (-1.f);
		}
		else
		{
			return 1.f;
		}
	}

}
