#include <SFML/Graphics.hpp>

#include "gameStateMainMenu.hpp"
#include "gameStateLoading.hpp"
#include "gameStateGame.hpp"
#include "gameState.hpp"
#include "easylogging++.hpp"

GameStateMainMenu::GameStateMainMenu(Game& game_) : 
	GameState(game_)
{
	settings = game_.getSettings();
	sf::Vector2f size = sf::Vector2f(game.window.getSize());
	guiView.setSize(size);
	view.setSize(size);
	size *= 0.5f; // for positioning view centrally
	guiView.setCenter(size);
	view.setCenter(size);

	background.setTexture(ResourceManager::getInstance().getTexture("background"));
	background.setTextureRect(sf::IntRect(0, 0, settings->width, settings->height));

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
	return;
}

void GameStateMainMenu::handleInput()
{
	sf::Event event;

	while (game.window.pollEvent(event))
	{
		switch (event.type)
		{
			/* Close the window */
			case sf::Event::Closed:
			{
				game.window.close();
				break;
			}
			/* Resize the window */
			case sf::Event::KeyPressed:
			{
				break;
			}
			default:
			{
				gui.handleEvent(event);
				break;
			}
		}
	}

	// check all the window's events that were triggered since the last iteration of the loop
	//while (game.window.pollEvent(event))
	//{
	//	// "close requested" event: we close the window
	//	if (event.type == sf::Event::Closed) {
	//		game.window.close();
	//	}
	//	else if (event.type == sf::Event::KeyPressed) {
	//		settings->fullscreen = settings->fullscreen;
	//		game.window.create(sf::VideoMode(settings->width, settings->height, settings->COLOR_DEPTH), settings->APPNAME, (settings->fullscreen ? sf::Style::Fullscreen : sf::Style::Resize | sf::Style::Close));

	//		settings->saveSettings();
	//	}

	//	return;
	//}
}

void GameStateMainMenu::startGame()
{
	LOG(INFO) << "Switching to loading screen...";

	game.pushState(std::shared_ptr<GameState>(new GameStateLoading(game)));

	return;
}

void GameStateMainMenu::quit()
{
	LOG(INFO) << "Quitting game...";

	game.window.close();

	return;
}

void GameStateMainMenu::save()
{
	LOG(INFO) << "Saving settings...";

	return;
}

void GameStateMainMenu::loadGui()
{
	unsigned int buttonTextSize = 32u;
	unsigned int labelTextSize = 24u;
	unsigned int statusTextSize = 20u;

	gui.setWindow(game.window);

	// set global font that all widgets can use by default
	gui.setFont("res/fonts/DejaVuSans.ttf");

	tgui::VerticalLayout::Ptr layout = std::make_shared<tgui::VerticalLayout>();
	tgui::HorizontalLayout::Ptr lastLineLayout = std::make_shared<tgui::HorizontalLayout>();

	layoutWidth = 400;
	layoutHeight = 800;
	layoutCenterX = static_cast<unsigned int>(settings->width) * 0.5f;
	layoutCenterY = static_cast<unsigned int>(settings->height) * 0.5f;

	layout->setSize(layoutWidth, layoutHeight);
	layout->setPosition(layoutCenterX - layoutWidth * 0.5f, layoutCenterY - layoutHeight * 0.5f);

	gui.add(layout, "MainMenu_Layout");

	tgui::Button::Ptr startButton = std::make_shared<tgui::Button>();
	startButton->setText("Start Game");
	startButton->setOpacity(0.9f);
	startButton->setTextSize(buttonTextSize);
	startButton->connect("pressed", [&](){ startGame(); });

	tgui::Button::Ptr okButton = std::make_shared<tgui::Button>();
	okButton->setText("Ok");
	okButton->setOpacity(0.9f);
	okButton->setTextSize(buttonTextSize);
	okButton->connect("pressed", [&](){ save(); });

	tgui::Button::Ptr quitButton = std::make_shared<tgui::Button>();
	quitButton->setText("Quit");
	quitButton->setOpacity(0.9f);
	quitButton->setTextSize(buttonTextSize);
	quitButton->connect("pressed", [&](){ quit(); });
	

	tgui::Label::Ptr playerNameLabel = std::make_shared<tgui::Label>();
	playerNameLabel->setText("Enter player name:");
	playerNameLabel->setTextColor(sf::Color::Black);
	playerNameLabel->setTextSize(labelTextSize);

	tgui::EditBox::Ptr playerNameEditbox = std::make_shared<tgui::EditBox>();
	playerNameEditbox->setDefaultText("Playername");
	playerNameEditbox->setTextSize(labelTextSize);

	tgui::Label::Ptr mazeSizeLabel = std::make_shared<tgui::Label>();
	mazeSizeLabel->setText("Maze Size:");
	mazeSizeLabel->setTextColor(sf::Color::Black);
	mazeSizeLabel->setTextSize(labelTextSize);

	tgui::ComboBox::Ptr mazeSize = std::make_shared<tgui::ComboBox>();
	mazeSize->addItem("3");
	mazeSize->addItem("5");
	mazeSize->addItem("7");
	mazeSize->addItem("9");
	mazeSize->addItem("11");
	mazeSize->addItem("13");
	mazeSize->addItem("15");
	mazeSize->setTextSize(labelTextSize);

	tgui::CheckBox::Ptr fullscreen = std::make_shared<tgui::CheckBox>();
	fullscreen->setText("Fullscreen");
	fullscreen->setTextSize(labelTextSize);

	tgui::Label::Ptr resolutionLabel = std::make_shared<tgui::Label>();
	resolutionLabel->setText("Resolution:");
	resolutionLabel->setTextColor(sf::Color::Black);
	resolutionLabel->setTextSize(labelTextSize);

	tgui::ComboBox::Ptr resolutions = std::make_shared<tgui::ComboBox>();
	resolutions->addItem("1280 x 720");
	resolutions->addItem("1600 x 900");
	resolutions->addItem("1920 x 1080");
	resolutions->setTextSize(labelTextSize);

	tgui::Label::Ptr statusLabel = std::make_shared<tgui::Label>();
	statusLabel->setText("Restart game in order for settings to take effect!");
	statusLabel->setTextColor(sf::Color::White);
	statusLabel->setTextSize(statusTextSize);

	layout->add(startButton);
	layout->add(playerNameLabel);
	layout->add(playerNameEditbox);
	layout->add(mazeSizeLabel);
	layout->add(mazeSize);
	layout->add(fullscreen);
	layout->add(resolutionLabel);
	layout->add(resolutions);
	layout->add(lastLineLayout);

	layout->insertSpace(1, 0.25f);
	layout->insertSpace(3, 0.25f);
	layout->insertSpace(4, 0.25f);

	lastLineLayout->add(okButton);
	lastLineLayout->add(quitButton);
	lastLineLayout->insertSpace(1, 0.1f);

	/*layout->add(playerNameLabel);
	layout->add(playerNameEditbox);*/
	/*layout->add(mazeSize);
	layout->add(fullscreen);
	layout->add(resolutions);*/
}
