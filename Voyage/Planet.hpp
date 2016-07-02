#ifndef PLANET_HPP
#define PLANET_HPP


#include "SFML\Graphics.hpp"

#include <iostream>

#include "Element.hpp"


class Planet : public Element
{
private:
	double mMass;
	double mDensity;
	double mRadius;

	sf::Vector2<double> mPosition;
	sf::Vector2<double> mVelocity;
	sf::Vector2<double> mAcceleration;

	sf::Vector2<double> mForces;

	sf::CircleShape *pCircleShape;

	sf::Vector2<double> mLastPosition;

	static unsigned int sPlayerNumber;
	unsigned int mPlanetNumber;

public:
	Planet(double mass, double density, sf::Vector2<double> position, sf::Vector2<double> velocity, sf::Color color);
	~Planet();

	void update(sf::RenderWindow *renderWindow, sf::Time frametime) override;
	void handleEvents(EventData eventData) override;
	void render(sf::RenderWindow *renderWindow) override;

	double getMass() const;
	double getRadius() const;
	sf::Vector2<double> getPosition() const;
	sf::Vector2<double> getVelocity() const;
	sf::Vector2<double> getAcceleration() const;
	sf::Vector2<double> getForces() const;
	sf::Vector2<double> getLastPosition() const;
	unsigned int getPlanetNumber() const;

	void resetForces();
	void setForces(sf::Vector2<double> forces);
	void addForce(sf::Vector2<double> force);
	void setVelocity(sf::Vector2<double> velocity);
	void invertVelocity();


};






#endif