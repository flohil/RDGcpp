#include <SFML/Graphics.hpp>

#include "gameStateLoading.hpp"
#include "gameStateGame.hpp"
#include "gameState.hpp"
#include "easylogging++.hpp"

GameStateLoading::GameStateLoading(Game& game_) :
GameState(game_)
{
	std::cout << "inside GameStateLoading constructor" << std::endl;

	settings = game_.getSettings();
	sf::Vector2f size = sf::Vector2f(game.window.getSize());
	view.setSize(size);
	size *= 0.5f; // for positioning view centrally
	view.setCenter(size);

	background.setTexture(ResourceManager::getInstance().getTexture("background"));
	background.setTextureRect(sf::IntRect(0, 0, settings->width, settings->height));

	// create gui 
	loadGui();
}

void GameStateLoading::draw(const float deltaTime)
{
	game.window.setView(view);

	game.window.clear(sf::Color::Black);
	game.window.draw(background);
	gui.draw();

	return;
}

void GameStateLoading::update(const float deltaTime)
{
	triggeredLoadingCountdown--;

	if (triggeredLoadingCountdown == 0)
	{
		triggerLoading();
	}

	return;
}

void GameStateLoading::handleInput()
{
	return;
}

void GameStateLoading::triggerLoading()
{
	std::shared_ptr<GameState> gameGameState(new GameStateGame(game));

	game.popState();
	game.pushState(gameGameState);

	LOG(INFO) << "Finished loading Game";

	return;
}

void GameStateLoading::loadGui()
{
	gui.removeAllWidgets();
	gui.setWindow(game.window);

	// set global font that all widgets can use by default
	gui.setFont("res/fonts/DejaVuSans.ttf");

	tgui::VerticalLayout::Ptr layout = std::make_shared<tgui::VerticalLayout>();

	layoutWidth = 330;
	layoutHeight = 100;
	layoutCenterX = static_cast<unsigned int>(settings->width) * 0.5f;
	layoutCenterY = static_cast<unsigned int>(settings->height) * 0.5f;

	layout->setSize(layoutWidth, layoutHeight);
	layout->setPosition(10, settings->height - layoutHeight);

	gui.add(layout, "LoadingScreen_Layout");

	tgui::Label::Ptr loadingLabel = std::make_shared<tgui::Label>();
	loadingLabel->setText("loading...");
	loadingLabel->setTextColor(sf::Color::White);
	loadingLabel->setOpacity(0.5f);
	loadingLabel->setTextSize(72);

	layout->add(loadingLabel, "LoadingLabel_Label");
}