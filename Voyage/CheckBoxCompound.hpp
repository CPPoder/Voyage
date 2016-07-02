#ifndef CHECKBOXCOMPOUND_HPP
#define CHECKBOXCOMPOUND_HPP

#include <iostream>
#include <list>
#include <vector>
#include <string>

#include "Element.hpp"
#include "CheckBox.hpp"

#include "SFML\Graphics.hpp"


class CheckBoxCompound : public Element
{
private:
	unsigned int mNumberOfCheckBoxes;
	

	sf::Vector2i mPositionOfFirstCheckBox;
	sf::Vector2i mRelPosBetweenCheckBoxes;
	sf::Vector2i mRelPosOfTextFromCheckBox;

	sf::Texture *pCheckBoxTexture;
	std::vector<CheckBox*> mVectorOfCheckBoxes;

	sf::Font *pFont;
	unsigned int mTextSize;
	sf::Color mTextColor;

	bool mCoupled;
	unsigned int mChosenBox;

public:
	CheckBoxCompound::CheckBoxCompound(unsigned int numberOfCheckBoxes, bool coupled, std::vector<bool> vectorOfBoxesActivity, std::vector<bool> vectorOfBoxesChosen, sf::Texture *checkBoxTexture, sf::Vector2u checkBoxSize, sf::Vector2i positionOfFirstCheckBox, sf::Vector2i relPosBetweenCheckBoxes, sf::Vector2i relPosOfTextFromCheckBox, sf::Font *font, unsigned int textSize, sf::Color textColor, std::vector<std::string> vectorOfTexts);
	~CheckBoxCompound();

	void handleEvents(EventData eventData) override;
	void update(sf::RenderWindow *renderWindow, sf::Time frametime) override;
	void render(sf::RenderWindow *renderWindow) override;

	bool getCoupled() const;
	unsigned int getChosenBox() const;
	bool getAllBoxesActive() const;
	std::vector<bool> getVectorOfBoxesActivity() const;

	void setAllBoxesActive(bool allBoxesActive);
	void setBoxesActivityFromVector(std::vector<bool> activity);


};





#endif