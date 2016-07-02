#ifndef ELEMENT_HPP
#define ELEMENT_HPP

#include "SFML\Graphics.hpp"

#include "EventData.hpp"

#include <iostream>

class Element
{
private:

public:
	Element();
	virtual ~Element();
	
	virtual void update(sf::RenderWindow *renderWindow, sf::Time frametime);
	virtual void handleEvents(EventData eventData);
	virtual void render(sf::RenderWindow *renderWindow);
};


#endif