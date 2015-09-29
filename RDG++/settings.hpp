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
	const unsigned int COLOR_DEPTH;

	// variables
	unsigned int width;
	unsigned int height;
	bool fullscreen;

	// functions
	Settings(unsigned int desktopWidth, unsigned int desktopHeight);
	void writeDefaultSettings();
	void saveSettings();
	bool loadSettings();

private:

	SettingsParser settingsParser;
};

#endif // SETTINGS_INCLUDE