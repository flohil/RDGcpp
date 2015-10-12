#ifndef GAME_STATE_HPP
#define GAME_STATE_HPP

#include "game.hpp"

class GameState
{
public:

	Game& game;
	std::shared_ptr < Settings > settings;

	GameState(Game& game_) : game(game_) {};

	virtual void draw(const float deltaTime) = 0;
	virtual void update(const float deltaTime) = 0;
	virtual void handleInput() = 0;
};

#endif /* GAME_STATE_HPP */