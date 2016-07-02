#include "stdafx.h"
#include "EventData.hpp"

//Standard Constructor
EventData::EventData()
	: mouseButtonIsReleased(false),
	  mouseButton(),
	  mousePosition(),
	  keyIsReleased(false),
	  key()
{
}


//Advanced Constructors
EventData::EventData(bool _mouseButtonIsReleased, sf::Mouse::Button _mouseButton, sf::Vector2i _mousePosition)
	: mouseButtonIsReleased(_mouseButtonIsReleased),
	mouseButton(_mouseButton),
	mousePosition(_mousePosition),
	keyIsReleased(false),
	key()
{
}

EventData::EventData(bool _keyIsReleased, sf::Keyboard::Key _key)
	: mouseButtonIsReleased(false),
	mouseButton(),
	mousePosition(),
	keyIsReleased(_keyIsReleased),
	key(_key)
{
}

EventData::EventData(bool _mouseButtonIsReleased, sf::Mouse::Button _mouseButton, sf::Vector2i _mousePosition, bool _keyIsReleased, sf::Keyboard::Key _key)
	: mouseButtonIsReleased(_mouseButtonIsReleased),
	mouseButton(_mouseButton),
	mousePosition(_mousePosition),
	keyIsReleased(_keyIsReleased),
	key(_key)
{
}


//Copy Constructor
EventData::EventData(EventData const &eventData)
	: mouseButtonIsReleased(eventData.mouseButtonIsReleased),
	  mouseButton(eventData.mouseButton),
	  mousePosition(eventData.mousePosition),
	  keyIsReleased(eventData.keyIsReleased),
	  key(eventData.key)
{
}


//Destructor
EventData::~EventData()
{
}


//Setter
void EventData::setMouseButtonReleased(sf::Mouse::Button _mouseButton, sf::Vector2i _mousePosition)
{
	mouseButtonIsReleased = true;
	mouseButton = _mouseButton;
	mousePosition = _mousePosition;
}

void EventData::setKeyReleased(sf::Keyboard::Key _key)
{
	keyIsReleased = true;
	key = _key;
}

void EventData::reset()
{
	mouseButtonIsReleased = false;
	keyIsReleased = false;
}


//Output on shell
void EventData::outputOnShell() const
{
	std::cout << "mouseButtonIsReleased: " << mouseButtonIsReleased << " keyIsReleased: " << keyIsReleased << std::endl;
}