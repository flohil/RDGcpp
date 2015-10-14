#include "resourceManager.hpp"
#include "prototypes.hpp"
#include <memory>
#include "easylogging++.hpp"
#include "random.hpp"

// free memory from textures
ResourceManager::~ResourceManager() 
{

}

bool ResourceManager::loadTexture(const std::string& textureName, const std::string& texturePath)
{
	sf::Texture texture;

	// avoid loading same texture twice
	if (!textures.count(textureName)) {
		if (!texture.loadFromFile(settings->IMAGE_PATH + texturePath))
		{
			LOG(ERROR) << "Failed to load texture " << settings->IMAGE_PATH + texturePath;
			return false;
		}
		else
		{
			textures[textureName] = texture;

			sf::Sprite sprite;
			sprite.setTexture(texture);
			sprites.insert(std::pair<const std::string, sf::Sprite>(textureName, sprite));

			LOG(DEBUG) << "loaded " << settings->IMAGE_PATH + texturePath;
		}
	}

	return true;
}

bool ResourceManager::loadAdditionalResources()
{
	std::map<std::string, std::string> textureNames;

	/*for (std::string objectName : prototypeStorage->armamentFactory->getObjectNames()) {
		textureNames.insert(std::pair<std::string, std::string>(objectName, std::static_pointer_cast<ArmamentTemplate>(prototypeStorage->armamentFactory->getTemplate(objectName))->getTextureName()));
	}
	for (std::string objectName : prototypeStorage->monsterFactory->getObjectNames()) {
		textureNames.insert(std::pair<std::string, std::string>(objectName, std::static_pointer_cast<MonsterTemplate>(prototypeStorage->monsterFactory->getTemplate(objectName))->getTextureName()));
	}
	for (std::string objectName : prototypeStorage->potionFactory->getObjectNames()) {
		textureNames.insert(std::pair<std::string, std::string>(objectName, std::static_pointer_cast<PotionTemplate>(prototypeStorage->potionFactory->getTemplate(objectName))->getTextureName()));
	}
	for (std::string objectName : prototypeStorage->weaponFactory->getObjectNames()) {
		textureNames.insert(std::pair<std::string, std::string>(objectName, std::static_pointer_cast<WeaponTemplate>(prototypeStorage->weaponFactory->getTemplate(objectName))->getTextureName()));
	}*/

	textureNames.insert(std::pair<std::string, std::string>("PlayerBig", "soldier_64x64.png"));
	textureNames.insert(std::pair<std::string, std::string>("PlayerSmall", "soldier_32x32.png"));
	textureNames.insert(std::pair<std::string, std::string>("Armor_Background", "warrior_160x160.png"));
	textureNames.insert(std::pair<std::string, std::string>("Accuracy_Stats", "accuracy_stats.png"));
	textureNames.insert(std::pair<std::string, std::string>("Strength_Stats", "strength_stats.png"));
	textureNames.insert(std::pair<std::string, std::string>("Speed_Stats", "speed_stats.png"));
	textureNames.insert(std::pair<std::string, std::string>("Key", "key.png"));
	textureNames.insert(std::pair<std::string, std::string>("tileset", "rooms/tileset.png"));

	// load single textures
	for (std::map<std::string, std::string>::iterator it = textureNames.begin(); it != textureNames.end(); ++it)
	{
		loadTexture(it->first, it->second);
	}

	loadTiles();

	LOG(INFO) << "Resources loaded successfully";

	return true;
}

void ResourceManager::loadTiles()
{
	sf::Texture& tileset = textures.at("tileset");
	unsigned int ts = 32;
	typedef std::pair<const std::string, std::list<sf::Sprite>> spritePair;

	std::list<sf::Sprite> greyGrounds;
	greyGrounds.push_back(sf::Sprite(tileset, sf::IntRect(1 * ts, 0, ts, ts)));
	greyGrounds.push_back(sf::Sprite(tileset, sf::IntRect(1 * ts, 1 * ts, ts, ts)));
	greyGrounds.push_back(sf::Sprite(tileset, sf::IntRect(1 * ts, 2 * ts, ts, ts)));
	greyGrounds.push_back(sf::Sprite(tileset, sf::IntRect(1 * ts, 3 * ts, ts, ts)));
	greyGrounds.push_back(sf::Sprite(tileset, sf::IntRect(2 * ts, 0, ts, ts)));
	greyGrounds.push_back(sf::Sprite(tileset, sf::IntRect(2 * ts, 1 * ts, ts, ts)));
	greyGrounds.push_back(sf::Sprite(tileset, sf::IntRect(2 * ts, 2 * ts, ts, ts)));
	greyGrounds.push_back(sf::Sprite(tileset, sf::IntRect(2 * ts, 3 * ts, ts, ts)));
	tiles.insert(spritePair("greyGrounds", greyGrounds));

	std::list<sf::Sprite> darkGreyGrounds;
	darkGreyGrounds.push_back(sf::Sprite(tileset, sf::IntRect(3 * ts, 3 * ts, ts, ts)));
	darkGreyGrounds.push_back(sf::Sprite(tileset, sf::IntRect(3 * ts, 4 * ts, ts, ts)));
	darkGreyGrounds.push_back(sf::Sprite(tileset, sf::IntRect(3 * ts, 5 * ts, ts, ts)));
	darkGreyGrounds.push_back(sf::Sprite(tileset, sf::IntRect(3 * ts, 6 * ts, ts, ts)));
	darkGreyGrounds.push_back(sf::Sprite(tileset, sf::IntRect(4 * ts, 3, ts, ts)));
	darkGreyGrounds.push_back(sf::Sprite(tileset, sf::IntRect(4 * ts, 4 * ts, ts, ts)));
	darkGreyGrounds.push_back(sf::Sprite(tileset, sf::IntRect(4 * ts, 5 * ts, ts, ts)));
	darkGreyGrounds.push_back(sf::Sprite(tileset, sf::IntRect(4 * ts, 6 * ts, ts, ts)));
	tiles.insert(spritePair("darkGreyGrounds", darkGreyGrounds));

	std::list<sf::Sprite> yellowOneGrounds;
	yellowOneGrounds.push_back(sf::Sprite(tileset, sf::IntRect(3 * ts, 0, ts, ts)));
	yellowOneGrounds.push_back(sf::Sprite(tileset, sf::IntRect(3 * ts, 1 * ts, ts, ts)));
	yellowOneGrounds.push_back(sf::Sprite(tileset, sf::IntRect(3 * ts, 2 * ts, ts, ts)));
	yellowOneGrounds.push_back(sf::Sprite(tileset, sf::IntRect(4 * ts, 0, ts, ts)));
	yellowOneGrounds.push_back(sf::Sprite(tileset, sf::IntRect(4 * ts, 1 * ts, ts, ts)));
	yellowOneGrounds.push_back(sf::Sprite(tileset, sf::IntRect(4 * ts, 2 * ts, ts, ts)));
	yellowOneGrounds.push_back(sf::Sprite(tileset, sf::IntRect(5 * ts, 0, ts, ts)));
	yellowOneGrounds.push_back(sf::Sprite(tileset, sf::IntRect(5 * ts, 1 * ts, ts, ts)));
	yellowOneGrounds.push_back(sf::Sprite(tileset, sf::IntRect(5 * ts, 2 * ts, ts, ts)));
	tiles.insert(spritePair("yellowOneGrounds", yellowOneGrounds));

	std::list<sf::Sprite> yellowTwoGrounds;
	yellowTwoGrounds.push_back(sf::Sprite(tileset, sf::IntRect(6 * ts, 0, ts, ts)));
	yellowTwoGrounds.push_back(sf::Sprite(tileset, sf::IntRect(6 * ts, 1 * ts, ts, ts)));
	yellowTwoGrounds.push_back(sf::Sprite(tileset, sf::IntRect(6 * ts, 2 * ts, ts, ts)));
	yellowTwoGrounds.push_back(sf::Sprite(tileset, sf::IntRect(7 * ts, 0, ts, ts)));
	yellowTwoGrounds.push_back(sf::Sprite(tileset, sf::IntRect(7 * ts, 1 * ts, ts, ts)));
	yellowTwoGrounds.push_back(sf::Sprite(tileset, sf::IntRect(7 * ts, 2 * ts, ts, ts)));
	yellowTwoGrounds.push_back(sf::Sprite(tileset, sf::IntRect(8 * ts, 0, ts, ts)));
	yellowTwoGrounds.push_back(sf::Sprite(tileset, sf::IntRect(8 * ts, 1 * ts, ts, ts)));
	yellowTwoGrounds.push_back(sf::Sprite(tileset, sf::IntRect(8 * ts, 2 * ts, ts, ts)));
	tiles.insert(spritePair("yellowTwoGrounds", yellowTwoGrounds));

	std::list<sf::Sprite> greenGrounds;
	greenGrounds.push_back(sf::Sprite(tileset, sf::IntRect(5 * ts, 4 * ts, ts, ts)));
	greenGrounds.push_back(sf::Sprite(tileset, sf::IntRect(5 * ts, 5 * ts, ts, ts)));
	greenGrounds.push_back(sf::Sprite(tileset, sf::IntRect(5 * ts, 6 * ts, ts, ts)));
	greenGrounds.push_back(sf::Sprite(tileset, sf::IntRect(6 * ts, 4 * ts, ts, ts)));
	greenGrounds.push_back(sf::Sprite(tileset, sf::IntRect(6 * ts, 5 * ts, ts, ts)));
	greenGrounds.push_back(sf::Sprite(tileset, sf::IntRect(6 * ts, 6 * ts, ts, ts)));
	greenGrounds.push_back(sf::Sprite(tileset, sf::IntRect(7 * ts, 4 * ts, ts, ts)));
	greenGrounds.push_back(sf::Sprite(tileset, sf::IntRect(7 * ts, 5 * ts, ts, ts)));
	greenGrounds.push_back(sf::Sprite(tileset, sf::IntRect(7 * ts, 6 * ts, ts, ts)));
	tiles.insert(spritePair("greenGrounds", greenGrounds));

	std::list<sf::Sprite> brownGrounds;
	brownGrounds.push_back(sf::Sprite(tileset, sf::IntRect(0, 4 * ts, ts, ts)));
	brownGrounds.push_back(sf::Sprite(tileset, sf::IntRect(1 * ts, 5 * ts, ts, ts)));
	brownGrounds.push_back(sf::Sprite(tileset, sf::IntRect(2 * ts, 6 * ts, ts, ts)));
	brownGrounds.push_back(sf::Sprite(tileset, sf::IntRect(0, 4 * ts, ts, ts)));
	brownGrounds.push_back(sf::Sprite(tileset, sf::IntRect(1 * ts, 5 * ts, ts, ts)));
	brownGrounds.push_back(sf::Sprite(tileset, sf::IntRect(2 * ts, 6 * ts, ts, ts)));
	brownGrounds.push_back(sf::Sprite(tileset, sf::IntRect(0, 4 * ts, ts, ts)));
	brownGrounds.push_back(sf::Sprite(tileset, sf::IntRect(1 * ts, 5 * ts, ts, ts)));
	brownGrounds.push_back(sf::Sprite(tileset, sf::IntRect(2 * ts, 6 * ts, ts, ts)));
	tiles.insert(spritePair("brownGrounds", brownGrounds));

	std::list<sf::Sprite> doorGroundOne;
	doorGroundOne.push_back(sf::Sprite(tileset, sf::IntRect(5 * ts, 3 * ts, ts, ts)));
	tiles.insert(spritePair("doorGroundOne", doorGroundOne));

	std::list<sf::Sprite> doorGroundTwo;
	sf::Sprite doorGroundFlipped = sf::Sprite(tileset, sf::IntRect(5 * ts, 3 * ts, ts, ts));
	doorGroundFlipped.setRotation(180.0f);
	doorGroundTwo.push_back(doorGroundFlipped);
	tiles.insert(spritePair("doorGroundTwo", doorGroundTwo));
}

sf::Sprite& ResourceManager::getRandomTile(const std::string& tileName)
{
	return *select_randomly(tiles[tileName].begin(), tiles[tileName].end());
}