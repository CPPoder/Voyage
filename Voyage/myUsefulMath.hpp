#ifndef MYUSEFULMATH_HPP
#define MYUSEFULMATH_HPP

namespace myMath {

	template <typename T> T max(T num1, T num2)
	{
		if (num1 > num2)
		{
			return num1;
		}
		else
		{
			return num2;
		}
	}

	template <typename T> T min(T num1, T num2)
	{
		if (num1 > num2)
		{
			return num2;
		}
		else
		{
			return num1;
		}
	}

	float meanValue(float num1, float num2);

	int randIntervali(int small, int big);

	float randIntervalf(int small, int big);

	template <typename T> T abs(T value)
	{
		if (value < T(0))
		{
			return (-value);
		}
		else
		{
			return value;
		}
	}

	int sign(int value);
	float sign(float value);


}



#endif