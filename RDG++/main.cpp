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
#include "gameStateMainMenu.hpp"

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

	game.pushState(new GameStateMainMenu(game));
	game.gameLoop();

	LOG(INFO) << "Shutting down...";
	return 0;
}

