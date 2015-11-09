#include <SFML/Graphics.hpp>

#include "gameStateMainMenu.hpp"
#include "gameStateOptions.hpp"
#include "gameStateLoading.hpp"
#include "gameStateGame.hpp"
#include "gameState.hpp"
#include "easylogging++.hpp"
#include <thread>
#include <chrono>

GameStateMainMenu::GameStateMainMenu(Game& game_) : 
	GameState(game_)
{
	settings = game_.getSettings();
	//sf::Vector2f size = sf::Vector2f(game.window.getSize());
	size = sf::Vector2f(static_cast<float>(settings->scaleWidth), static_cast<float>(settings->scaleHeight));
	guiView.setSize(size);
	view.setSize(size);
	guiView.setCenter(size * 0.5f);
	view.setCenter(size * 0.5f);

	background.setTexture(ResourceManager::getInstance().getTexture("background"));
	
	game.changeMusic("mainMenu", 0.f, 0.f, 0.f);

	loadGui();
}

void GameStateMainMenu::draw(const float deltaTime)
{
	game.window.setView(view);

	game.window.clear(sf::Color::Black);
	game.window.draw(background);
	gui.draw();

	return;
}

void GameStateMainMenu::update(const float deltaTime)
{
	if (triggeredGameLoad && !game.isMusicReady())
	{
		LOG(INFO) << "Switching to loading screen...";
		triggeredGameLoad = false;
		game.pushState(std::shared_ptr<GameState>(new GameStateLoading(game)));
	}

	return;
}

void GameStateMainMenu::handleInput()
{
	sf::Event event;

	while (game.window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			game.window.close();
		}

		gui.handleEvent(event);
	}
}

void GameStateMainMenu::startGame()
{
	ResourceManager::getInstance().getSound("buttonClick").play();
	game.changeMusic("loading", 0.f, 0.f, 0.f);
	triggeredGameLoad = true;

	return;
}

void GameStateMainMenu::quit()
{
	LOG(INFO) << "Quitting game...";

	ResourceManager::getInstance().getSound("buttonClick").play();
	std::this_thread::sleep_for(std::chrono::milliseconds(300));
	game.window.close();

	return;
}

void GameStateMainMenu::openSettings()
{
	LOG(INFO) << "Opening settings...";

	ResourceManager::getInstance().getSound("buttonClick").play();
	game.pushState(std::shared_ptr<GameState>(new GameStateOptions(game)));

	return;
}

void GameStateMainMenu::loadGui()
{
	background.setScale(static_cast<float>(settings->width) / static_cast<float>(background.getTexture()->getSize().x), static_cast<float>(settings->height) / static_cast<float>(background.getTexture()->getSize().y));
	view.setSize(static_cast<float>(settings->width), static_cast<float>(settings->height));
	view.setCenter(static_cast<float>(settings->width) * 0.5f, static_cast<float>(settings->height) * 0.5f);

	gui.removeAllWidgets();
	gui.setWindow(game.window);

	// set global font that all widgets can use by default
	gui.setFont("res/fonts/DejaVuSans.ttf");

	tgui::VerticalLayout::Ptr layout = std::make_shared<tgui::VerticalLayout>();

	layoutWidth = static_cast<float>(settings->defWidgetWidth) * settings->widthScaleFactor;
	layoutHeight = static_cast<float>(settings->widgetHeight * 3.5) * settings->heightScaleFactor;
	layoutCenterX = static_cast<unsigned int>(settings->width) * 0.5f;
	layoutCenterY = static_cast<unsigned int>(settings->height) * 0.5f;

	layout->setSize(layoutWidth, layoutHeight);
	layout->setPosition(layoutCenterX - layoutWidth * 0.5f, layoutCenterY - layoutHeight * 0.5f);

	gui.add(layout, "MainMenu_Layout");

	tgui::Button::Ptr startButton = std::make_shared<tgui::Button>();
	startButton->setText("Start Game");
	startButton->setOpacity(0.9f);
	startButton->setTextSize(static_cast<unsigned int>(settings->buttonTextSize * settings->heightScaleFactor));
	startButton->connect("pressed", [&](){ startGame(); });

	tgui::Button::Ptr optionsButton = std::make_shared<tgui::Button>();
	optionsButton->setText("Settings");
	optionsButton->setOpacity(0.9f);
	optionsButton->setTextSize(static_cast<unsigned int>(settings->buttonTextSize * settings->heightScaleFactor));
	optionsButton->connect("pressed", [&](){ openSettings(); });

	tgui::Button::Ptr quitButton = std::make_shared<tgui::Button>();
	quitButton->setText("Quit");
	quitButton->setOpacity(0.9f);
	quitButton->setTextSize(static_cast<unsigned int>(settings->buttonTextSize * settings->heightScaleFactor));
	quitButton->connect("pressed", [&](){ quit(); });
	
	layout->add(startButton);
	layout->add(optionsButton);
	layout->add(quitButton);

	layout->insertSpace(1, 0.25f);
	layout->insertSpace(3, 0.25f);
}
