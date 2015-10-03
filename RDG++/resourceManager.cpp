#include "resourceManager.hpp"
#include "prototypes.hpp"
#include "exceptions.hpp"
#include <memory>

ResourceManager& ResourceManager::getInstance()
{
	static ResourceManager instance;
	return instance;
}

void ResourceManager::loadResources(const std::string imagesPath, std::shared_ptr<PrototypeStorage> prototypeStorage)
{
	std::map<std::string, std::string> textureNames;

	for (std::string objectName : prototypeStorage->armamentFactory->getObjectNames()) {
		textureNames.insert(std::pair<std::string, std::string>(objectName, prototypeStorage->armamentFactory->getTemplate(objectName)->getTextureName()));
	}
	for (std::string objectName : prototypeStorage->monsterFactory->getObjectNames()) {
		textureNames.insert(std::pair<std::string, std::string>(objectName, prototypeStorage->monsterFactory->getTemplate(objectName)->getTextureName()));
	}
	for (std::string objectName : prototypeStorage->potionFactory->getObjectNames()) {
		textureNames.insert(std::pair<std::string, std::string>(objectName, prototypeStorage->potionFactory->getTemplate(objectName)->getTextureName()));
	}
	for (std::string objectName : prototypeStorage->weaponFactory->getObjectNames()) {
		textureNames.insert(std::pair<std::string, std::string>(objectName, prototypeStorage->weaponFactory->getTemplate(objectName)->getTextureName()));
	}
	for (std::string objectName : prototypeStorage->roomFactory->getObjectNames()) {
		textureNames.insert(std::pair<std::string, std::string>(objectName, prototypeStorage->roomFactory->getTemplate(objectName)->getTextureName()));
	}

	textureNames.insert(std::pair<std::string, std::string>("PlayerBig", "soldier_64x64.png"));
	textureNames.insert(std::pair<std::string, std::string>("PlayerSmall", "soldier_32x32.png"));
	textureNames.insert(std::pair<std::string, std::string>("Armor_Background", "warrior_160x160.png"));
	textureNames.insert(std::pair<std::string, std::string>("Accuracy_Stats", "accuracy_stats.png"));
	textureNames.insert(std::pair<std::string, std::string>("Strength_Stats", "strength_stats.png"));
	textureNames.insert(std::pair<std::string, std::string>("Speed_Stats", "speed_stats.png"));
	textureNames.insert(std::pair<std::string, std::string>("Key", "key.png"));

	//// TILES = new SpriteSheet(Game.IMAGEPATH + "/rooms/tileset.png", 32, 32);

	for (std::map<std::string, std::string>::iterator it = textureNames.begin(); it != textureNames.end(); ++it){
		sf::Texture texture;

		// avoid loading same texture twice
		if (!textures.count(it->first)) {
			if (!texture.loadFromFile(imagesPath + it->second))
			{
				throw LoadingException(imagesPath + it->second);
			}
			else
			{
				textures[it->first] = texture;
				std::cout << "loaded " << imagesPath + it->second << std::endl;
			}
		}
	}
}