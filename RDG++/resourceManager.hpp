#ifndef RESOURCEMANAGER_INCLUDE
#define RESOURCEMANAGER_INCLUDE

#include <SFML/Graphics.hpp>
#include "prototypes.hpp"
#include <iostream>
#include <map>
#include <memory>

class ResourceManager
{
public:

	static ResourceManager& getInstance();
	void loadResources(const std::string imagesPath, std::shared_ptr<PrototypeStorage> prototypeStorage);

private:

	std::map <std::string, sf::Texture> textures;

	ResourceManager() {};
	ResourceManager(ResourceManager const&);
	void operator = (ResourceManager const&);
};

#endif // RESOURCEMANAGER_INCLUDE