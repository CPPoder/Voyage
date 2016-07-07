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
	mVectorOfPlanets.push_back(new Planet(7.349E22/* * 1.0E05*/, 3.341E03, sf::Vector2<double>(3.844E08, 0.0), sf::Vector2<double>(0.0, 1.023E03), sf::Color(120, 120, 120)));
	
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
		mVectorOfPlanets.push_back(new Planet(1.0E23, 4.0E03, static_cast<sf::Vector2<double>>(mySFML::createNormalVectorWithAngle(i * 2 * 3.141592653f / numberOfPlanets) * (100000000.f + 10000.f * myMath::randIntervalf(0, 1000))), static_cast<sf::Vector2<double>>(mySFML::randNormalVector() * 10000.f) * 0.0, sf::Color::White));
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
	determineFriction();
	determineGravityForPlayer();

	//Update Actual Time Factor
	updateActualTimeFactor();

	//Evolve the Simulation
	evolveSimulation(renderWindow, usedTime * actualTimeFactor);

	//Manage Collisions
	manageCollisions(renderWindow, usedTime);
	
	//Update Render-Things
	updateView(renderWindow, frametime);
	updateTexts(renderWindow, frametime);
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
	double frictionCoefficient = 1.0E16;
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
				sf::Vector2<double> planet1LastPos = (*planetIt1)->getLastPosition();
				sf::Vector2<double> planet1Velocity = (*planetIt1)->getVelocity();
				double planet2Mass = (*planetIt2)->getMass();
				double planet2Rad = (*planetIt2)->getRadius();
				sf::Vector2<double> planet2Pos = (*planetIt2)->getPosition();
				sf::Vector2<double> planet2LastPos = (*planetIt2)->getLastPosition();
				sf::Vector2<double> planet2Velocity = (*planetIt2)->getVelocity();
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
					sf::Time timeStepOfLastEvolution = actualTimeFactor * time;
					sf::Vector2<double> lastDistance = planet1LastPos - planet2LastPos;
					double lastDistanceValue = mySFML::lengthOf(lastDistance);
					double relativeVelocityValue = mySFML::lengthOf(relativeVelocity);
					double scalarProd = mySFML::scalarProduct(lastDistance, relativeVelocity);
					sf::Time timeBetweenLastEvolutionAndCollision = sf::seconds(-(scalarProd + sqrt(scalarProd * scalarProd + relativeVelocityValue * relativeVelocityValue * (sumOfRadii * sumOfRadii - lastDistanceValue * lastDistanceValue))) / (relativeVelocityValue * relativeVelocityValue)); //Big Error due to circle problem
					//sf::Time timeBetweenLastEvolutionAndCollision = sf::seconds(0.f);
					//std::cout << "timeStepOfLastEvolution: " << timeStepOfLastEvolution.asSeconds() << "\t" << "timeBetweenLastEvolutionAndCollision: " << timeBetweenLastEvolutionAndCollision.asSeconds() << std::endl;
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

					(*planetIt1)->setVelocity(v1Prime);
					(*planetIt2)->setVelocity(v2Prime);

					//Evolve forwards in time till origin
					evolveSimulation(renderWindow, timeSinceCollision);

					//Friction during collision
					//(*planetIt1)->setVelocity((*planetIt1)->getVelocity() * 0.95);
					//(*planetIt2)->setVelocity((*planetIt2)->getVelocity() * 0.95);
				}
			}
		}
	}
	//std::cout << "Number of Fix Attempts: " << counterOfCollisionFixAttempts << std::endl;
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