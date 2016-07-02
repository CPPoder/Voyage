#include "stdafx.h"
#include "Button.hpp"


//Constructors
Button::Button()
	: mButtonPressed(false),
	  mPosition(sf::Vector2i()),
	  mSize(sf::Vector2u())
{
	pSprite = new sf::Sprite;
	pText = new sf::Text;
}

Button::Button(sf::Texture *texture)
	: Button::Button()
{
	pSprite->setTexture(*texture);
}

Button::Button(sf::Texture *texture, sf::Vector2i position, sf::Vector2u size, sf::Font *font, std::string text, unsigned int textSize)
	: Button::Button(texture)
{
	mPosition = position;
	mSize = size;
	pSprite->setTextureRect(sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(static_cast<sf::Vector2i>(mSize))));
	pSprite->setPosition(static_cast<sf::Vector2f>(position));
	pFont = font;
	pText->setFont(*font);
	pText->setString(text);
	pText->setCharacterSize(textSize);
	pText->setOrigin((pText->getGlobalBounds().left + pText->getGlobalBounds().width) / 2.f, (pText->getGlobalBounds().top + pText->getGlobalBounds().height) / 2.f * 1.2f);
	pText->setPosition(mySFML::meanVector(static_cast<sf::Vector2f>(mPosition), static_cast<sf::Vector2f>(mPosition) + static_cast<sf::Vector2f>(mSize)));
	
}


//Destructor
Button::~Button()
{
	delete pSprite;
	pSprite = nullptr;

	delete pText;
	pText = nullptr;
}


//Update, HandleEvents, Render
void Button::update(sf::RenderWindow *renderWindow, sf::Time frametime)
{
	//Determine, if the Button is pressed
	bool mousePressed = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);
	bool mouseOverButton = (mySFML::smaller(mPosition, sf::Mouse::getPosition(*renderWindow)) && mySFML::smaller(sf::Mouse::getPosition(*renderWindow), mPosition + static_cast<sf::Vector2i>(mSize)));
	if (mousePressed && mouseOverButton)
	{
		mButtonPressed = true;
	}
	else
	{
		mButtonPressed = false;
	}

	//Change Rectangle, if Button is pressed
	if (mButtonPressed)
	{
		pSprite->setTextureRect(sf::IntRect(sf::Vector2i(0, static_cast<int>(mSize.y)), sf::Vector2i(static_cast<sf::Vector2i>(mSize))));
	}
	else
	{
		pSprite->setTextureRect(sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(static_cast<sf::Vector2i>(mSize))));
	}
}

void Button::handleEvents(EventData eventData)
{
	if (eventData.mouseButtonIsReleased && (eventData.mouseButton == sf::Mouse::Button::Left) && (mySFML::smaller(mPosition, eventData.mousePosition) && mySFML::smaller(eventData.mousePosition, mPosition + static_cast<sf::Vector2i>(mSize))))
	{
		mButtonReleased = true;
	}
	else
	{
		mButtonReleased = false;
	}
}

void Button::render(sf::RenderWindow *renderWindow)
{
	renderWindow->draw(*pSprite);
	renderWindow->draw(*pText);
}


//Getter
bool Button::getIsReleased() const
{
	return mButtonReleased;
}