#include "stdafx.h"
#include "Player.hpp"


//Constructor
Player::Player(double mass, double momentOfInertia, sf::Vector2<double> position, sf::Vector2<double> velocity, double angle, double angularFrequency, sf::Texture *playerTexture)
	: mMass(mass),
	  mMomentOfInertia(momentOfInertia),
	  mPosition(position),
	  mAngle(angle),
	  mVelocity(velocity),
	  mAngularFrequency(angularFrequency),
	  mAcceleration(sf::Vector2<double>(0.0, 0.0)),
	  mAngularAcceleration(0.0),
	  mForces(sf::Vector2<double>(0.0, 0.0)),
	  mTorsionalMoments(0.0),
	  mTextureSize(playerTexture->getSize()),
	  mNumberOfRectangles(2, 4),
	  mRectangleSize(mTextureSize.x / mNumberOfRectangles.x, mTextureSize.y / mNumberOfRectangles.y),
	  mLengthOfSpaceship(200.f),
	  mPositionBackup(mPosition),
	  mVelocityBackup(mVelocity)
{
	pPlayerTexture = playerTexture;
	pPlayerSprite = new sf::Sprite;
	pPlayerSprite->setTexture(*pPlayerTexture);
	pPlayerSprite->setOrigin(static_cast<sf::Vector2f>(mRectangleSize) / 2.f);
	pPlayerSprite->setPosition(static_cast<sf::Vector2f>(mPosition));
	pPlayerSprite->setRotation(static_cast<float>(mAngle));
	float scaleOfSpaceship = mLengthOfSpaceship / 150; //150: number of pixels in y direction
	pPlayerSprite->setScale(scaleOfSpaceship, scaleOfSpaceship);
}

//Destructor
Player::~Player()
{
	delete pPlayerSprite;
	pPlayerSprite = nullptr;
}

//Update
void Player::update(sf::RenderWindow *renderWindow, sf::Time frametime)
{
	//Update Rectangle
	sf::Vector2i rectanglePos(0, 0);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
	{
		rectanglePos.x = 1;
	}
	else
	{
		rectanglePos.x = 0;
	}
	bool keyDPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D);
	bool keyAPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A);
	if (!keyAPressed && !keyDPressed)
	{
		rectanglePos.y = 0;
	}
	if (!keyAPressed && keyDPressed)
	{
		rectanglePos.y = 1;
	}
	if (keyAPressed && !keyDPressed)
	{
		rectanglePos.y = 2;
	}
	if (keyAPressed && keyDPressed)
	{
		rectanglePos.y = 3;
	}
	pPlayerSprite->setTextureRect(sf::IntRect(mySFML::compMult(rectanglePos, mRectangleSize), static_cast<sf::Vector2i>(mRectangleSize)));
	
	//Update Position
	mAcceleration = mForces / mMass;
	mVelocity = mVelocity + mAcceleration * static_cast<double>(frametime.asSeconds());
	mPosition = mPosition + mVelocity * static_cast<double>(frametime.asSeconds());
	pPlayerSprite->setPosition(static_cast<sf::Vector2f>(mPosition));

	//Update Rotation
	mAngularAcceleration = mTorsionalMoments / mMomentOfInertia;
	mAngularFrequency = mAngularFrequency + mAngularAcceleration * static_cast<double>(frametime.asSeconds());
	mAngle = mAngle + mAngularFrequency * static_cast<double>(frametime.asSeconds());
	pPlayerSprite->setRotation(mAngle);
	//std::cout << "Angle: " << mAngle << std::endl;
}

//Handle Events
void Player::handleEvents(EventData eventData)
{
}

//Render
void Player::render(sf::RenderWindow *renderWindow)
{
	renderWindow->draw(*pPlayerSprite);
}

//Getter
double Player::getMass() const
{
	return mMass;
}
sf::Vector2<double> Player::getPosition() const
{
	return mPosition;
}
sf::Vector2<double> Player::getVelocity() const
{
	return mVelocity;
}
sf::Vector2<double> Player::getAcceleration() const
{
	return mAcceleration;
}
sf::Vector2<double> Player::getForces() const
{
	return mForces;
}

double Player::getMomentOfInertia() const
{
	return mMomentOfInertia;
}
double Player::getAngle() const
{
	return mAngle;
}
double Player::getAngularFrequency() const
{
	return mAngularFrequency;
}
double Player::getAngularAcceleration() const
{
	return mAngularAcceleration;
}
double Player::getTorsionalMoments() const
{
	return mTorsionalMoments;
}

sf::Vector2<double> Player::getOrientation() const
{
	//std::cout << "c: " << cos(mAngle / 180.0 * 3.141592653) << std::endl;
	//std::cout << "s: " << sin(mAngle / 180.0 * 3.141592653) << std::endl;
	double c = cos(mAngle / 180.0 * 3.141592653);
	double s = sin(mAngle / 180.0 * 3.141592653);

	return sf::Vector2<double>(s, -c);
}
sf::Vector2u Player::getTextureSize() const
{
	return pPlayerTexture->getSize();
}
sf::Vector2f Player::getSpriteScale() const
{
	return pPlayerSprite->getScale();
}

//Setter
void Player::resetForces()
{
	mForces = sf::Vector2<double>(0.0, 0.0);
}
void Player::setForces(sf::Vector2<double> forces)
{
	mForces = forces;
}
void Player::addForce(sf::Vector2<double> force)
{
	mForces = mForces + force;
}

void Player::resetTorsionalMoments()
{
	mTorsionalMoments = 0.0;
}
void Player::setTorsionalMoments(double torsionalMoments)
{
	mTorsionalMoments = torsionalMoments;
}
void Player::addTorsionalMoment(double torsionalMoment)
{
	mTorsionalMoments = mTorsionalMoments + torsionalMoment;
}


//Create Backup
void Player::createBackup()
{
	mPositionBackup = mPosition;
	mVelocityBackup = mVelocity;
}

//Set Back with Backup
void Player::setBack()
{
	mPosition = mPositionBackup;
	mVelocity = mVelocityBackup;
}