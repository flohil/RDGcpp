#include "resourceManager.hpp"
#include "prototypes.hpp"
#include <memory>

// free memory from textures
ResourceManager::~ResourceManager() 
{

}

bool ResourceManager::loadResources(const std::string imagesPath, std::shared_ptr<PrototypeStorage> prototypeStorage)
{
	std::map<std::string, std::string> textureNames;
	sf::Image tilesSpritesheet;

	for (std::string objectName : prototypeStorage->armamentFactory->getObjectNames()) {
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
	}

	// load room textures from tileset, not directly
	/*for (std::string objectName : prototypeStorage->roomFactory->getObjectNames()) {
		textureNames.insert(std::pair<std::string, std::string>(objectName, prototypeStorage->roomFactory->getTemplate(objectName)->getTextureName()));
	}*/

	textureNames.insert(std::pair<std::string, std::string>("PlayerBig", "soldier_64x64.png"));
	textureNames.insert(std::pair<std::string, std::string>("PlayerSmall", "soldier_32x32.png"));
	textureNames.insert(std::pair<std::string, std::string>("Armor_Background", "warrior_160x160.png"));
	textureNames.insert(std::pair<std::string, std::string>("Accuracy_Stats", "accuracy_stats.png"));
	textureNames.insert(std::pair<std::string, std::string>("Strength_Stats", "strength_stats.png"));
	textureNames.insert(std::pair<std::string, std::string>("Speed_Stats", "speed_stats.png"));
	textureNames.insert(std::pair<std::string, std::string>("Key", "key.png"));

	// load single textures
	for (std::map<std::string, std::string>::iterator it = textureNames.begin(); it != textureNames.end(); ++it){
		
		sf::Texture texture;

		// avoid loading same texture twice
		if (!textures.count(it->first)) {
			if (!texture.loadFromFile(imagesPath + it->second))
			{
				std::cerr << "Failed to load texture " << imagesPath + it->second << std::endl;
				return false;
			}
			else
			{
				std::shared_ptr<sf::Texture> texturePointer(new sf::Texture(texture));
				textures[it->first] = texturePointer;
				std::cout << "loaded " << imagesPath + it->second << std::endl;
			}
		}
	}

	// load room tiles textures from spritesheet
	tilesSpritesheet.loadFromFile("rooms/tileset.png");

	sf::Texture groundTexture;	

	return true;
}