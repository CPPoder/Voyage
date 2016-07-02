#ifndef UNIVERSE_HPP
#define UNIVERSE_HPP


#include "SFML\Graphics.hpp"

#include <iostream>
#include <list>
#include <vector>
#include <utility>
#include <math.h>

#include "myUsefulMath.hpp"
#include "mySFMLVectorFunctions.hpp"

#include "Element.hpp"
#include "Planet.hpp"
#include "Player.hpp"


class Universe : public Element
{
private:
	sf::Font *pFont;
	sf::Texture *pPlayerTexture;

	sf::View *pGlobalView;
	sf::View *pPlayerView;
	bool *pShowPlayerView;

	double const G = 6.67408E-11;
	float timeFactor = 1.f;
	float actualTimeFactor = timeFactor;
	bool mEqualTimeFactors = true;

	std::list<Planet*> mListOfPlanets;
	Player *pPlayer;

	sf::Text *pTextTimeFactor;
	sf::Text *pTextActualTimeFactor;
	sf::Text *pTextWindowSize;
	sf::Text *pTextFramesPerSecond;

	bool mViewAutoZoom = false;
	bool mViewAutoPosition = false;

	unsigned int mDebugCounter = 0;

public:
	Universe(sf::View *globalView, sf::View *playerView, bool *showPlayerView, sf::Font *font, sf::Texture *playerTexture);
	~Universe();

	void update(sf::RenderWindow *renderWindow, sf::Time frametime) override;
	void handleEvents(EventData eventData) override;
	void render(sf::RenderWindow *renderWindow) override;

private:
	void changeTimeFactor(sf::Time frametime);
	void determineGravityBetweenPlanets();
	void determineElasticForcesBetweenPlanets();
	void determineFriction();
	void determineGravityForPlayer();
	void updateActualTimeFactor();
	void manageCollisions(sf::RenderWindow *renderWindow, sf::Time frametime);
	void updateControlOfPlayer();
	void updateView(sf::RenderWindow *renderWindow, sf::Time frametime);
	void updateTexts(sf::RenderWindow *renderWindow, sf::Time frametime);
	void evolveSimulation(sf::RenderWindow *renderWindow, sf::Time time);


};






#endif