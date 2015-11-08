#include "settings.hpp"
#include <iostream>
#include <fstream>
#include "easylogging++.hpp"

Settings::Settings(unsigned int desktopWidth, unsigned int desktopHeight) : 
	APPNAME("RDG++"),
	SETTINGS_FILE_PATH("settings.txt"),
	CONFIG_PATH("config/"),
	IMAGE_PATH("res/"),
	SOUNDS_PATH("res/sounds/"),
	COLOR_DEPTH(32), 
	ROOM_WIDTH(8),
	ROOM_HEIGHT(6),
	MAX_TRIES(15),
	scaleWidth(1280),
	scaleHeight(720)
{

	//set default values for variables
	fullscreen = true;
	width = desktopWidth;
	height = desktopHeight;
	widthScaleFactor = static_cast<float>(width) / static_cast<float>(scaleWidth);
	heightScaleFactor = static_cast<float>(height) / static_cast<float>(scaleHeight);
	widthDownScaleFactor = 1.f / widthScaleFactor;
	heightDownScaleFactor = 1.f / heightScaleFactor;
	mazeSize = 7;
	tileSize = 32;
	creatureSize = 128;
	playerName = "Player";
	maxInventorySize = 15;
	buttonTextSize = 32u;
	labelBigTextSize = 24u;
	labelSmallTextSize = 22u;
	widgetHeight = 60u;
	defWidgetWidth = 450u;
	labelPaddingX = 8.f;
	labelBigHeight = 36.f;
	labelSmallHeight = 34.f;
	sliderHeight = 23.f;
	enableSound = true;
	effectsVolume = 100.f;
	musicVolume = 100.f;
	actualEffectsVolume = effectsVolume;
	actualMusicVolume = musicVolume;

	successfullyLoaded = loadSettings();
}

// write initial default settings to newly created output file
void Settings::writeDefaultSettings() 
{

	LOG(INFO) << "Writing default settings.";

	// open settings file
	std::ofstream outfile(SETTINGS_FILE_PATH);

	// write video mode variables
	outfile << "# VideoMode" << std::endl;
	outfile << "width = " << width << std::endl;
	outfile << "height = " << height << std::endl;
	outfile << "fullscreen = " << ((fullscreen == true) ? "TRUE" : "FALSE") << std::endl;
	outfile << std::endl;
	outfile << "# Game Settings" << std::endl;
	outfile << "mazeSize = " << mazeSize << std::endl;
	outfile << "enableSound = " << ((enableSound == true) ? "TRUE" : "FALSE") << std::endl;
	outfile << "effectsVolume = " << effectsVolume << std::endl;
	outfile << "musicVolume = " << musicVolume << std::endl;
	outfile << "playerName = " << playerName << std::endl;

	// close settings file
	outfile.close();
}

// writes settings to settings file
void Settings::saveSettings() 
{

	std::cout << "Writing settings to " << SETTINGS_FILE_PATH << std::endl;

	settingsParser.set("width", width);
	settingsParser.set("height", height);
	settingsParser.set("fullscreen", fullscreen);
	settingsParser.set("mazeSize", mazeSize);
	settingsParser.set("enableSound", enableSound);
	settingsParser.set("effectsVolume", effectsVolume);
	settingsParser.set("musicVolume", musicVolume);
	settingsParser.set("playerName", playerName);

	settingsParser.saveToFile();
	settingsParser.print();

	LOG(INFO) << "Writing settings to " << SETTINGS_FILE_PATH;
}

// loads settings from settings file
bool Settings::loadSettings()
{

	// try to create new settings file if none exists
	if (!settingsParser.loadFromFile(SETTINGS_FILE_PATH))
	{
		LOG(WARNING) << "Settings file not found! Trying to create new settings file...";

		// create settings file with default settings
		writeDefaultSettings();
	}

	// load settings file
	if (!settingsParser.loadFromFile(SETTINGS_FILE_PATH))
	{
		LOG(ERROR) << "Error loading settings file!";
		return false;
	}
	else
	{
		LOG(INFO) << "Loading settings from " << SETTINGS_FILE_PATH;

		settingsParser.get("width", width);
		settingsParser.get("height", height);
		settingsParser.get("fullscreen", fullscreen);
		settingsParser.get("mazeSize", mazeSize);
		settingsParser.get("tileSize", tileSize);
		settingsParser.get("enableSound", enableSound);
		settingsParser.get("effectsVolume", effectsVolume);
		settingsParser.get("musicVolume", musicVolume);
		settingsParser.get("playerName", playerName);

		if (playerName.length() == 0)
		{
			playerName = "Player";
		}

		// mazeSize must be uneven number >= 3
		if (mazeSize < 3 || (mazeSize % 2) != 1)
		{
			LOG(WARNING) << "mazeSize must be uneven number >= 3 but was " << mazeSize << ".";
			LOG(INFO) << "Settings mazeSize to 3.";

			mazeSize = 3;
		}

		if (effectsVolume < 0 || effectsVolume > 100.f)
		{
			effectsVolume = 100.f;
		}

		if (musicVolume < 0 || musicVolume > 100.f)
		{
			musicVolume = 100.f;
		}

		if (enableSound)
		{
			actualEffectsVolume = effectsVolume;
			actualMusicVolume = musicVolume;
		}
		else
		{
			actualEffectsVolume = 0.f;
			actualMusicVolume = 0.f;
		}

		std::cout << "scalewidht: " << scaleWidth << std::endl;
		std::cout << "scaleheight: " << scaleHeight << std::endl;

		std::cout << "width: " << width << std::endl;
		std::cout << "height: " << height << std::endl;

		widthScaleFactor = static_cast<float>(width) / static_cast<float>(scaleWidth);
		heightScaleFactor = static_cast<float>(height) / static_cast<float>(scaleHeight);
		widthDownScaleFactor = 1.f / widthScaleFactor;
		heightDownScaleFactor = 1.f / heightScaleFactor;

		std::cout << "widthScaleFactor: " << widthScaleFactor << std::endl;
		std::cout << "heightScaleFactor: " << heightScaleFactor << std::endl;
		std::cout << "widthDownScaleFactor: " << widthDownScaleFactor << std::endl;
		std::cout << "heightDownScaleFactor: " << heightDownScaleFactor << std::endl;

		settingsParser.print();
	}

	LOG(INFO) << "Settings loaded successfully";

	return true;
}