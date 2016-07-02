#include "stdafx.h"
#include "CheckBox.hpp"


//Constructors
CheckBox::CheckBox()
	: mPosition(sf::Vector2i()),
	  mSize(sf::Vector2u())
{
	pSprite = new sf::Sprite;
	pText = new sf::Text;
}

CheckBox::CheckBox(sf::Texture *texture)
	: CheckBox::CheckBox()
{
	pSprite->setTexture(*texture);
}

CheckBox::CheckBox(sf::Texture *texture, sf::Vector2i position, sf::Vector2u size, bool active, bool chosen, bool autonomic, sf::Font *font, std::string text, sf::Vector2i textPosition, unsigned int textSize, sf::Color textColor)
	: CheckBox::CheckBox(texture)
{
	mActive = active;
	mChosen = chosen;
	mAutonomic = autonomic;
	mPosition = position;
	mSize = size;
	pSprite->setTextureRect(sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(static_cast<sf::Vector2i>(mSize))));
	updateImage();
	pSprite->setPosition(static_cast<sf::Vector2f>(position));
	pFont = font;
	pText->setFont(*font);
	pText->setColor(textColor);
	pText->setString(text);
	pText->setCharacterSize(textSize);
	//pText->setOrigin((pText->getGlobalBounds().left + pText->getGlobalBounds().width) / 2.f, (pText->getGlobalBounds().top + pText->getGlobalBounds().height) / 2.f * 1.2f);
	pText->setPosition(static_cast<sf::Vector2f>(textPosition));
	
}


//Destructor
CheckBox::~CheckBox()
{
	delete pSprite;
	pSprite = nullptr;

	delete pText;
	pText = nullptr;
}


//Update, HandleEvents, Render
void CheckBox::update(sf::RenderWindow *renderWindow, sf::Time frametime)
{
	//If autonomic: Change mChosen
	if (mAutonomic && mIsReleased)
	{
		mChosenChanged = true;
		mChosen = !mChosen;
		mIsReleased = false;
	}

	//Change Rectangle, if Chosen or Active changed
	if (mChosenChanged || mActiveChanged)
	{
		updateImage();
		mChosenChanged = false;
		mActiveChanged = false;
	}
}

void CheckBox::handleEvents(EventData eventData)
{
	if (eventData.mouseButtonIsReleased && (eventData.mouseButton == sf::Mouse::Button::Left) && (mySFML::smaller(mPosition, eventData.mousePosition) && mySFML::smaller(eventData.mousePosition, mPosition + static_cast<sf::Vector2i>(mSize))))
	{
		if (mActive)
		{
			mIsReleased = true;
		}
	}
}

void CheckBox::render(sf::RenderWindow *renderWindow)
{
	renderWindow->draw(*pSprite);
	renderWindow->draw(*pText);
}


//Getter
bool CheckBox::getChosen() const
{
	return mChosen;
}

bool CheckBox::getActive() const
{
	return mActive;
}

bool CheckBox::getAutonomic() const
{
	return mAutonomic;
}

bool CheckBox::getIsReleased() const
{
	return mIsReleased;
}

bool CheckBox::getIsReleasedAndReset()
{
	bool isReleased = mIsReleased;
	mIsReleased = false;
	return isReleased;
}


//Setter
void CheckBox::setChosen(bool chosen)
{
	mChosenChanged = !(mChosen == chosen);
	mChosen = chosen;
}

void CheckBox::setActive(bool active)
{
	mActiveChanged = !(mActive == active);
	mActive = active;
}

void CheckBox::setAutonomic(bool autonomic)
{
	mAutonomic = autonomic;
}


//Update Image with Respect to mActive and mChosen
void CheckBox::updateImage()
{
	if (!mChosen)
	{
		if (mActive)
		{
			pSprite->setTextureRect(sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(static_cast<sf::Vector2i>(mSize))));
		}
		else
		{
			pSprite->setTextureRect(sf::IntRect(sf::Vector2i(static_cast<int>(mSize.x), 0), sf::Vector2i(static_cast<sf::Vector2i>(mSize))));
		}
	}
	else
	{
		if (mActive)
		{
			pSprite->setTextureRect(sf::IntRect(sf::Vector2i(0, static_cast<int>(mSize.y)), sf::Vector2i(static_cast<sf::Vector2i>(mSize))));
		}
		else
		{
			pSprite->setTextureRect(sf::IntRect(sf::Vector2i(static_cast<int>(mSize.x), static_cast<int>(mSize.y)), sf::Vector2i(static_cast<sf::Vector2i>(mSize))));
		}
	}
}