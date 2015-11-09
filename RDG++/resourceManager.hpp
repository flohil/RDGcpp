#ifndef RESOURCEMANAGER_INCLUDE
#define RESOURCEMANAGER_INCLUDE

#include <SFML/Graphics.hpp>
#include <iostream>
#include <map>
#include <memory>
#include <list>
#include "settings.hpp"
#include <SFML/Audio.hpp>

class MusicInformation
{
public:

	MusicInformation(std::string path_) : path(path_), timeOffset(sf::seconds(0)) {};

	std::string path;
	sf::Time timeOffset;
};

class ResourceManager
{
public:

	static ResourceManager& getInstance()
	{
		static ResourceManager instance;
		return instance;
	};

	void setSettings(std::shared_ptr<Settings> settings_) { settings = settings_; };
	bool loadTexture(const std::string& textureName, const std::string& texturePath);
	bool loadAdditionalResources();
	sf::Texture& getTexture(const std::string& textureName) { return textures.at(textureName); };
	sf::Sprite& getRandomTile(const std::string& tileName);
	sf::Sound& getSound(const std::string& soundName) { return sounds.at(soundName); };
	void setSoundVolumes(const float effectsVolume);
	bool loadSound(const std::string soundName, const std::string filePath);
	bool loadMusicPath(const std::string musicName, const std::string filePath);
	std::shared_ptr<MusicInformation> getMusic(const std::string musicName) const { return musicPieces.at(musicName); };

private:

	std::shared_ptr<Settings> settings;
	std::map <const std::string, sf::Texture> textures;
	std::map <const std::string, sf::Sprite> sprites;
	std::map <const std::string, std::list<sf::Sprite>> tiles;
	std::map <const std::string, sf::SoundBuffer> soundBuffers;
	std::map <const std::string, sf::Sound> sounds;
	std::map <const std::string, std::shared_ptr<MusicInformation>> musicPieces;

	void loadTiles();
	
	ResourceManager() {};
	ResourceManager(ResourceManager const&);
	~ResourceManager();
	void operator = (ResourceManager const&);
};

#endif // RESOURCEMANAGER_INCLUDE