#include "settings.hpp"
#include "exceptions.hpp"
#include <iostream>
#include <fstream>

Settings::Settings(unsigned int desktopWidth, unsigned int desktopHeight) : 
	COLOR_DEPTH( 32 ),
	APPNAME("RDG++"),
	SETTINGS_FILE_PATH("settings.txt"),
	CONFIG_PATH("config/"),
	IMAGE_PATH("assets/")
{

	//set default values for variables
	fullscreen = true;
	width = desktopWidth;
	height = desktopHeight;

	try
	{
		loadSettings();
	} 
	catch (LocationException& e)
	{
		throw e;
	}	
}

// write initial default settings to newly created output file
void Settings::writeDefaultSettings() 
{

	std::cout << "Writing default settings." << std::endl;

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
}

// loads settings from settings file
bool Settings::loadSettings()
{

	// try to create new settings file if none exists
	if (!settingsParser.loadFromFile(SETTINGS_FILE_PATH))
	{
		std::cout << "Settings file not found! Trying to create new settings file..." << std::endl;

		// create settings file with default settings
		writeDefaultSettings();
	}

	// load settings file
	if (!settingsParser.loadFromFile(SETTINGS_FILE_PATH))
	{
		std::cout << "Error loading settings file!" << std::endl;
		throw LocationException(SETTINGS_FILE_PATH);
	}
	else
	{
		std::cout << "Loading settings from " << SETTINGS_FILE_PATH << std::endl;

		settingsParser.get("width", width);
		settingsParser.get("height", height);
		settingsParser.get("fullscreen", fullscreen);

		settingsParser.print();
	}

	return true;
}