#ifndef MYSFMLVECTORFUNCTIONS_HPP
#define MYSFMLVECTORFUNCTIONS_HPP

#include "SFML\Graphics.hpp"

#include <iostream>
#include <math.h>
#include <string>


namespace mySFML {
	//Lengths
	template <typename T> T lengthOf(sf::Vector2<T> vector)
	{
		return sqrt(vector.x * vector.x + vector.y * vector.y);
	}

	//Useful Functions
	template <typename T> sf::Vector2<T> normalize(sf::Vector2<T> vector)
	{
		T length = lengthOf(vector);
		if (length == T(0))
		{
			return sf::Vector2<T>(T(0), T(0));
		}
		else
		{
			return (vector / length);
		}
	}

	sf::Vector2f meanVector(sf::Vector2f vec1, sf::Vector2f vec2);
	sf::Vector2f createNormalVectorWithAngle(float angleInRadiant);
	sf::Vector2f randNormalVector();

	//Comparisons
	bool smaller(sf::Vector2f smallerVector, sf::Vector2f largerVector);
	bool smaller(sf::Vector2f smallerVector, sf::Vector2f largerVector, bool &xSmaller, bool &ySmaller);
	bool smaller(sf::Vector2i smallerVector, sf::Vector2i largerVector);
	bool smaller(sf::Vector2i smallerVector, sf::Vector2i largerVector, bool &xSmaller, bool &ySmaller);

	//Componentwise Multiplication (Returns a Vector with the first arguments type)
	template <typename T, typename S> sf::Vector2<T> compMult(sf::Vector2<T> vec1, sf::Vector2<S> vec2)
	{
		return sf::Vector2<T>(vec1.x * vec2.x, vec1.y * vec2.y);
	}

	//Terminal Output
	template <typename T> sf::Vector2<T> outputOnTerminal(sf::Vector2<T> vector, std::string description = "", std::string seperator = " ")
	{
		std::cout << description << vector.x << seperator << vector.y << std::endl;
		return vector;
	}

	//OrthogonalVector
	template <typename T> sf::Vector2<T> createOrthogonalVector(sf::Vector2<T> vector)
	{
		sf::Vector2<T> result;
		result.x = -vector.y;
		result.y = vector.x;
		return result;
	}

	//Inner product
	template <typename T> T scalarProduct(sf::Vector2<T> vec1, sf::Vector2<T> vec2)
	{
		return (vec1.x * vec2.x + vec1.y * vec2.y);
	}
	template <typename T> T scalarProduct(sf::Vector3<T> vec1, sf::Vector3<T> vec2)
	{
		return (vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z);
	}

	//Color division operator
	sf::Color colorMultiplication(sf::Color color, float factor);

	//Mix colors
	sf::Color mixColors(sf::Color color1, sf::Color color2, float weight1, float weight2);

}


#endif