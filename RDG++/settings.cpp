#include "settings.hpp"
#include <iostream>
#include <fstream>
#include "easylogging++.hpp"

Settings::Settings(unsigned int desktopWidth, unsigned int desktopHeight) : 
	APPNAME("RDG++"),
	SETTINGS_FILE_PATH("settings.txt"),
	CONFIG_PATH("config/"),
	IMAGE_PATH("res/"),
	COLOR_DEPTH(32)
{

	//set default values for variables
	fullscreen = true;
	width = desktopWidth;
	height = desktopHeight;

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

		settingsParser.print();
	}

	LOG(INFO) << "Settings loaded successfully";

	return true;
}