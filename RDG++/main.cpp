#include <SFML/Graphics.hpp>
#include "settings.hpp"
#include "prototypes.hpp"
#include "enums.hpp"
#include "gameObjects.hpp"
#include "debugPrint.hpp"
#include "resourceManager.hpp"
#include <iostream>
#include <fstream>
#include <memory>
#include "easylogging++.hpp"
#include "game.hpp"

INITIALIZE_EASYLOGGINGPP

int main()
{

//initialize logging

	std::remove("logs/rdg++.log"); // delete custom log file at every game start

	// load logger configuration
	el::Configurations conf("logging.conf");
	el::Loggers::reconfigureAllLoggers(conf);

	std::remove("logs/myeasylog.log"); // delete standard log file

//start game

	Game game;

	if (!game.initializedSuccessfully())
	{
		LOG(ERROR) << "Game initialization failed. Shutting down...";
		return -1;
	}

	if (!game.gameLoop())
	{
		LOG(ERROR) << "Shutting down with errors...";
		return -1;
	}

	LOG(INFO) << "Shutting down...";
	return 0;
}

