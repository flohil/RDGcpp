#ifndef SETTINGS_INCLUDE
#define SETTINGS_INCLUDE

#include "SettingsParser.hpp"
#include <iostream>

// don't make Settings a singleton - could use different settings for different profiles!
class Settings
{
public:

	// constants
	const std::string APPNAME;
	const std::string SETTINGS_FILE_PATH;
	const std::string CONFIG_PATH;
	const std::string IMAGE_PATH;
	const unsigned int COLOR_DEPTH;
	const unsigned int ROOM_WIDTH; // in tiles excluding walls
	const unsigned int ROOM_HEIGHT; // in tiles excluding walls
	const unsigned int MAX_TRIES; // number of tries when looking for a free field in a room

	// variables
	unsigned int width;
	unsigned int height;
	bool fullscreen;
	unsigned int mazeSize;
	unsigned int tileSize;

	// functions
	Settings(unsigned int desktopWidth, unsigned int desktopHeight);
	void writeDefaultSettings();
	void saveSettings();
	bool loadSettings();
	bool loadedSuccesfully() { return successfullyLoaded; };

private:

	SettingsParser settingsParser;
	bool successfullyLoaded = false;
};

#endif // SETTINGS_INCLUDE