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
			texture.setSmooth(true);
			textures[textureName] = texture;
			LOG(DEBUG) << "loaded " << settings->IMAGE_PATH + texturePath;
		}
	}

	return true;
}

bool ResourceManager::loadAdditionalResources()
{
	std::map<std::string, std::string> textureNames;
	std::map<std::string, std::string> soundNames;
	std::map<std::string, std::string> musicNames;

	textureNames.insert(std::pair<std::string, std::string>("player_big", "player_big.png"));
	textureNames.insert(std::pair<std::string, std::string>("player", "player.png"));
	textureNames.insert(std::pair<std::string, std::string>("armorBackground", "warrior_160x160.png"));
	textureNames.insert(std::pair<std::string, std::string>("potionBar", "balken.png"));
	textureNames.insert(std::pair<std::string, std::string>("accuracyStats", "accuracy_stats.png"));
	textureNames.insert(std::pair<std::string, std::string>("strengthStats", "strength_stats.png"));
	textureNames.insert(std::pair<std::string, std::string>("speedStats", "speed_stats.png"));
	textureNames.insert(std::pair<std::string, std::string>("key", "key.png"));
	textureNames.insert(std::pair<std::string, std::string>("tileset", "rooms/tileset.png"));
	textureNames.insert(std::pair<std::string, std::string>("icon", "icon.png"));
	textureNames.insert(std::pair<std::string, std::string>("background", "background.png"));
	textureNames.insert(std::pair<std::string, std::string>("hp", "hp.png"));
	textureNames.insert(std::pair<std::string, std::string>("accuracy", "accuracy.png"));
	textureNames.insert(std::pair<std::string, std::string>("speed", "speed.png"));
	textureNames.insert(std::pair<std::string, std::string>("strength", "strength.png"));

	soundNames.insert(std::pair<std::string, std::string>("buttonClick", "gui_control_response.ogg"));
	soundNames.insert(std::pair<std::string, std::string>("guiControlResponse", "gui_control_response.ogg"));
	soundNames.insert(std::pair<std::string, std::string>("keyStroke", "keyStroke.ogg"));
	soundNames.insert(std::pair<std::string, std::string>("keyStrokeReturn", "keyStrokeReturn.ogg"));
	soundNames.insert(std::pair<std::string, std::string>("error", "error.ogg"));
	soundNames.insert(std::pair<std::string, std::string>("dropItem", "drop_item.ogg"));
	soundNames.insert(std::pair<std::string, std::string>("putInInventory", "put_in_inventory.ogg"));
	soundNames.insert(std::pair<std::string, std::string>("footsteps", "footsteps.ogg"));
	soundNames.insert(std::pair<std::string, std::string>("key", "keys.ogg"));
	soundNames.insert(std::pair<std::string, std::string>("drink", "drink.ogg"));
	soundNames.insert(std::pair<std::string, std::string>("humanDies", "humanDies.ogg"));
	soundNames.insert(std::pair<std::string, std::string>("humanHit", "humanHit.ogg"));
	soundNames.insert(std::pair<std::string, std::string>("miss", "miss.ogg"));
	soundNames.insert(std::pair<std::string, std::string>("forceParry", "parry.ogg"));
	soundNames.insert(std::pair<std::string, std::string>("changeSet", "changeSet.ogg"));

	musicNames.insert(std::pair<std::string, std::string>("mainMenu", "mainMenu.ogg"));
	musicNames.insert(std::pair<std::string, std::string>("loading", "loading.ogg"));
	musicNames.insert(std::pair<std::string, std::string>("game", "game.ogg"));
	musicNames.insert(std::pair<std::string, std::string>("fight", "fight.ogg"));
	musicNames.insert(std::pair<std::string, std::string>("victory", "victory.ogg"));
	musicNames.insert(std::pair<std::string, std::string>("defeat", "defeat.ogg"));

	// load single textures
	for (std::map<std::string, std::string>::iterator it = textureNames.begin(); it != textureNames.end(); ++it)
	{
		if (!loadTexture(it->first, it->second))
		{
			return false;
		}
	}

	loadTiles();

	// load sounds
	for (std::map<std::string, std::string>::iterator it = soundNames.begin(); it != soundNames.end(); ++it)
	{
		if (!loadSound(it->first, it->second))
		{
			return false;
		}
	}

	// load music file paths
	for (std::map<std::string, std::string>::iterator it = musicNames.begin(); it != musicNames.end(); ++it)
	{
		if (!loadMusicPath(it->first, it->second))
		{
			return false;
		}
	}

	LOG(INFO) << "Resources loaded successfully";

	return true;
}

void ResourceManager::loadTiles()
{
	sf::Texture& tileset = textures.at("tileset");
	textures["tileset"] = tileset;
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
	tiles.insert(spritePair("greyGround", greyGrounds));

	std::list<sf::Sprite> darkGreyGrounds;
	darkGreyGrounds.push_back(sf::Sprite(tileset, sf::IntRect(3 * ts, 3 * ts, ts, ts)));
	darkGreyGrounds.push_back(sf::Sprite(tileset, sf::IntRect(3 * ts, 4 * ts, ts, ts)));
	darkGreyGrounds.push_back(sf::Sprite(tileset, sf::IntRect(3 * ts, 5 * ts, ts, ts)));
	darkGreyGrounds.push_back(sf::Sprite(tileset, sf::IntRect(3 * ts, 6 * ts, ts, ts)));
	darkGreyGrounds.push_back(sf::Sprite(tileset, sf::IntRect(4 * ts, 3 * ts, ts, ts)));
	darkGreyGrounds.push_back(sf::Sprite(tileset, sf::IntRect(4 * ts, 4 * ts, ts, ts)));
	darkGreyGrounds.push_back(sf::Sprite(tileset, sf::IntRect(4 * ts, 5 * ts, ts, ts)));
	darkGreyGrounds.push_back(sf::Sprite(tileset, sf::IntRect(4 * ts, 6 * ts, ts, ts)));
	tiles.insert(spritePair("darkGreyGround", darkGreyGrounds));

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
	tiles.insert(spritePair("yellowOneGround", yellowOneGrounds));

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
	tiles.insert(spritePair("yellowTwoGround", yellowTwoGrounds));

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
	tiles.insert(spritePair("greenGround", greenGrounds));

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
	tiles.insert(spritePair("brownGround", brownGrounds));

	std::list<sf::Sprite> doorGroundOne;
	doorGroundOne.push_back(sf::Sprite(tileset, sf::IntRect(5 * ts, 3 * ts, ts, ts)));
	tiles.insert(spritePair("doorGroundOne", doorGroundOne));

	std::list<sf::Sprite> doorGroundTwo;
	sf::Sprite doorGroundFlipped = sf::Sprite(tileset, sf::IntRect(5 * ts, 3 * ts, ts, ts));
	doorGroundTwo.push_back(doorGroundFlipped);
	tiles.insert(spritePair("doorGroundTwo", doorGroundTwo));
}

bool ResourceManager::loadSound(const std::string soundName, const std::string filePath)
{
	sf::SoundBuffer buffer;
	sf::Sound sound;

	if (sounds.find(soundName) != sounds.end()) // sound has already been loaded
	{
		return true;
	}

	if (!buffer.loadFromFile(settings->SOUNDS_PATH + filePath))
	{
		LOG(ERROR) << "Failed to load sound " << settings->SOUNDS_PATH + filePath;
		return false;
	}

	soundBuffers.insert(std::pair<const std::string, sf::SoundBuffer>(soundName, buffer));

	sound.setBuffer(soundBuffers.at(soundName));

	sounds.insert(std::pair<const std::string, sf::Sound>(soundName, sound));

	LOG(DEBUG) << "loaded " << settings->SOUNDS_PATH + filePath;

	return true;
}

bool ResourceManager::loadMusicPath(const std::string musicName, const std::string filePath)
{
	FILE *stream;
	errno_t err;
	std::string fullPath = settings->MUSIC_PATH + filePath;

	err = fopen_s(&stream, fullPath.c_str(), "r");

	if (err != 0)
	{
		LOG(ERROR) << "Failed to open music file " << fullPath;
		return false;
	}
	
	if (stream)
	{
		err = fclose(stream);

		if (err != 0)
		{
			LOG(ERROR) << "Failed to close music file " << fullPath;
			return false;
		}
	}

	std::shared_ptr<MusicInformation> musicInfo(new MusicInformation(fullPath));

	musicPieces.insert(std::pair<const std::string, std::shared_ptr<MusicInformation>>(musicName, musicInfo));

	LOG(DEBUG) << "Succeeded in opening music file " << fullPath;

	return true;
}

sf::Sprite& ResourceManager::getRandomTile(const std::string& tileName)
{
	return *selectRandomly(tiles[tileName].begin(), tiles[tileName].end());
}

void ResourceManager::setSoundVolumes(const float effectsVolume)
{
	typedef std::map <const std::string, sf::Sound>::iterator soundIt;

	for (soundIt it = sounds.begin(); it != sounds.end(); ++it)
	{
		it->second.setVolume(effectsVolume);
	}
}