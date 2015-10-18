#ifndef PLAYER_INCLUDE
#define PLAYER_INCLUDE

#include "gameObjects.hpp"

class Player : public Creature
{
public:
	Player();
	~Player();

private:
	// VARIABLES
	bool up, right, down, left;
	bool keyReleasedUp, keyReleasedDown, keyReleasedLeft, keyReleasedRight;
	ViewingDirections lastViewingDirection;
	// Point originOfGameEnvironment;
	// Point cameraPosition;	// tracks the position of the player in the camera
	std::vector<std::vector<bool>> visited;	// keeps track of visited rooms
	//bool[][] camera;	// kind of camera simulation, that is used for special background movement
	//Map map;

	// METHODS

};

#endif //PLAYER_INCLUDE