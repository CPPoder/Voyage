#include "stdafx.h"
#include "Planet.hpp"

//Initialize static variables
unsigned int Planet::sPlayerNumber = 0;


//Constructor
Planet::Planet(double mass, double density, sf::Vector2<double> position, sf::Vector2<double> velocity, sf::Color color)
	: mMass(mass),
	  mDensity(density),
	  mRadius(pow(3.0/4.0/3.141592653*mass/density, 1.0/3.0)),
	  mPosition(position),
	  mVelocity(velocity),
	  mAcceleration(sf::Vector2<double>(0.0, 0.0)),
	  mForces(sf::Vector2<double>(0.0, 0.0)),
	  mLastPosition(mPosition),
	  mPlanetNumber(sPlayerNumber)
{
	pCircleShape = new sf::CircleShape;
	pCircleShape->setFillColor(color);
	pCircleShape->setRadius(mRadius);
	pCircleShape->setOrigin(pCircleShape->getRadius(), pCircleShape->getRadius());
	pCircleShape->setPosition(static_cast<sf::Vector2f>(mPosition));
	pCircleShape->setPointCount(400);
	sPlayerNumber++;
}

//Destructor
Planet::~Planet()
{
	delete pCircleShape;
	pCircleShape = nullptr;
}

//Update
void Planet::update(sf::RenderWindow *renderWindow, sf::Time frametime)
{
	mLastPosition = mPosition;
	mAcceleration = mForces / mMass;
	mVelocity = mVelocity + mAcceleration * static_cast<double>(frametime.asSeconds());
	mPosition = mPosition + mVelocity * static_cast<double>(frametime.asSeconds());
	pCircleShape->setPosition(static_cast<sf::Vector2f>(mPosition));
}

//Handle Events
void Planet::handleEvents(EventData eventData)
{
}

//Render
void Planet::render(sf::RenderWindow *renderWindow)
{
	renderWindow->draw(*pCircleShape);
}

//Getter
double Planet::getMass() const
{
	return mMass;
}
double Planet::getRadius() const
{
	return mRadius;
}
double Planet::getDensity() const
{
	return mDensity;
}
sf::Vector2<double> Planet::getPosition() const
{
	return mPosition;
}
sf::Vector2<double> Planet::getVelocity() const
{
	return mVelocity;
}
sf::Vector2<double> Planet::getAcceleration() const
{
	return mAcceleration;
}
sf::Vector2<double> Planet::getForces() const
{
	return mForces;
}
sf::Vector2<double> Planet::getLastPosition() const
{
	return mLastPosition;
}
unsigned int Planet::getPlanetNumber() const
{
	return mPlanetNumber;
}
sf::Color Planet::getColor() const
{
	return pCircleShape->getFillColor();
}

//Setter
void Planet::resetForces()
{
	mForces = sf::Vector2<double>(0.0, 0.0);
}
void Planet::setForces(sf::Vector2<double> forces)
{
	mForces = forces;
}
void Planet::addForce(sf::Vector2<double> force)
{
	mForces = mForces + force;
}
void Planet::setVelocity(sf::Vector2<double> velocity)
{
	mVelocity = velocity;
}
void Planet::invertVelocity()
{
	mVelocity = -mVelocity;
}
void Planet::setColor(sf::Color color)
{
	pCircleShape->setFillColor(color);
}