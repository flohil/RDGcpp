#ifndef RESOURCEMANAGER_INCLUDE
#define RESOURCEMANAGER_INCLUDE

#include <SFML/Graphics.hpp>
#include <iostream>
#include <map>
#include <memory>
#include <list>

class PrototypeStorage;

class ResourceManager
{
public:

	static ResourceManager& getInstance()
	{
		static ResourceManager instance;
		return instance;
	};

	bool loadResources(const std::string imagesPath, std::shared_ptr<PrototypeStorage> prototypeStorage);
	sf::Texture& getTexture(const std::string& textureName) { return textures.at(textureName); };
	sf::Sprite& getSprite(const std::string& spriteName) { return sprites.at(spriteName); };
	sf::Sprite& getRandomTile(const std::string& tileName);

private:

	std::map <const std::string, sf::Texture> textures;
	std::map <const std::string, sf::Sprite> sprites;
	std::map <const std::string, std::list<sf::Sprite>> tiles;

	void loadTiles();

	ResourceManager() {};
	ResourceManager(ResourceManager const&);
	~ResourceManager();
	void operator = (ResourceManager const&);
};

#endif // RESOURCEMANAGER_INCLUDE