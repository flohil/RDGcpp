#ifndef RESOURCEMANAGER_INCLUDE
#define RESOURCEMANAGER_INCLUDE

#include <SFML/Graphics.hpp>
#include <iostream>
#include <map>
#include <memory>

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
	std::shared_ptr<sf::Texture> getTexture(const std::string& textureName) { return textures[textureName]; };

private:

	std::map <const std::string, std::shared_ptr<sf::Texture>> textures;

	ResourceManager() {};
	ResourceManager(ResourceManager const&);
	~ResourceManager();
	void operator = (ResourceManager const&);
};

#endif // RESOURCEMANAGER_INCLUDE