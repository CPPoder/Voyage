#ifndef PLAYER_HPP
#define PLAYER_HPP


#include "SFML\Graphics.hpp"

#include <iostream>

#include "Element.hpp"

#include "mySFMLVectorFunctions.hpp"


class Player : public Element
{
private:
	double mMass;
	double mMomentOfInertia;

	sf::Vector2<double> mPosition;
	double mAngle;

	sf::Vector2<double> mVelocity;
	double mAngularFrequency;

	sf::Vector2<double> mAcceleration;
	double mAngularAcceleration;

	sf::Vector2<double> mForces;
	double mTorsionalMoments;

	sf::Texture *pPlayerTexture;
	sf::Sprite *pPlayerSprite;
	sf::Vector2u mTextureSize;
	sf::Vector2u mNumberOfRectangles;
	sf::Vector2u mRectangleSize;
	float mLengthOfSpaceship;

	sf::Vector2<double> mPositionBackup;
	sf::Vector2<double> mVelocityBackup;

public:
	Player(double mass, double momentOfInertia, sf::Vector2<double> position, sf::Vector2<double> velocity, double angle, double angularFrequency, sf::Texture *playerTexture);
	~Player();

	void update(sf::RenderWindow *renderWindow, sf::Time frametime) override;
	void handleEvents(EventData eventData) override;
	void render(sf::RenderWindow *renderWindow) override;

	double getMass() const;
	sf::Vector2<double> getPosition() const;
	sf::Vector2<double> getVelocity() const;
	sf::Vector2<double> getAcceleration() const;
	sf::Vector2<double> getForces() const;

	double getMomentOfInertia() const;
	double getAngle() const;
	double getAngularFrequency() const;
	double getAngularAcceleration() const;
	double getTorsionalMoments() const;

	sf::Vector2<double> getOrientation() const;
	sf::Vector2u getTextureSize() const;
	sf::Vector2f getSpriteScale() const;

	void resetForces();
	void setForces(sf::Vector2<double> forces);
	void addForce(sf::Vector2<double> force);
	void createBackup();
	void setBack();

	void resetTorsionalMoments();
	void setTorsionalMoments(double torsionalMoments);
	void addTorsionalMoment(double torsionalMoment);
	
};






#endif