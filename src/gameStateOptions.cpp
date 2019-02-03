#include <SFML/Graphics.hpp>

#include "gameStateOptions.hpp"
#include "gameState.hpp"
#include "easylogging++.hpp"
#include <cstdlib>
#include <chrono>
#include <thread>

GameStateOptions::GameStateOptions(Game& game_) :
GameState(game_)
{
	settings = game_.getSettings();
	size = sf::Vector2f(static_cast<float>(settings->scaleWidth), static_cast<float>(settings->scaleHeight));
	guiView.setSize(size);
	view.setSize(size);
	guiView.setCenter(size * 0.5f);
	view.setCenter(size * 0.5f);

	status = "";

	background.setTexture(ResourceManager::getInstance().getTexture("background"));

	loadGui();
}

void GameStateOptions::draw(const float deltaTime)
{
	game.window.setView(view);

	game.window.clear(sf::Color::Black);
	game.window.draw(background);
	gui.draw();

	return;
}

void GameStateOptions::update(const float deltaTime)
{
	return;
}

void GameStateOptions::handleInput()
{
	sf::Event event;

	while (game.window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			game.window.close();
		}
		else if (event.type == sf::Event::KeyPressed)
		{
			if (event.key.code == sf::Keyboard::Escape)
			{
				returnToMainMenu();
			}
		}

		gui.handleEvent(event);
	}
}

void GameStateOptions::returnToMainMenu()
{
	LOG(INFO) << "Returning to main menu...";

	ResourceManager::getInstance().setSoundVolumes(settings->actualEffectsVolume);
	game.music.setVolume(settings->actualMusicVolume);
	ResourceManager::getInstance().getSound("buttonClick").play();
	game.popState();

	return;
}

void GameStateOptions::saveSettings()
{
	LOG(INFO) << "Saving settings...";

	ResourceManager::getInstance().getSound("buttonClick").play();

	bool oldFullScreen = settings->fullscreen;
	unsigned int oldWidth = settings->width;
	unsigned int oldHeight = settings->height;
	float oldActualEffectsVolume = settings->actualEffectsVolume;
	float oldActualMusicVolume = settings->actualMusicVolume;
	
	std::string playerName = playerNameEditbox->getText().toAnsiString();
	std::string selectedItemId = mazeSizeCombobox->getSelectedItemId();
	std::string resolutionStr = resolutionsCombobox->getSelectedItem();
	sf::Vector2u resolution = GameStateOptions::guiStrCrossPairToUi(resolutionStr);
	if (playerName.length() > 0)
	{
		settings->playerName = playerName;
	}
	settings->fullscreen = fullscreenCheckbox->isChecked();
	settings->mazeSize = std::stoul(selectedItemId, nullptr, 0);
	settings->width = resolution.x;
	settings->height = resolution.y;
	settings->enableSound = enableSoundCheckbox->isChecked();
	settings->effectsVolume = static_cast<float>(effectsVolumeSlider->getValue());
	settings->musicVolume = static_cast<float>(musicVolumeSlider->getValue());

	if (settings->enableSound)
	{
		settings->actualEffectsVolume = settings->effectsVolume;
		settings->actualMusicVolume = settings->musicVolume;
	}
	else
	{
		settings->actualEffectsVolume = 0.f;
		settings->actualMusicVolume = 0.f;
	}

	settings->saveSettings();

	status = "Settings have been saved!";

	statusLabel->setText(status);

	if (oldActualEffectsVolume != settings->actualEffectsVolume)
	{
		std::cout << "actualEffectsVolume is now " << settings->actualEffectsVolume << std::endl;
		ResourceManager::getInstance().setSoundVolumes(settings->actualEffectsVolume);
	}

	if (oldActualMusicVolume != settings->actualMusicVolume)
	{
		// change music volume
	}

	if (oldFullScreen != settings->fullscreen || oldWidth != settings->width || oldHeight != settings->height)
	{
		game.window.create(sf::VideoMode(settings->width, settings->height, settings->COLOR_DEPTH), settings->APPNAME, (settings->fullscreen ? sf::Style::Fullscreen : sf::Style::Resize | sf::Style::Close));

		if (oldWidth != settings->width || oldHeight != settings->height)
		{
			std::cout << "scalewidht: " << settings->scaleWidth << std::endl;
			std::cout << "scaleheight: " << settings->scaleHeight << std::endl;

			std::cout << "width: " << settings->width << std::endl;
			std::cout << "height: " << settings->height << std::endl;

			settings->widthScaleFactor = static_cast<float>(settings->width) / static_cast<float>(settings->scaleWidth);
			settings->heightScaleFactor = static_cast<float>(settings->height) / static_cast<float>(settings->scaleHeight);
			settings->widthDownScaleFactor = 1.f / settings->widthScaleFactor;
			settings->heightDownScaleFactor = 1.f / settings->heightScaleFactor;

			std::cout << "widthScaleFactor: " << settings->widthScaleFactor << std::endl;
			std::cout << "heightScaleFactor: " << settings->heightScaleFactor << std::endl;
			std::cout << "widthDownScaleFactor: " << settings->widthDownScaleFactor << std::endl;
			std::cout << "heightDownScaleFactor: " << settings->heightDownScaleFactor << std::endl;

			game.reloadGuis();
		}
	}

	return;
}

void GameStateOptions::loadGui()
{
	background.setScale(static_cast<float>(settings->width) / static_cast<float>(background.getTexture()->getSize().x), static_cast<float>(settings->height) / static_cast<float>(background.getTexture()->getSize().y));
	view.setSize(static_cast<float>(settings->width), static_cast<float>(settings->height));
	view.setCenter(static_cast<float>(settings->width) * 0.5f, static_cast<float>(settings->height) * 0.5f);

	gui.removeAllWidgets();
	gui.setWindow(game.window);

	//obtain possible resolutions
	std::vector<sf::VideoMode> availiableVideoModes;
	availiableVideoModes = sf::VideoMode::getFullscreenModes();
	sf::VideoMode desktopVideoMode = sf::VideoMode::getDesktopMode();

	float aspectRatio = static_cast<float>(desktopVideoMode.width) / static_cast<float>(desktopVideoMode.height);

	std::vector<sf::VideoMode>::iterator it = availiableVideoModes.begin();

	while (it != availiableVideoModes.end()) 
	{
		if ((static_cast<float>(it->width) / static_cast<float>(it->height)) != aspectRatio || it->bitsPerPixel != 32u)
		{
			it = availiableVideoModes.erase(it);
		}
		else
		{
			++it;
		}
	}

	// set global font that all widgets can use by default
	gui.setFont("res/fonts/DejaVuSans.ttf");

	layoutWidth = static_cast<float>(settings->defWidgetWidth) * settings->widthScaleFactor;
	layoutCenterX = static_cast<unsigned int>(settings->width) * 0.5f;
	layoutCenterY = static_cast<unsigned int>(settings->height) * 0.5f;

	float layoutLeft = layoutCenterX - layoutWidth * 0.5f;
	float horSpace = 20.f * settings->widthScaleFactor;
	float verSpace = 20.f * settings->heightScaleFactor;

	tgui::Label::Ptr playerNameLabel = std::make_shared<tgui::Label>();
	playerNameLabel->setText("Player Name");
	playerNameLabel->setTextColor(sf::Color::Black);
	playerNameLabel->setTextSize(static_cast<unsigned int>(settings->labelSmallTextSize * settings->heightScaleFactor));
	playerNameLabel->setSize(settings->widthScaleFactor * (settings->defWidgetWidth - settings->labelPaddingX), settings->heightScaleFactor * settings->labelSmallHeight);

	playerNameEditbox = std::make_shared<tgui::EditBox>();
	playerNameEditbox->setOpacity(0.9f);
	playerNameEditbox->setDefaultText(settings->playerName);
	playerNameEditbox->setTextSize(static_cast<unsigned int>(settings->heightScaleFactor * settings->labelBigTextSize));
	playerNameEditbox->setSize(settings->widthScaleFactor * settings->defWidgetWidth, settings->heightScaleFactor * settings->labelBigHeight);
	playerNameEditbox->setMaximumCharacters(20);
	playerNameEditbox->connect("clicked", [&](){ ResourceManager::getInstance().getSound("guiControlResponse").play(); });
	playerNameEditbox->connect("textchanged", [&](){ ResourceManager::getInstance().getSound("keyStroke").play(); });
	playerNameEditbox->connect("returnkeypressed", [&](){ ResourceManager::getInstance().getSound("keyStrokeReturn").play(); });

	tgui::Label::Ptr mazeSizeLabel = std::make_shared<tgui::Label>();
	mazeSizeLabel->setText("Maze Size");
	mazeSizeLabel->setTextColor(sf::Color::Black);
	mazeSizeLabel->setTextSize(static_cast<unsigned int>(settings->heightScaleFactor * settings->labelSmallTextSize));
	mazeSizeLabel->setSize(settings->widthScaleFactor * (settings->defWidgetWidth * 0.5f - settings->labelPaddingX) - horSpace * 0.5f, settings->heightScaleFactor * settings->labelSmallHeight);

	mazeSizeCombobox = std::make_shared<tgui::ComboBox>();
	mazeSizeCombobox->setOpacity(0.9f);
	mazeSizeCombobox->addItem(" 3 x 3", "3");
	mazeSizeCombobox->addItem(" 5 x 5", "5");
	mazeSizeCombobox->addItem(" 7 x 7", "7");
	mazeSizeCombobox->addItem(" 9 x 9", "9");
	mazeSizeCombobox->addItem(" 11 x 11", "11");
	if (settings->mazeSize >= 3 && settings->mazeSize <= 11 && settings->mazeSize % 2 == 1)
	{
		mazeSizeCombobox->setSelectedItemById(std::to_string(settings->mazeSize));
	}
	else
	{
		mazeSizeCombobox->setSelectedItemById(std::to_string(5u));
	}
	mazeSizeCombobox->setTextSize(static_cast<unsigned int>(settings->heightScaleFactor * settings->labelBigTextSize));
	mazeSizeCombobox->setSize(settings->widthScaleFactor * (settings->defWidgetWidth * 0.5f) - horSpace * 0.5f, settings->heightScaleFactor * settings->labelBigHeight);
	mazeSizeCombobox->connect("focused", [&](){ ResourceManager::getInstance().getSound("guiControlResponse").play(); });
	mazeSizeCombobox->connect("itemselected", [&](){ ResourceManager::getInstance().getSound("guiControlResponse").play(); });

	tgui::Label::Ptr fullscreenLabel = std::make_shared<tgui::Label>();
	fullscreenLabel->setText("Fullscreen");
	fullscreenLabel->setTextColor(sf::Color::Black);
	fullscreenLabel->setTextSize(static_cast<unsigned int>(settings->heightScaleFactor * settings->labelSmallTextSize));
	fullscreenLabel->setSize(settings->widthScaleFactor * (settings->defWidgetWidth * 0.5f - settings->labelPaddingX) - horSpace * 0.5f, settings->heightScaleFactor * settings->labelSmallHeight);

	fullscreenCheckbox = std::make_shared<tgui::CheckBox>();
	fullscreenCheckbox->setOpacity(0.9f);
	fullscreenCheckbox->setSize(settings->heightScaleFactor * settings->labelBigHeight, settings->heightScaleFactor * settings->labelBigHeight);
	if (settings->fullscreen)
	{
		fullscreenCheckbox->check();
	}
	fullscreenCheckbox->connect("checked", [&](){ ResourceManager::getInstance().getSound("guiControlResponse").play(); });
	fullscreenCheckbox->connect("unchecked", [&](){ ResourceManager::getInstance().getSound("guiControlResponse").play(); });

	tgui::Label::Ptr resolutionLabel = std::make_shared<tgui::Label>();
	resolutionLabel->setText("Resolution");
	resolutionLabel->setTextColor(sf::Color::Black);
	resolutionLabel->setTextSize(static_cast<unsigned int>(settings->heightScaleFactor * settings->labelSmallTextSize));
	resolutionLabel->setSize(settings->widthScaleFactor * (settings->defWidgetWidth * 0.5f - settings->labelPaddingX) - horSpace * 0.5f, settings->heightScaleFactor * settings->labelSmallHeight);

	resolutionsCombobox = std::make_shared<tgui::ComboBox>();
	resolutionsCombobox->setOpacity(0.9f);
	for (sf::VideoMode vMode : availiableVideoModes)
	{
		resolutionsCombobox->addItem(GameStateOptions::uiToGuiStrCrossPair(vMode.width, vMode.height));
	}
	resolutionsCombobox->setSelectedItem(GameStateOptions::uiToGuiStrCrossPair(settings->width, settings->height));
	resolutionsCombobox->setTextSize(static_cast<unsigned int>(settings->heightScaleFactor * settings->labelBigTextSize));
	resolutionsCombobox->setSize(settings->widthScaleFactor * (settings->defWidgetWidth * 0.5f) - horSpace * 0.5f, settings->heightScaleFactor * settings->labelBigHeight);
	resolutionsCombobox->connect("focused", [&](){ ResourceManager::getInstance().getSound("guiControlResponse").play(); });
	resolutionsCombobox->connect("itemselected", [&](){ ResourceManager::getInstance().getSound("guiControlResponse").play(); });

	tgui::Label::Ptr enableSoundLabel = std::make_shared<tgui::Label>();
	enableSoundLabel->setText("Enable Sound");
	enableSoundLabel->setTextColor(sf::Color::Black);
	enableSoundLabel->setTextSize(static_cast<unsigned int>(settings->heightScaleFactor * settings->labelSmallTextSize));
	enableSoundLabel->setSize(settings->widthScaleFactor * (settings->defWidgetWidth * 0.5f - settings->labelPaddingX) - horSpace * 0.5f, settings->heightScaleFactor * settings->labelSmallHeight);

	enableSoundCheckbox = std::make_shared<tgui::CheckBox>();
	enableSoundCheckbox->setOpacity(0.9f);
	enableSoundCheckbox->setSize(settings->heightScaleFactor * settings->labelBigHeight, settings->heightScaleFactor * settings->labelBigHeight);
	if (settings->enableSound)
	{
		enableSoundCheckbox->check();
	}
	enableSoundCheckbox->connect("checked", [&](){ 
		ResourceManager::getInstance().setSoundVolumes(static_cast<float>(effectsVolumeSlider->getValue()));
		game.music.setVolume(static_cast<float>(musicVolumeSlider->getValue()));
		sf::Time musicOffset = sf::seconds(0);
		ResourceManager::getInstance().getMusic(game.currentMusic)->timeOffset = musicOffset;
		game.music.setPlayingOffset(musicOffset);
		game.music.play();
		ResourceManager::getInstance().getSound("guiControlResponse").play();
	});
	enableSoundCheckbox->connect("unchecked", [&](){ 
		ResourceManager::getInstance().getSound("guiControlResponse").play();
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
		ResourceManager::getInstance().setSoundVolumes(0.f);
		game.music.setVolume(0.f);
		game.music.stop();
	});

	tgui::Label::Ptr effectsVolumeLabel = std::make_shared<tgui::Label>();
	effectsVolumeLabel->setText("Effects Volume");
	effectsVolumeLabel->setTextColor(sf::Color::Black);
	effectsVolumeLabel->setTextSize(static_cast<unsigned int>(settings->heightScaleFactor * settings->labelSmallTextSize));
	effectsVolumeLabel->setSize(settings->widthScaleFactor * (settings->defWidgetWidth * 0.5f - settings->labelPaddingX) - horSpace * 0.5f, settings->heightScaleFactor * settings->labelSmallHeight);

	effectsVolumeSlider = std::make_shared<tgui::Slider>();
	effectsVolumeSlider->setSize(settings->widthScaleFactor * (settings->defWidgetWidth * 0.5f) - horSpace * 0.5f, settings->heightScaleFactor * settings->sliderHeight);
	effectsVolumeSlider->setMaximum(100);
	effectsVolumeSlider->setValue(static_cast<int>(settings->effectsVolume));
	effectsVolumeSlider->connect("valuechanged", [&](){ 
		sf::Sound& sound = ResourceManager::getInstance().getSound("guiControlResponse");
		if (enableSoundCheckbox->isChecked())
		{
			ResourceManager::getInstance().setSoundVolumes(static_cast<float>(effectsVolumeSlider->getValue()));
			sound.play();
		}
	});

	tgui::Label::Ptr musicVolumeLabel = std::make_shared<tgui::Label>();
	musicVolumeLabel->setText("Music Volume");
	musicVolumeLabel->setTextColor(sf::Color::Black);
	musicVolumeLabel->setTextSize(static_cast<unsigned int>(settings->heightScaleFactor * settings->labelSmallTextSize));
	musicVolumeLabel->setSize(settings->widthScaleFactor * (settings->defWidgetWidth * 0.5f - settings->labelPaddingX) - horSpace * 0.5f, settings->heightScaleFactor * settings->labelSmallHeight);

	musicVolumeSlider = std::make_shared<tgui::Slider>();
	musicVolumeSlider->setSize(settings->widthScaleFactor * (settings->defWidgetWidth * 0.5f) - horSpace * 0.5f, settings->heightScaleFactor * settings->sliderHeight);
	musicVolumeSlider->setMaximum(100);
	musicVolumeSlider->setValue(static_cast<int>(settings->musicVolume));
	musicVolumeSlider->connect("valuechanged", [&](){
		if (enableSoundCheckbox->isChecked())
		{
			game.music.setVolume(static_cast<float>(musicVolumeSlider->getValue()));
		}
	});

	statusLabel = std::make_shared<tgui::Label>();
	statusLabel->setText(status);
	statusLabel->setTextColor(sf::Color::Black);
	statusLabel->setTextSize(static_cast<unsigned int>(settings->heightScaleFactor * settings->labelSmallTextSize));
	statusLabel->setSize(settings->widthScaleFactor * settings->defWidgetWidth, settings->heightScaleFactor * settings->labelSmallHeight);

	tgui::Button::Ptr saveButton = std::make_shared<tgui::Button>();
	saveButton->setText("Save");
	saveButton->setOpacity(0.9f);
	saveButton->setTextSize(static_cast<unsigned int>(settings->heightScaleFactor * settings->buttonTextSize));
	saveButton->connect("pressed", [&](){ saveSettings(); });
	saveButton->setSize(settings->widthScaleFactor * (settings->defWidgetWidth * 0.5f) - horSpace * 0.5f, static_cast<float>(settings->heightScaleFactor * settings->widgetHeight));

	tgui::Button::Ptr backButton = std::make_shared<tgui::Button>();
	backButton->setText("Back");
	backButton->setOpacity(0.9f);
	backButton->setTextSize(static_cast<unsigned int>(settings->heightScaleFactor * settings->buttonTextSize));
	backButton->connect("pressed", [&](){ returnToMainMenu(); });
	backButton->setSize(settings->widthScaleFactor * (settings->defWidgetWidth * 0.5f) - horSpace * 0.5f, static_cast<float>(settings->heightScaleFactor * settings->widgetHeight));

	// obtain size of all elements
	layoutHeight = playerNameLabel->getSize().y + playerNameEditbox->getSize().y + mazeSizeCombobox->getSize().y + fullscreenCheckbox->getSize().y + resolutionsCombobox->getSize().y + statusLabel->getSize().y + enableSoundCheckbox->getSize().y + effectsVolumeSlider->getSize().y + musicVolumeSlider->getSize().y + saveButton->getSize().y + 2 * (settings->labelBigHeight - settings->sliderHeight) + 8 * verSpace;
	float layoutTop = layoutCenterY - layoutHeight * 0.5f;
	float heightSum = layoutTop;

	//set positions
	playerNameLabel->setPosition(layoutLeft + settings->widthScaleFactor * (settings->labelPaddingX), layoutTop);
	heightSum += playerNameLabel->getSize().y;
	playerNameEditbox->setPosition(layoutLeft, heightSum);
	heightSum += playerNameEditbox->getSize().y;
	heightSum += verSpace; //space
	mazeSizeLabel->setPosition(layoutLeft + settings->widthScaleFactor * (settings->labelPaddingX), heightSum + (settings->labelBigHeight - settings->labelSmallHeight) * 0.5f);
	mazeSizeCombobox->setPosition(layoutLeft + settings->widthScaleFactor * (settings->defWidgetWidth * 0.5f) + horSpace * 0.5f, heightSum);
	heightSum += mazeSizeCombobox->getSize().y;
	heightSum += verSpace; //space
	fullscreenLabel->setPosition(layoutLeft + settings->widthScaleFactor * (settings->labelPaddingX), heightSum + (settings->labelBigHeight - settings->labelSmallHeight) * 0.5f);
	fullscreenCheckbox->setPosition(layoutLeft + settings->widthScaleFactor * (settings->defWidgetWidth * 0.5f - 2.f) + horSpace * 0.5f, heightSum);
	heightSum += fullscreenCheckbox->getSize().y;
	heightSum += verSpace; //space
	resolutionLabel->setPosition(layoutLeft + settings->widthScaleFactor * (settings->labelPaddingX), heightSum + (settings->labelBigHeight - settings->labelSmallHeight) * 0.5f);
	resolutionsCombobox->setPosition(layoutLeft + settings->widthScaleFactor * (settings->defWidgetWidth * 0.5f) + horSpace * 0.5f, heightSum);
	heightSum += resolutionsCombobox->getSize().y;
	heightSum += verSpace; //space
	enableSoundLabel->setPosition(layoutLeft + settings->widthScaleFactor * (settings->labelPaddingX), heightSum + (settings->labelBigHeight - settings->labelSmallHeight) * 0.5f);
	enableSoundCheckbox->setPosition(layoutLeft + settings->widthScaleFactor * (settings->defWidgetWidth * 0.5f - 2.f) + horSpace * 0.5f, heightSum);
	heightSum += enableSoundCheckbox->getSize().y;
	heightSum += verSpace; //space
	effectsVolumeLabel->setPosition(layoutLeft + settings->widthScaleFactor * (settings->labelPaddingX), heightSum + (settings->labelBigHeight - settings->labelSmallHeight) * 0.5f);
	effectsVolumeSlider->setPosition(layoutLeft + settings->widthScaleFactor * (settings->defWidgetWidth * 0.5f) + horSpace * 0.5f, heightSum + (settings->labelBigHeight - settings->sliderHeight) * 0.5f);
	heightSum += effectsVolumeSlider->getSize().y + (settings->labelBigHeight - settings->sliderHeight);
	heightSum += verSpace; //space
	musicVolumeLabel->setPosition(layoutLeft + settings->widthScaleFactor * (settings->labelPaddingX), heightSum + (settings->labelBigHeight - settings->labelSmallHeight) * 0.5f);
	musicVolumeSlider->setPosition(layoutLeft + settings->widthScaleFactor * (settings->defWidgetWidth * 0.5f) + horSpace * 0.5f, heightSum + (settings->labelBigHeight - settings->sliderHeight) * 0.5f);
	heightSum += musicVolumeSlider->getSize().y + (settings->labelBigHeight - settings->sliderHeight);
	heightSum += verSpace; //space
	statusLabel->setPosition(layoutLeft + settings->widthScaleFactor * (75), heightSum);
	heightSum += statusLabel->getSize().y;
	heightSum += verSpace; //space
	saveButton->setPosition(layoutLeft, heightSum);
	backButton->setPosition(layoutLeft + settings->widthScaleFactor * (settings->defWidgetWidth * 0.5f) + horSpace * 0.5f, heightSum);

	gui.add(playerNameLabel);
	gui.add(playerNameEditbox);
	gui.add(mazeSizeLabel);
	gui.add(mazeSizeCombobox);
	gui.add(fullscreenLabel);
	gui.add(fullscreenCheckbox);
	gui.add(resolutionLabel);
	gui.add(resolutionsCombobox);
	gui.add(enableSoundLabel);
	gui.add(enableSoundCheckbox);
	gui.add(effectsVolumeLabel);
	gui.add(effectsVolumeSlider);
	gui.add(musicVolumeLabel);
	gui.add(musicVolumeSlider);
	gui.add(statusLabel);
	gui.add(saveButton);
	gui.add(backButton);
}

std::string GameStateOptions::uiToGuiStrCrossPair(unsigned int nbr1, unsigned int nbr2)
{
	std::string retStr = " ";
	retStr = retStr + std::to_string(nbr1) + " x " + std::to_string(nbr2);
	return retStr;
}

sf::Vector2u GameStateOptions::guiStrCrossPairToUi(std::string guiCrossPairStr)
{
	unsigned int crossPos = guiCrossPairStr.find("x");
	unsigned int nbr1Start = 1u;
	unsigned int nbr1Length = crossPos - 2u;
	unsigned int nbr2Start = crossPos + 2u;
	unsigned int nbr2Length = guiCrossPairStr.length() - (crossPos + 2u);

	std::string nbr1 = guiCrossPairStr.substr(nbr1Start, nbr1Length);
	std::string nbr2 = guiCrossPairStr.substr(nbr2Start, nbr2Length);
	char *succ1;
	char *succ2;

	return sf::Vector2u(std::strtoul(nbr1.c_str(), &succ1, 10), std::strtoul(nbr2.c_str(), &succ2, 10));
}