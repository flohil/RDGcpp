#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

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
	// check for memory leaks
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);

// initialize logging

	std::remove("logs/rdg++.log"); // delete custom log file at every game start

	// load logger configuration
	el::Configurations conf("logging.conf");
	el::Loggers::reconfigureAllLoggers(conf);

	std::remove("logs/myeasylog.log"); // delete standard log file

// start game

	Game game;

	if (!game.initializedSuccessfully())
	{
		LOG(ERROR) << "Game initialization failed. Shutting down...";
		return -1;
	}
	else {
		LOG(INFO) << "Game initialized successfully";
	}

	game.pushState(new GameStateMainMenu(game));
	game.gameLoop();

	LOG(INFO) << "Shutting down...";
	return 0;
}

