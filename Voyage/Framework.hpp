#ifndef FRAMEWORK_HPP
#define FRAMEWORK_HPP

#include <iostream>
#include "SFML\Graphics.hpp"

#include "Universe.hpp"


class Framework
{
private:
	sf::Font *pFontArial;
	sf::Texture *pPlayerTexture;

	sf::RenderWindow *pRenderWindow;

	sf::View *pGlobalView;
	sf::View *pPlayerView;
	bool mShowPlayerView;

	sf::Clock *pClock;

	Universe *pUniverse;

	void handleEvents();
	void update();
	void render();


public:
	Framework();
	~Framework();

	void run();

};



#endif