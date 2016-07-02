#include "stdafx.h"
#include "mySFMLVectorFunctions.hpp"
#include "myUsefulMath.hpp"

namespace mySFML {

	sf::Vector2f mySFML::meanVector(sf::Vector2f vec1, sf::Vector2f vec2)
	{
		return sf::Vector2f(myMath::meanValue(vec1.x, vec2.x), myMath::meanValue(vec1.y, vec2.y));
	}

	sf::Vector2f mySFML::createNormalVectorWithAngle(float angleInRadiant)
	{
		return sf::Vector2f(cos(angleInRadiant), sin(angleInRadiant));
	}

	sf::Vector2f mySFML::randNormalVector()
	{
		//return mySFML::normalize(sf::Vector2f(static_cast<float>(rand()), static_cast<float>(rand())));
		return mySFML::normalize(sf::Vector2f(myMath::randIntervalf(-1000,1000),myMath::randIntervalf(-1000,1000)));
	}

	bool mySFML::smaller(sf::Vector2f smallerVector, sf::Vector2f largerVector)
	{
		bool xSmaller = (smallerVector.x < largerVector.x);
		bool ySmaller = (smallerVector.y < largerVector.y);

		return (xSmaller && ySmaller);
	}
	bool mySFML::smaller(sf::Vector2f smallerVector, sf::Vector2f largerVector, bool &xSmaller, bool &ySmaller)
	{
		xSmaller = (smallerVector.x < largerVector.x);
		ySmaller = (smallerVector.y < largerVector.y);

		return ( xSmaller && ySmaller);
	}

	bool mySFML::smaller(sf::Vector2i smallerVector, sf::Vector2i largerVector)
	{
		bool xSmaller = (smallerVector.x < largerVector.x);
		bool ySmaller = (smallerVector.y < largerVector.y);

		return (xSmaller && ySmaller);
	}
	bool mySFML::smaller(sf::Vector2i smallerVector, sf::Vector2i largerVector, bool &xSmaller, bool &ySmaller)
	{
		xSmaller = (smallerVector.x < largerVector.x);
		ySmaller = (smallerVector.y < largerVector.y);

		return (xSmaller && ySmaller);
	}
	
}