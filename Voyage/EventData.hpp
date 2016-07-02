#ifndef EVENTDATA_HPP
#define EVENTDATA_HPP

#include <iostream>

#include "SFML\Graphics.hpp"


class EventData
{
private:


public:
	EventData();
	EventData(bool _mouseButtonIsReleased, sf::Mouse::Button _mouseButton, sf::Vector2i _mousePosition);
	EventData(bool _keyIsReleased, sf::Keyboard::Key _key);
	EventData(bool _mouseButtonIsReleased, sf::Mouse::Button _mouseButton, sf::Vector2i _mousePosition, bool _keyIsReleased, sf::Keyboard::Key _key);
	
	EventData(EventData const &eventData);


	~EventData();


	bool mouseButtonIsReleased;
	sf::Mouse::Button mouseButton;
	sf::Vector2i mousePosition;

	bool keyIsReleased;
	sf::Keyboard::Key key;


	void setMouseButtonReleased(sf::Mouse::Button _mouseButton, sf::Vector2i _mousePosition);
	void setKeyReleased(sf::Keyboard::Key _key);
	void reset();

	void outputOnShell() const;
	
};




#endif