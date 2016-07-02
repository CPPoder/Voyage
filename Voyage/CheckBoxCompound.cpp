#include "stdafx.h"
#include "CheckBoxCompound.hpp"



//Constructor
CheckBoxCompound::CheckBoxCompound(unsigned int numberOfCheckBoxes, bool coupled, std::vector<bool> vectorOfBoxesActivity, std::vector<bool> vectorOfBoxesChosen, sf::Texture *checkBoxTexture, sf::Vector2u checkBoxSize, sf::Vector2i positionOfFirstCheckBox, sf::Vector2i relPosBetweenCheckBoxes, sf::Vector2i relPosOfTextFromCheckBox, sf::Font *font, unsigned int textSize, sf::Color textColor, std::vector<std::string> vectorOfTexts)
	: mNumberOfCheckBoxes(numberOfCheckBoxes),
	  mPositionOfFirstCheckBox(positionOfFirstCheckBox),
	  mRelPosBetweenCheckBoxes(relPosBetweenCheckBoxes),
	  mRelPosOfTextFromCheckBox(relPosOfTextFromCheckBox),
	  pFont(font),
	  mTextSize(textSize),
	  mTextColor(textColor),
	  mCoupled(coupled)
{
	pCheckBoxTexture = checkBoxTexture;
	
	//Construct and Pushback the CheckBoxes
	for (unsigned int boxNumber = 0; boxNumber < mNumberOfCheckBoxes; boxNumber++)
	{
		mVectorOfCheckBoxes.push_back(new CheckBox(pCheckBoxTexture, mPositionOfFirstCheckBox + (static_cast<int>(boxNumber) * mRelPosBetweenCheckBoxes), checkBoxSize, vectorOfBoxesActivity.at(boxNumber), vectorOfBoxesChosen.at(boxNumber), (!mCoupled), pFont, vectorOfTexts.at(boxNumber), mPositionOfFirstCheckBox + (static_cast<int>(boxNumber) * mRelPosBetweenCheckBoxes) + mRelPosOfTextFromCheckBox, mTextSize, mTextColor));
	}
	
	//Determine the first chosen Box
	unsigned int firstChosenBox = 0;
	for (unsigned int boxNumber = 0; boxNumber < mNumberOfCheckBoxes; boxNumber++)
	{
		if (vectorOfBoxesChosen.at(boxNumber))
		{
			firstChosenBox = boxNumber;
			break;
		}
	}
	mChosenBox = firstChosenBox;
}


//Destructor
CheckBoxCompound::~CheckBoxCompound()
{
	pCheckBoxTexture = nullptr;

	for (auto box : mVectorOfCheckBoxes)
	{
		delete box;
		box = nullptr;
	}
	mVectorOfCheckBoxes.clear();
}



//HandleEvents
void CheckBoxCompound::handleEvents(EventData eventData)
{
	for (auto box : mVectorOfCheckBoxes)
	{
		box->handleEvents(eventData);
	}
}


//Update
void CheckBoxCompound::update(sf::RenderWindow *renderWindow, sf::Time frametime)
{
	//If coupled: Determine mChosenBox and set the Box as chosen
	if (mCoupled)
	{
		for (unsigned int boxNumber = 0; boxNumber < mNumberOfCheckBoxes; boxNumber++)
		{
			if (mVectorOfCheckBoxes.at(boxNumber)->getIsReleasedAndReset())
			{
				mChosenBox = boxNumber;
				break;
			}
		}
		for (unsigned int boxNumber = 0; boxNumber < mNumberOfCheckBoxes; boxNumber++)
		{
			if (boxNumber == mChosenBox)
			{
				mVectorOfCheckBoxes.at(boxNumber)->setChosen(true);
			}
			else
			{
				mVectorOfCheckBoxes.at(boxNumber)->setChosen(false);
			}
		}
	}

	//Update all checkBoxes
	for (auto box : mVectorOfCheckBoxes)
	{
		box->update(renderWindow, frametime);
	}
}


//Render
void CheckBoxCompound::render(sf::RenderWindow *renderWindow)
{
	for (auto box : mVectorOfCheckBoxes)
	{
		box->render(renderWindow);
	}
}


//Getter
bool CheckBoxCompound::getCoupled() const
{
	return mCoupled;
}

unsigned int CheckBoxCompound::getChosenBox() const
{
	return mChosenBox;
}

bool CheckBoxCompound::getAllBoxesActive() const
{
	bool allActive = true;
	for (auto box : mVectorOfCheckBoxes)
	{
		allActive = allActive && box->getActive();
	}
	return allActive;
}

std::vector<bool> CheckBoxCompound::getVectorOfBoxesActivity() const
{
	std::vector<bool> vectorOfBoxesActivity;
	for (auto box : mVectorOfCheckBoxes)
	{
		vectorOfBoxesActivity.push_back(box->getActive());
	}
	return vectorOfBoxesActivity;
}



//Setter
void CheckBoxCompound::setAllBoxesActive(bool allBoxesActive)
{
	for (auto box : mVectorOfCheckBoxes)
	{
		box->setActive(allBoxesActive);
	}
}

void CheckBoxCompound::setBoxesActivityFromVector(std::vector<bool> activity)
{
	if (activity.size() == mVectorOfCheckBoxes.size())
	{
		for (unsigned int counter = 0; counter < mNumberOfCheckBoxes; counter++)
		{
			mVectorOfCheckBoxes.at(counter)->setActive(activity.at(counter));
		}
	}
	else
	{
		std::cout << "FirstScreen::setBoxesActivityFromVector(std::vector<bool>): The size of input-vector is not compatible with the size of mVectorOfCheckBoxes!" << std::endl;
	}
}