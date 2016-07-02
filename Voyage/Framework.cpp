#include "stdafx.h"
#include "Framework.hpp"

//Constructor
Framework::Framework()
{
	srand(time(0));

	pFontArial = new sf::Font;
	pFontArial->loadFromFile("./Fonts/arial.ttf");

	pPlayerTexture = new sf::Texture;
	sf::Image playerSubImage;
	playerSubImage.loadFromFile("./Images/PlayerTextureRedWithEngines.png");
	playerSubImage.createMaskFromColor(sf::Color::Red);
	pPlayerTexture->loadFromImage(playerSubImage);
	//pPlayerTexture->setSmooth(true);

	pRenderWindow = new sf::RenderWindow(sf::VideoMode(1600, 950), "Voyage");
	pGlobalView = new sf::View(sf::Vector2f(0.f, 0.f), static_cast<sf::Vector2f>(pRenderWindow->getSize()) * 1000000.f);
	pPlayerView = new sf::View(sf::Vector2f(0.f, 0.f), static_cast<sf::Vector2f>(pRenderWindow->getSize()) * 1000000.f);
	mShowPlayerView = false;
	pRenderWindow->setView(*pGlobalView);

	pClock = new sf::Clock;
	pClock->restart();

	pUniverse = new Universe(pGlobalView, pPlayerView, &mShowPlayerView, pFontArial, pPlayerTexture);
}

//Destructor
Framework::~Framework()
{
	delete pGlobalView;
	delete pPlayerView;
	delete pRenderWindow;
	delete pClock;
	delete pUniverse;

	pGlobalView = nullptr;
	pPlayerView = nullptr;
	pRenderWindow = nullptr;
	pClock = nullptr;
	pUniverse = nullptr;
}


//HandleEvents
void Framework::handleEvents()
{
	sf::Event mEvent;
	while (pRenderWindow->pollEvent(mEvent))
	{
		if (mEvent.type == sf::Event::EventType::Closed)
		{
			pRenderWindow->close();
		}
		if (mEvent.type == sf::Event::EventType::KeyReleased)
		{
			if (mEvent.key.code == sf::Keyboard::Key::Escape)
			{
				pRenderWindow->close();
			}
		}
	}
}

//Update
void Framework::update()
{
	pUniverse->update(pRenderWindow, pClock->restart());
}

//Render
void Framework::render()
{
	pRenderWindow->clear();
	pRenderWindow->setView(*pGlobalView);
	pUniverse->render(pRenderWindow);
	//std::cout << pRenderWindow->getView().getViewport().left << " " << pRenderWindow->getView().getViewport().top << " " << pRenderWindow->getView().getViewport().width << " " << pRenderWindow->getView().getViewport().height << std::endl;
	if (mShowPlayerView)
	{
		std::cout << "Show!" << std::endl;
		pRenderWindow->setView(*pPlayerView);
		pUniverse->render(pRenderWindow);
		pRenderWindow->setView(*pGlobalView);
	}
	pRenderWindow->display();
}


//Run
void Framework::run()
{
	unsigned int counter = 0;
	while (pRenderWindow->isOpen())
	{
		counter += 1;
		//std::cout << "Main Loop: " << counter << std::endl;
		handleEvents();
		update();
		render();
	}
}