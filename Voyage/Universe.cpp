#include "stdafx.h"
#include "Universe.hpp"


//Constructor
Universe::Universe(sf::View *globalView, sf::View *playerView, bool *showPlayerView, sf::Font *font, sf::Texture *playerTexture)
	: pGlobalView(globalView),
	  pPlayerView(playerView),
	  pShowPlayerView(showPlayerView),
	  pFont(font),
	  pPlayerTexture(playerTexture)
{
	pTextTimeFactor = new sf::Text("", *pFont, 20);
	pTextActualTimeFactor = new sf::Text("", *pFont, 20);
	pTextWindowSize = new sf::Text("", *pFont, 20);
	pTextFramesPerSecond = new sf::Text("", *pFont, 20);
	pPlayer = new Player(1.0E06, 1.0, sf::Vector2<double>(1.0E07, 0.0), sf::Vector2<double>(0.0, 0.0), 0.0, 10.0, pPlayerTexture);
	
	
	
	//mVectorOfPlanets.push_back(new Planet(1.988E30 * 1.0E05, 1.408E03, sf::Vector2<double>(-1.496E11, 0.0), sf::Vector2<double>(0.0, 0.0), sf::Color::Yellow, createPlanetNumber()));
	mVectorOfPlanets.push_back(new Planet(5.974E24/* * 1.0E05*/, 5.515E03, sf::Vector2<double>(0.0, 0.0), sf::Vector2<double>(0.0, 0.0), sf::Color::Blue));
	mVectorOfPlanets.push_back(new Planet(7.349E22/* * 1.0E05*/, 3.341E03, sf::Vector2<double>(3.844E08, 0.0), 0.0 * sf::Vector2<double>(0.0, 1.023E03), sf::Color(120, 120, 120)));
	
	//Create Planets 1
	/*
	unsigned int numberOfPlanets = 10;
	for (unsigned int i = 0; i < numberOfPlanets; i++)
	{
		//mVectorOfPlanets.push_back(new Planet(1000000000.f, sf::Vector2f(720.f, 450.f) + mySFML::randNormalVector() * 120.f, mySFML::randNormalVector() * 0.1f));
		mVectorOfPlanets.push_back(new Planet(1.0E24, 4.0E03, static_cast<sf::Vector2<double>>(mySFML::createNormalVectorWithAngle(i * 2 * 3.141592653f / numberOfPlanets) * 10000000.f), static_cast<sf::Vector2<double>>(mySFML::randNormalVector() * 10000.f) * 0.0, sf::Color::White));
	}
	*/

	//Create Planets 2
	unsigned int numberOfPlanets = 100;
	for (unsigned int i = 0; i < numberOfPlanets; i++)
	{
		sf::Vector2<double> planetPos = static_cast<sf::Vector2<double>>(mySFML::createNormalVectorWithAngle(i * 2 * 3.141592653f / numberOfPlanets) * (100000000.f + 10000.f * myMath::randIntervalf(0, 1000)));
		//sf::Vector2<double> planetVel = static_cast<sf::Vector2<double>>(mySFML::randNormalVector() * 10000.f) * 0.0;
		sf::Vector2<double> planetVel = /*(static_cast<double>(myMath::randIntervali(1, 2)) - 1.5) * 2.0 * */mySFML::createOrthogonalVector(mySFML::normalize(planetPos)) * 2400.0;
		sf::Color color = sf::Color(myMath::randIntervali(0, 255), myMath::randIntervali(0, 255), myMath::randIntervali(0, 255));
		double mass = 1.0E23 * myMath::randIntervali(100, 1000) / 1000.0;
		mVectorOfPlanets.push_back(new Planet(mass, 4.0E03, planetPos, planetVel, color));
	}

}


//Destructor
Universe::~Universe()
{
	delete pTextTimeFactor;
	pTextTimeFactor = nullptr;

	delete pTextActualTimeFactor;
	pTextActualTimeFactor = nullptr;

	delete pTextFramesPerSecond;
	pTextFramesPerSecond = nullptr;

	for (auto planet : mVectorOfPlanets)
	{
		delete planet;
		planet = nullptr;
	}
}


//Update
void Universe::update(sf::RenderWindow *renderWindow, sf::Time frametime)
{
	//Choose Frametime or Fixed Time, depending on the Mode
	sf::Time usedTime;
	if (mFixedEvolveTimeMode)
	{
		usedTime = mFixedEvolveTime;
	}
	else
	{
		usedTime = frametime;
	}

	//User Input: Change Time Factor & Movements
	changeTimeFactor(usedTime);
	updateControlOfPlayer();

	//Determine several Forces
	determineGravityBetweenPlanets();
	//determineElasticForcesBetweenPlanets();
	//determineFriction();
	determineGravityForPlayer();

	//Update Actual Time Factor
	updateActualTimeFactor();

	//Evolve the Simulation
	evolveSimulation(renderWindow, usedTime * actualTimeFactor);

	//Manage Collisions
	manageCollisions(renderWindow, usedTime);

	//Fusion of planets
	manageFusions(usedTime * actualTimeFactor);
	normalizeDensities();
	
	//Update Render-Things
	updateView(renderWindow, frametime);
	updateTexts(renderWindow, frametime);

	//Measure the energy
	determineEnergy();
}


//Handle Events
void Universe::handleEvents(EventData eventData)
{
	for (auto planet : mVectorOfPlanets)
	{
		planet->handleEvents(eventData);
	}
}


//Render
void Universe::render(sf::RenderWindow *renderWindow)
{
	for (auto planet : mVectorOfPlanets)
	{
		planet->render(renderWindow);
	}
	pPlayer->render(renderWindow);
	renderWindow->draw(*pTextTimeFactor);
	renderWindow->draw(*pTextActualTimeFactor);
	renderWindow->draw(*pTextWindowSize);
	renderWindow->draw(*pTextFramesPerSecond);
}



///////////////////
//Private Functions

//Change Time Factor
void Universe::changeTimeFactor(sf::Time frametime)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Add))
	{
		timeFactor = timeFactor * pow(10.f, frametime.asSeconds());
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Subtract))
	{
		timeFactor = timeFactor / pow(10.f, frametime.asSeconds());
	}
}

//Determine Gravity Between Planets
void Universe::determineGravityBetweenPlanets()
{
	for (auto planet : mVectorOfPlanets)
	{
		float planetMass = planet->getMass();
		sf::Vector2<double> planetPos = planet->getPosition();
		planet->resetForces();
		for (auto gravitatingPlanet : mVectorOfPlanets)
		{
			if (gravitatingPlanet == planet)
			{
				continue;
			}
			sf::Vector2<double> distVec = gravitatingPlanet->getPosition() - planetPos;
			double distance = mySFML::lengthOf(distVec);
			if (distance != 0.0)
			{
				planet->addForce(G * planetMass * gravitatingPlanet->getMass() / distance / distance / distance * distVec);
			}
		}
	}
}

//Determine Elastic Forces between Planets
void Universe::determineElasticForcesBetweenPlanets()
{
	float elasticity = 1.0E19;
	for (std::vector<Planet*>::iterator planetIt1 = mVectorOfPlanets.begin(); planetIt1 != mVectorOfPlanets.end(); planetIt1++)
	{
		sf::Vector2<double> planet1Pos = (*planetIt1)->getPosition();
		double planet1Rad = (*planetIt1)->getRadius();
		for (std::vector<Planet*>::iterator planetIt2 = planetIt1; planetIt2 != mVectorOfPlanets.end(); planetIt2++)
		{
			if (planetIt1 == planetIt2)
			{
				continue;
			}
			sf::Vector2<double> distanceVector = planet1Pos - (*planetIt2)->getPosition();
			sf::Vector2<double> unitVector = mySFML::normalize(distanceVector);
			double distance = mySFML::lengthOf(distanceVector);
			double sumOfRadii = planet1Rad + (*planetIt2)->getRadius();
			if (distance < sumOfRadii)
			{
				double elasticForce = elasticity * (sumOfRadii - distance);
				(*planetIt1)->addForce(elasticForce * unitVector);
				(*planetIt2)->addForce(-elasticForce * unitVector);
			}
		}
	}
}

//Determine Friction
void Universe::determineFriction()
{
	double frictionCoefficient = 1.0E15;
	for (auto planet : mVectorOfPlanets)
	{
		sf::Vector2<double> velocity = planet->getVelocity();
		double velValue = mySFML::lengthOf(velocity);
		planet->addForce(-mySFML::normalize(velocity) * frictionCoefficient * velValue * velValue);
	}
}

//Determine Gravity for the Player
void Universe::determineGravityForPlayer()
{
	sf::Vector2<double> playerPos = pPlayer->getPosition();
	double playerMass = pPlayer->getMass();
	for (auto gravitatingPlanet : mVectorOfPlanets)
	{
		sf::Vector2<double> distVec = gravitatingPlanet->getPosition() - playerPos;
		double distance = mySFML::lengthOf(distVec);
		if (distance != 0.0)
		{
			pPlayer->addForce(G * playerMass * gravitatingPlanet->getMass() / distance / distance / distance * distVec);
		}
	}
}

//Update the Actual Time Factor
void Universe::updateActualTimeFactor()
{
	double biggestAcceleration = 0.0;
	for (auto planet : mVectorOfPlanets)
	{
		double acceleration = mySFML::lengthOf(planet->getForces()) / planet->getMass();
		if (acceleration > biggestAcceleration)
		{
			biggestAcceleration = acceleration;
		}
	}
	float const timeFactorConstant = 4.0E07;
	float maximalAllowedTimeFactor = timeFactorConstant / biggestAcceleration;
	actualTimeFactor = myMath::min(maximalAllowedTimeFactor, timeFactor);
	mEqualTimeFactors = (timeFactor == actualTimeFactor);
}

//Manage Collisions
void Universe::manageCollisions(sf::RenderWindow *renderWindow, sf::Time time)
{
	sf::Time timeStepOfLastEvolution = actualTimeFactor * time;
	bool collisionOccured = true;
	unsigned int const numberOfCollisionFixAttempts = 50;
	unsigned int counterOfCollisionFixAttempts = 0;
	bool infiniteNumberOfFixAttempts = true;
	while (collisionOccured && ((counterOfCollisionFixAttempts < numberOfCollisionFixAttempts) || infiniteNumberOfFixAttempts)) //Repeat till no collisions occurr anymore
	{
		counterOfCollisionFixAttempts++;
		collisionOccured = false;
		for (auto planetIt1 = mVectorOfPlanets.begin(); planetIt1 != mVectorOfPlanets.end(); planetIt1++)
		{
			double planet1Mass = (*planetIt1)->getMass();
			double planet1Rad = (*planetIt1)->getRadius();
			for (auto planetIt2 = planetIt1; planetIt2 != mVectorOfPlanets.end(); planetIt2++)
			{
				sf::Vector2<double> planet1Pos = (*planetIt1)->getPosition();
				//sf::Vector2<double> planet1LastPos = (*planetIt1)->getLastPosition();
				sf::Vector2<double> planet1Velocity = (*planetIt1)->getVelocity();
				sf::Vector2<double> planet1LastPos = planet1Pos - (static_cast<double>(timeStepOfLastEvolution.asSeconds()) * planet1Velocity);
				double planet2Mass = (*planetIt2)->getMass();
				double planet2Rad = (*planetIt2)->getRadius();
				sf::Vector2<double> planet2Pos = (*planetIt2)->getPosition();
				//sf::Vector2<double> planet2LastPos = (*planetIt2)->getLastPosition();
				sf::Vector2<double> planet2Velocity = (*planetIt2)->getVelocity();
				sf::Vector2<double> planet2LastPos = planet2Pos - (static_cast<double>(timeStepOfLastEvolution.asSeconds()) * planet2Velocity);
				if (planetIt1 == planetIt2)
				{
					continue;
				}
				sf::Vector2<double> distVector = planet1Pos - planet2Pos;
				double distance = mySFML::lengthOf(distVector); //This is a source of errors, because planets that overlap alternated after one evolution are very near
				sf::Vector2<double> relativeVelocity = planet1Velocity - planet2Velocity;
				double velocityProjectedOnDistance = mySFML::scalarProduct(distVector, relativeVelocity);
				bool comingCloser = (velocityProjectedOnDistance < 0);
				double sumOfRadii = planet1Rad + planet2Rad;
				if (distance <= (planet1Rad + planet2Rad) && comingCloser)
				{
					//std::cout << "Collision " << mDebugCounter << "!" << std::endl;
					//mySFML::outputOnTerminal((*planetIt1)->getPosition(), "Pos1: ", "\t");
					//mySFML::outputOnTerminal((*planetIt2)->getPosition(), "Pos2: ", "\t");
					mDebugCounter++;
					collisionOccured = true;

					//Determine time since collision
					sf::Vector2<double> lastDistance = planet1LastPos - planet2LastPos;
					double lastDistanceValue = mySFML::lengthOf(lastDistance);
					double relativeVelocityValue = mySFML::lengthOf(relativeVelocity);
					double scalarProd = mySFML::scalarProduct(lastDistance, relativeVelocity);
					sf::Time timeBetweenLastEvolutionAndCollision = -sf::seconds((scalarProd + sqrt(scalarProd * scalarProd + relativeVelocityValue * relativeVelocityValue * (sumOfRadii * sumOfRadii - lastDistanceValue * lastDistanceValue))) / (relativeVelocityValue * relativeVelocityValue)); //Big Error due to circle problem
					//sf::Time timeBetweenLastEvolutionAndCollision = sf::seconds(0.f);
					/*
					std::cout << "------------------------------------------------" << std::endl;
					std::cout << "relativeVelocityValue: " << relativeVelocityValue << std::endl;
					std::cout << "distance: " << (distance - sumOfRadii) << std::endl;
					std::cout << "timeStepOfLastEvolution: " << timeStepOfLastEvolution.asSeconds() << "\t" << "timeBetweenLastEvolutionAndCollision: " << timeBetweenLastEvolutionAndCollision.asSeconds() << std::endl;
					*/
					sf::Time timeSinceCollision = timeStepOfLastEvolution - timeBetweenLastEvolutionAndCollision;

					//Evolve backwards in time till collision
					evolveSimulation(renderWindow, -timeSinceCollision);

					//Execute collision algorithm
					sf::Vector2<double> rHat = mySFML::normalize(distVector);
					sf::Vector2<double> nHat = mySFML::createOrthogonalVector(rHat);

					double v1Para = mySFML::scalarProduct(planet1Velocity, rHat);
					double v1Orth = mySFML::scalarProduct(planet1Velocity, nHat);
					double v2Para = mySFML::scalarProduct(planet2Velocity, rHat);
					double v2Orth = mySFML::scalarProduct(planet2Velocity, nHat);

					//double v1ParaPrime = (planet1Mass - planet2Mass) / (planet1Mass + planet2Mass) * v1Para;
					double v1ParaPrime = (2 * ((planet1Mass * v1Para) + (planet2Mass * v2Para)) / (planet1Mass + planet2Mass)) - v1Para;
					double v1OrthPrime = v1Orth;
					//double v2ParaPrime = (planet2Mass - planet1Mass) / (planet2Mass + planet1Mass) * v2Para;
					double v2ParaPrime = (2 * ((planet1Mass * v1Para) + (planet2Mass * v2Para)) / (planet1Mass + planet2Mass)) - v2Para;
					double v2OrthPrime = v2Orth;
					
					sf::Vector2<double> v1Prime = (v1ParaPrime * rHat) + (v1OrthPrime * nHat);
					sf::Vector2<double> v2Prime = (v2ParaPrime * rHat) + (v2OrthPrime * nHat);


					//Friction during collision
					//Determine Momentum Transfer
					sf::Vector2<double> momentumTransfer = (v1Prime - planet1Velocity) * planet1Mass;
					double momentumTransferValue = mySFML::lengthOf(momentumTransfer);

					//Loss of momentum proportional to momentum transfer
					double const inelasticCollisionConstant = 0.15;
					double momentumLossValue = inelasticCollisionConstant * momentumTransferValue;

					sf::Vector2<double> momentumLoss1 = -momentumLossValue * mySFML::normalize(v1Prime);
					sf::Vector2<double> momentumLoss2 = -momentumLossValue * mySFML::normalize(v2Prime);

					(*planetIt1)->setVelocity(v1Prime + (momentumLoss1 / planet1Mass));
					(*planetIt2)->setVelocity(v2Prime + (momentumLoss2 / planet2Mass));

					double energyLoss1;
					double energyLoss2;


					//Evolve forwards in time till origin
					evolveSimulation(renderWindow, timeSinceCollision);


					//Color mixing
					(*planetIt1)->setColor(mySFML::mixColors((*planetIt1)->getColor(), (*planetIt2)->getColor(), 0.7, 0.3));
					(*planetIt2)->setColor(mySFML::mixColors((*planetIt1)->getColor(), (*planetIt2)->getColor(), 0.3, 0.7));
				}
			}
		}
	}
	if (counterOfCollisionFixAttempts != 1)
	{
		//std::cout << "Number of Fix Attempts: " << counterOfCollisionFixAttempts << std::endl;
	}
}


//Manage the Fusion of Planets
void Universe::manageFusions(sf::Time time)
{
	//Check for near planets
	unsigned int sizeOfVectorOfPlanets = mVectorOfPlanets.size();
	for (unsigned int planNum1 = 0; planNum1 < sizeOfVectorOfPlanets; planNum1++)
	{
		sf::Vector2<double> plan1Pos = mVectorOfPlanets.at(planNum1)->getPosition();
		double plan1Rad = mVectorOfPlanets.at(planNum1)->getRadius();
		unsigned int plan1UniqueNum = mVectorOfPlanets.at(planNum1)->getPlanetNumber();
		for (unsigned int planNum2 = (planNum1 + 1); planNum2 < sizeOfVectorOfPlanets; planNum2++)
		{
			sf::Vector2<double> plan2Pos = mVectorOfPlanets.at(planNum2)->getPosition();
			double plan2Rad = mVectorOfPlanets.at(planNum2)->getRadius();
			unsigned int plan2UniqueNum = mVectorOfPlanets.at(planNum2)->getPlanetNumber();
			double distance = mySFML::lengthOf(plan1Pos - plan2Pos);
			double fusionDistance = plan1Rad + plan2Rad + 0.05 * myMath::min(plan1Rad, plan2Rad);
			sf::Vector2u vectorOfTwoNearPlanets = sf::Vector2u(myMath::min(plan1UniqueNum, plan2UniqueNum), myMath::max(plan1UniqueNum, plan2UniqueNum));
			if (distance < fusionDistance)
			{
				//Search in mListOfNearPlanetsTimePairs for existing Pair and add time or Pair in case
				bool pairAlreadyExists = false;
				for (std::list<std::pair<sf::Vector2u, sf::Time>>::iterator nearPairListIt = mListOfNearPlanetsTimePairs.begin(); nearPairListIt != mListOfNearPlanetsTimePairs.end(); nearPairListIt++)
				{
					if ((*nearPairListIt).first == vectorOfTwoNearPlanets)
					{
						pairAlreadyExists = true;
						(*nearPairListIt).second = (*nearPairListIt).second + time;
						break;
					}
				}
				if (!pairAlreadyExists)
				{
					mListOfNearPlanetsTimePairs.push_back(std::pair<sf::Vector2u, sf::Time>(vectorOfTwoNearPlanets, time));
				}
			}
			else
			{
				//Search in mListOfNearPlanetsTimePairs for existing Pair and delete in case
				for (std::list<std::pair<sf::Vector2u, sf::Time>>::iterator nearPairListIt = mListOfNearPlanetsTimePairs.begin(); nearPairListIt != mListOfNearPlanetsTimePairs.end(); )
				{
					if ((*nearPairListIt).first == vectorOfTwoNearPlanets)
					{
						nearPairListIt = mListOfNearPlanetsTimePairs.erase(nearPairListIt);
						break;
					}
					else
					{
						nearPairListIt++;
					}
				}
			}
		}
	}

	//Merge two planets if a critical time is passed
	sf::Time const mergeTimeConstant = sf::seconds(600.f);
	while (true) //Repeat till no merges occur anymore!
	{
		bool planetsAreMerged = false;
		for (std::list<std::pair<sf::Vector2u, sf::Time>>::iterator nearPairListIt = mListOfNearPlanetsTimePairs.begin(); nearPairListIt != mListOfNearPlanetsTimePairs.end(); )
		{
			planetsAreMerged = false;
			Planet *newPlanetPointer = nullptr;
			unsigned int planet1UniqueNum;
			unsigned int planet2UniqueNum;
			if ((*nearPairListIt).second > mergeTimeConstant)
			{
				//Create a new Planet and delete the old ones
				planetsAreMerged = true;

				planet1UniqueNum = (*nearPairListIt).first.x;
				planet2UniqueNum = (*nearPairListIt).first.y;

				Planet *planet1UnsafePointer = nullptr;
				Planet *planet2UnsafePointer = nullptr;

				unsigned int sizeOfPlanetVector = mVectorOfPlanets.size();
				for (unsigned int planNum = 0; planNum < sizeOfPlanetVector; planNum++)
				{
					if (mVectorOfPlanets.at(planNum)->getPlanetNumber() == planet1UniqueNum)
					{
						planet1UnsafePointer = mVectorOfPlanets.at(planNum);
					}
					if (mVectorOfPlanets.at(planNum)->getPlanetNumber() == planet2UniqueNum)
					{
						planet2UnsafePointer = mVectorOfPlanets.at(planNum);
					}
				}

				double plan1Mass = planet1UnsafePointer->getMass();
				double plan2Mass = planet2UnsafePointer->getMass();

				double plan1Density = planet1UnsafePointer->getDensity();
				double plan2Density = planet2UnsafePointer->getDensity();

				double plan1Vol = planet1UnsafePointer->getVolume();
				double plan2Vol = planet2UnsafePointer->getVolume();

				sf::Vector2<double> plan1Pos = planet1UnsafePointer->getPosition();
				sf::Vector2<double> plan2Pos = planet2UnsafePointer->getPosition();

				sf::Vector2<double> plan1Vel = planet1UnsafePointer->getVelocity();
				sf::Vector2<double> plan2Vel = planet2UnsafePointer->getVelocity();

				sf::Color plan1Color = planet1UnsafePointer->getColor();
				sf::Color plan2Color = planet2UnsafePointer->getColor();


				double newMass = plan1Mass + plan2Mass;
				double newIntendedDensity = newMass / (plan1Vol + plan2Vol);
				sf::Vector2<double> newPosition = (plan1Mass / newMass) * plan1Pos + (plan2Mass / newMass) * plan2Pos;
				sf::Vector2<double> newVelocity = (plan1Mass / newMass) * plan1Vel + (plan2Mass / newMass) * plan2Vel;
				double newTemperature; //Determine EnergyLoss and mix of old temperatures
				sf::Color newColor = mySFML::mixColors(plan1Color, plan2Color, (plan1Mass / newMass), (plan2Mass / newMass));
				//Calculate newDensity such that no collisions happen!
				unsigned int size = mVectorOfPlanets.size();
				double minimalDistance;
				bool minimalDistanceValid = false;
				for (unsigned int planNum = 0; planNum < size; planNum++)
				{
					Planet *planetPointer = mVectorOfPlanets.at(planNum);
					if ((planetPointer == planet1UnsafePointer) || (planetPointer == planet2UnsafePointer))
					{
						continue;
					}
					double dist = myMath::abs(mySFML::lengthOf(planetPointer->getPosition() - newPosition) - planetPointer->getRadius());
					if (minimalDistanceValid)
					{
						if (dist < minimalDistance)
						{
							minimalDistance = dist;
						}
					}
					else
					{
						minimalDistance = dist;
						minimalDistanceValid = true;
					}
				}
				double newDensity;
				if (minimalDistanceValid)
				{
					newDensity = newMass / (4.0 / 3.0 * 3.141592653 * minimalDistance * minimalDistance * minimalDistance) * 2.0; //Factor 2.0 is only security!!!
				}
				else
				{
					newDensity = newIntendedDensity;
				}
				double usedNewDensity;
				bool wrongDensityUsed;
				if (newDensity > newIntendedDensity)
				{
					usedNewDensity = newDensity;
					wrongDensityUsed = true;
				}
				else
				{
					usedNewDensity = newIntendedDensity;
					wrongDensityUsed = false;
				}
				newPlanetPointer = new Planet(newMass, usedNewDensity, newPosition, newVelocity, newColor, newIntendedDensity);
				mVectorOfPlanets.push_back(newPlanetPointer);

				mListOfPlanetNumbersWithWrongDensity.push_back(newPlanetPointer->getPlanetNumber());

				delete planet1UnsafePointer;
				delete planet2UnsafePointer;
				planet1UnsafePointer = nullptr;
				planet2UnsafePointer = nullptr;

				for (std::vector<Planet*>::iterator planIt = mVectorOfPlanets.begin(); planIt != mVectorOfPlanets.end(); )
				{
					if (((*planIt)->getPlanetNumber() == planet1UniqueNum) || ((*planIt)->getPlanetNumber() == planet2UniqueNum))
					{
						planIt = mVectorOfPlanets.erase(planIt);
					}
					else
					{
						planIt++;
					}
				}

				nearPairListIt = mListOfNearPlanetsTimePairs.erase(nearPairListIt);
			}
			else
			{
				nearPairListIt++;
			}

			if (planetsAreMerged)
			{
				//Check if mListOfNearPlanetsTimePairs still contains the deleted planetNumbers and handle case
				unsigned int newPlanetUniqueNumber = newPlanetPointer->getPlanetNumber();
				for (auto pair : mListOfNearPlanetsTimePairs)
				{
					if ((pair.first.x == planet1UniqueNum) || (pair.first.x == planet2UniqueNum))
					{
						pair.first.x = newPlanetUniqueNumber;
					}
					if ((pair.first.y == planet1UniqueNum) || (pair.first.y == planet2UniqueNum))
					{
						pair.first.y = newPlanetUniqueNumber;
					}
				}

			}
		}

		//Exit if no planets are merged anymore
		if (!planetsAreMerged)
		{
			break;
		}
	}
}


//Normalize Densities
void Universe::normalizeDensities()
{
	unsigned int planVecSize = mVectorOfPlanets.size();
	for (std::list<unsigned int>::iterator wrongDensityPlanetsIt = mListOfPlanetNumbersWithWrongDensity.begin(); wrongDensityPlanetsIt != mListOfPlanetNumbersWithWrongDensity.end(); )
	{
		//Does Planet still exist?
		unsigned int uniquePlanNum = (*wrongDensityPlanetsIt);
		bool planetExists = false;
		Planet *planetPointer = nullptr;
		for (unsigned int planNum = 0; planNum < planVecSize; planNum++)
		{
			if (mVectorOfPlanets.at(planNum)->getPlanetNumber() == uniquePlanNum)
			{
				planetExists = true;
				planetPointer = mVectorOfPlanets.at(planNum);
				break;
			}
		}

		//If Planet exists, normalize Density; Else: Erase Number From List
		if (planetExists) //Normalize Density
		{
			double planRad = planetPointer->getRadius();
			sf::Vector2<double> planPos = planetPointer->getPosition();
			double planMass = planetPointer->getMass();
			double planDensity = planetPointer->getDensity();
			double planIntendedDensity = planetPointer->getIntendedDensity();
			double distanceMinusRadius;
			bool distanceMinusRadiusValid = false;
			for (unsigned int planNum = 0; planNum < planVecSize; planNum++)
			{
				Planet *otherPlanetPointer = mVectorOfPlanets.at(planNum);
				if (uniquePlanNum == otherPlanetPointer->getPlanetNumber())
				{
					continue;
				}
				double otherPlanRad = otherPlanetPointer->getRadius();
				sf::Vector2<double> otherPlanPos = otherPlanetPointer->getPosition();
				double distance = myMath::abs(mySFML::lengthOf(planPos - otherPlanPos) - otherPlanRad);
				if (distanceMinusRadiusValid)
				{
					distanceMinusRadius = myMath::min(distance, distanceMinusRadius);
				}
				else
				{
					distanceMinusRadius = distance;
					distanceMinusRadiusValid = true;
				}
			}
			double newPossibleDensity = planMass / (4.0 / 3.0 * 3.141592653 * distanceMinusRadius * distanceMinusRadius * distanceMinusRadius) * 1.01; //1.01: Security Factor
			double newDensity;
			bool maximalRadiusAchieved;
			if (newPossibleDensity > planIntendedDensity)
			{
				newDensity = newPossibleDensity;
				maximalRadiusAchieved = false;
			}
			else
			{
				newDensity = planIntendedDensity;
				maximalRadiusAchieved = true;
			}
			planetPointer->setDensity(newDensity);

			if (maximalRadiusAchieved)
			{
				wrongDensityPlanetsIt = mListOfPlanetNumbersWithWrongDensity.erase(wrongDensityPlanetsIt);
			}
			else
			{
				wrongDensityPlanetsIt++;
			}
		}
		else //Erase Number From the List
		{
			wrongDensityPlanetsIt = mListOfPlanetNumbersWithWrongDensity.erase(wrongDensityPlanetsIt);
		}
	}
}


//Update the Control of the Player (User Input)
void Universe::updateControlOfPlayer()
{
	pPlayer->resetForces();
	pPlayer->resetTorsionalMoments();
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
	{
		pPlayer->addForce(pPlayer->getOrientation() * 200000000.0);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
	{
		pPlayer->addTorsionalMoment(40.0);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
	{
		pPlayer->addTorsionalMoment(-40.0);
	}
}

//Update View
void Universe::updateView(sf::RenderWindow *renderWindow, sf::Time frametime)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::H))
	{
		pGlobalView->zoom(pow(10.f, frametime.asSeconds()));
		mViewAutoZoom = false;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Z))
	{
		pGlobalView->zoom(1.f / pow(10.f, frametime.asSeconds()));
		mViewAutoZoom = false;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
	{
		pGlobalView->move(sf::Vector2f(0.f, -1.f) * pGlobalView->getSize().y * frametime.asSeconds());
		mViewAutoPosition = false;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
	{
		pGlobalView->move(sf::Vector2f(0.f, 1.f) * pGlobalView->getSize().y * frametime.asSeconds());
		mViewAutoPosition = false;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
	{
		pGlobalView->move(sf::Vector2f(-1.f, 0.f) * pGlobalView->getSize().y * frametime.asSeconds());
		mViewAutoPosition = false;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
	{
		pGlobalView->move(sf::Vector2f(1.f, 0.f) * pGlobalView->getSize().y * frametime.asSeconds());
		mViewAutoPosition = false;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Return)) //Enter
	{
		mViewAutoZoom = true;
		mViewAutoPosition = true;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Period)) //.
	{
		mViewAutoZoom = true;
		mViewAutoPosition = true;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Comma)) //,
	{
		mViewAutoZoom = true;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
	{
		*pShowPlayerView = !(*pShowPlayerView);
	}
	if (*pShowPlayerView)
	{
		sf::Vector2i pixelPositionOfPlayer = renderWindow->mapCoordsToPixel(static_cast<sf::Vector2f>(pPlayer->getPosition()));
		sf::Vector2f ratioOfPixelPosAndWindowSize = sf::Vector2f(static_cast<float>(pixelPositionOfPlayer.x) / renderWindow->getSize().x, static_cast<float>(pixelPositionOfPlayer.y) / renderWindow->getSize().y);
		pPlayerView->setViewport(sf::FloatRect(ratioOfPixelPosAndWindowSize.x, ratioOfPixelPosAndWindowSize.y, 0.1, 0.1));
		pPlayerView->setCenter(static_cast<sf::Vector2f>(pPlayer->getPosition()));
		float autoViewSize = static_cast<float>(pPlayer->getTextureSize().x * pPlayer->getSpriteScale().x) * 5.f;
		pPlayerView->setSize(sf::Vector2f(autoViewSize, autoViewSize * pGlobalView->getSize().y / pGlobalView->getSize().x));
	}
	if (mViewAutoZoom)
	{
		*pShowPlayerView = false;
		float autoViewSize = static_cast<float>(pPlayer->getTextureSize().x * pPlayer->getSpriteScale().x) * 5.f;
		*pGlobalView = sf::View(pGlobalView->getCenter(), sf::Vector2f(autoViewSize, autoViewSize * pGlobalView->getSize().y / pGlobalView->getSize().x));
	}
	if (mViewAutoPosition)
	{
		*pGlobalView = sf::View(static_cast<sf::Vector2f>(pPlayer->getPosition()), pGlobalView->getSize());
	}
	//pGlobalView->setViewport(sf::FloatRect(0.25, 0.25, 0.5, 0.5));
}

//Update Texts
void Universe::updateTexts(sf::RenderWindow *renderWindow, sf::Time frametime)
{
	float scale = pGlobalView->getSize().x / static_cast<float>(renderWindow->getSize().x);

	pTextTimeFactor->setString("Time Factor: " + std::to_string(timeFactor));
	pTextTimeFactor->setScale(scale, scale);
	pTextTimeFactor->setPosition(renderWindow->mapPixelToCoords(sf::Vector2i(7, 4)));

	pTextActualTimeFactor->setString("Actual Time Factor: " + std::to_string(actualTimeFactor));
	pTextActualTimeFactor->setScale(scale, scale);
	pTextActualTimeFactor->setPosition(renderWindow->mapPixelToCoords(sf::Vector2i(7, 29)));
	if (mEqualTimeFactors)
	{
		pTextActualTimeFactor->setColor(sf::Color::White);
	}
	else
	{
		pTextActualTimeFactor->setColor(sf::Color::Red);
	}

	pTextWindowSize->setString("Window Size: " + std::to_string(pGlobalView->getSize().x / 1000.f) + " km");
	pTextWindowSize->setScale(scale, scale);
	pTextWindowSize->setPosition(renderWindow->mapPixelToCoords(sf::Vector2i(7, 54)));

	pTextFramesPerSecond->setString("Frames per Second: " + std::to_string(1.f / frametime.asSeconds()));
	pTextFramesPerSecond->setScale(scale, scale);
	pTextFramesPerSecond->setPosition(renderWindow->mapPixelToCoords(sf::Vector2i(7, 79)));
}

//Evolve Simulation
void Universe::evolveSimulation(sf::RenderWindow *renderWindow, sf::Time time)
{
	//std::cout << time.asMilliseconds() << std::endl;
	for (auto planet : mVectorOfPlanets)
	{
		planet->update(renderWindow, time);
	}
	pPlayer->update(renderWindow, time);
}

//Determine Energy
void Universe::determineEnergy()
{
	double energy = 0.0;
	unsigned int sizeOfVector = mVectorOfPlanets.size();
	for (unsigned int planetNumber1 = 0; planetNumber1 < sizeOfVector; planetNumber1++)
	{
		//Kinetic Energy
		double planet1Mass = mVectorOfPlanets.at(planetNumber1)->getMass();
		double planet1VelVal = mySFML::lengthOf(mVectorOfPlanets.at(planetNumber1)->getVelocity());
		sf::Vector2<double> planet1Pos = mVectorOfPlanets.at(planetNumber1)->getPosition();
		energy = energy + (planet1Mass / 2.0 * planet1VelVal * planet1VelVal);

		//Potential Energy
		for (unsigned int planetNumber2 = 0; planetNumber2 < sizeOfVector; planetNumber2++)
		{
			if (planetNumber1 == planetNumber2)
			{
				continue;
			}
			double planet2Mass = mVectorOfPlanets.at(planetNumber2)->getMass();
			sf::Vector2<double> planet2Pos = mVectorOfPlanets.at(planetNumber2)->getPosition();
			energy = energy - (G * planet1Mass * planet2Mass / mySFML::lengthOf(planet2Pos - planet1Pos));
		}
	}
	//std::cout << "Energy: " << energy << std::endl;
	if (mEnergiesValid)
	{
		mLastEnergy = mActualEnergy;
		mActualEnergy = energy;
		if ((mActualEnergy - mLastEnergy) > 1.0E29)
		{
			std::cout << "Error!!!" << std::endl;
		}
	}
	else
	{
		mActualEnergy = energy;
		mLastEnergy = energy;
		mEnergiesValid = true;
	}
}