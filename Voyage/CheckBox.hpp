#ifndef CHECKBOX_HPP
#define CHECKBOX_HPP

#include "Element.hpp"

#include "SFML\Graphics.hpp"

#include "mySFMLVectorFunctions.hpp"

class CheckBox : public Element
{
private:
	sf::Texture *pTexture;
	sf::Sprite *pSprite;

	sf::Font *pFont;
	sf::Text *pText;


	sf::Vector2i mPosition;
	sf::Vector2u mSize;
	
	bool mChosen;
	bool mChosenChanged = false;

	bool mActive;
	bool mActiveChanged = false;

	bool mAutonomic;
	bool mIsReleased = false;


public:
	CheckBox();
	CheckBox(sf::Texture *texture);
	CheckBox(sf::Texture *texture, sf::Vector2i position, sf::Vector2u size, bool active, bool chosen, bool autonomic, sf::Font *font, std::string text, sf::Vector2i textPosition, unsigned int textSize, sf::Color textColor);
	~CheckBox() override;

	void update(sf::RenderWindow *renderWindow, sf::Time frametime) override;
	void handleEvents(EventData eventData) override;
	void render(sf::RenderWindow *renderWindow) override;


	bool getChosen() const;
	bool getActive() const;
	bool getAutonomic() const;
	bool getIsReleased() const;
	bool getIsReleasedAndReset();

	void setChosen(bool chosen);
	void setActive(bool active);
	void setAutonomic(bool autonomic);

private:
	void updateImage();
};




#endif