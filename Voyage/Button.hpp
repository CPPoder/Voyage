#ifndef BUTTON_HPP
#define BUTTON_HPP

#include "Element.hpp"

#include "SFML\Graphics.hpp"

#include "mySFMLVectorFunctions.hpp"

class Button : public Element
{
private:
	sf::Texture *pTexture;
	sf::Sprite *pSprite;

	sf::Font *pFont;
	sf::Text *pText;


	sf::Vector2i mPosition;
	sf::Vector2u mSize;
	
	bool mButtonPressed;
	bool mButtonReleased = false;



public:
	Button();
	Button(sf::Texture *texture);
	Button(sf::Texture *texture, sf::Vector2i position, sf::Vector2u size, sf::Font *font, std::string text, unsigned int textSize);
	~Button() override;

	void update(sf::RenderWindow *renderWindow, sf::Time frametime) override;
	void handleEvents(EventData eventData) override;
	void render(sf::RenderWindow *renderWindow) override;


	bool getIsReleased() const;
};




#endif