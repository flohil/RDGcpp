#ifndef MAP
#define MAP

#include <vector>
#include <map>
#include "gameObjects.hpp"
#include "game.hpp"

class Map
{
public:

	Map(Game& game_) : 
		game(game_), settings(game_.getSettings()),
		width((game_.getSettings()->ROOM_WIDTH + 1) * game_.getSettings()->mazeSize + 1), 
		height((game_.getSettings()->ROOM_HEIGHT + 1) * game_.getSettings()->mazeSize + 1) {};

	void init(std::shared_ptr<Player> player_);
	void update(const float deltaTime);
	Point initPlayerPosition();
	std::vector<std::vector<std::shared_ptr<RenderableObject>>> getBackround() const { return background; };
	std::vector<std::vector<std::shared_ptr<RenderableObject>>> getOverlay() const { return overlay; };
	std::shared_ptr<RenderableObject> getOverlayObject(Point pos) const { return overlay[pos.y][pos.x]; };
	std::shared_ptr<Room> getRoom(Point pos) { return rooms[pos.y][pos.x]; };
	std::shared_ptr<RenderableObject> setOverlayObject(Point pos, std::shared_ptr<RenderableObject> obj) { return placeInOverlay(pos, obj); };
	bool isFieldPassable(Point fieldPos) const;
	void draw(sf::RenderWindow& window, float deltaTime);
	void openTreasureChamber();
	void closeTreasureChamber();
	std::shared_ptr<RenderableObject> getItemAtPixels(sf::Vector2i pixelPos) { return handleItemAtPixels(pixelPos, false, nullptr); };
	std::shared_ptr<RenderableObject> getItemAtPixels(sf::Vector2i pixelPos, bool remove) { return handleItemAtPixels(pixelPos, remove, nullptr); };
	std::shared_ptr<RenderableObject> setItemAtPixels(sf::Vector2i pixelPos, std::shared_ptr<RenderableObject> obj) { return handleItemAtPixels(pixelPos, false, obj); };
	std::shared_ptr<RenderableObject> handleItemAtPixels(sf::Vector2i pixelPos, bool remove, std::shared_ptr<RenderableObject> obj);
	Point getInitialPlayerPosition() const { return initialPlayerPosition; };

private:

	Game& game;
	std::unique_ptr<Maze> maze;
	std::shared_ptr<Settings> settings;
	std::shared_ptr<Player> player;

	std::vector<std::vector<std::shared_ptr<RenderableObject>>> background;
	std::vector<std::vector<std::shared_ptr<RenderableObject>>> overlay;
	std::vector<std::vector<std::shared_ptr<Room>>> rooms;

	std::map<std::string, unsigned int> easyMap;
	std::map<std::string, unsigned int> normalMap;
	std::map<std::string, unsigned int> hardMap;

	std::map<std::string, ItemBalance> weakMap;
	std::map<std::string, ItemBalance> mediumMap;
	std::map<std::string, ItemBalance> strongMap;

	const unsigned int width;
	const unsigned int height;

	// ensure that spawned items are a little balanced
	std::map<DifficultyLevel::Enum, std::map<std::string, unsigned int>> monsterBalance;
	std::map<Classes::Enum, std::map<std::string, ItemBalance>> itemsBalance;
	std::map<std::string, unsigned int> monsterBalanceOffsets;
	std::map<std::string, unsigned int> itemBalanceOffsets;

	// offsets for balance counters
	const unsigned int easyMonsterOffset = 1;
	const unsigned int normalMonsterOffset = 1;
	const unsigned int hardMonsterOffset = 1;
	const unsigned int weakItemOffset = 1;
	const unsigned int mediumItemOffset = 1;
	const unsigned int strongItemOffset = 1;

	// treasure chamber open doors positions
	Point treasureDoorOne;
	Point treasureDoorTwo;
	float treasureDoorOneAngle;
	float treasureDoorTwoAngle;
	bool treasureDoorOpened = false;

	// initial player position
	Point initialPlayerPosition;

	void fillWithRooms();
	RoomTypes::Enum detectRoomType(Point indizes) const;
	void loadRooms();
	void fillWalls();
	void setDoors(Point roomIndizes, ViewingDirections::Enum dir);
	void placeKey();
	void increaseMonsterBalance(std::string name);
	void increaseItemBalance(std::string name);
	void fillGround(std::shared_ptr<Room> room, RoomTypes::Enum type);
	void addMonster(std::shared_ptr<Room> room, RoomTypes::Enum type);
	void addItems(std::shared_ptr<Room> room, RoomTypes::Enum type);
	std::shared_ptr<RenderableObject> placeInOverlay(Point pos, std::shared_ptr<RenderableObject> obj);
	std::shared_ptr<RenderableObject> placeInBackground(Point pos, std::shared_ptr<RenderableObject> obj);
};

#endif /* MAP */