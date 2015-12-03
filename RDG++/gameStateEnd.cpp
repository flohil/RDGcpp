#include <SFML/Graphics.hpp>

#include "gameStateEnd.hpp"
#include "gameState.hpp"
#include "easylogging++.hpp"

GameStateEnd::GameStateEnd(Game& game_, bool victory_) :
GameState(game_), victory(victory_)
{
	/*std::cout << "inside GameStateEnd constructor" << std::endl;*/

	settings = game_.getSettings();
	size = sf::Vector2f(static_cast<float>(settings->scaleWidth), static_cast<float>(settings->scaleHeight));
	view.setSize(size);
	view.setCenter(view.getSize() * 0.5f);

	// background.setTexture(ResourceManager::getInstance().getTexture("background"));

	// create gui 
	loadGui();
}

void GameStateEnd::draw(const float deltaTime)
{
	game.window.setView(view);

	game.window.clear(sf::Color::Black);
	//game.window.draw(background);
	game.window.draw(endCaptionText);

	for (sf::Text line : lines)
	{
		game.window.draw(line);
	}

	//gui.draw();

	return;
}

void GameStateEnd::update(const float deltaTime)
{
	totalAccumulator += deltaTime;

	if (triggeredQuit)
	{
		quitAccumulator += deltaTime;

		if (quitAccumulator > quitSpan)
		{
			game.popState();
		}
	}

	scrollUpdateAccumulator += deltaTime;

	while (scrollUpdateAccumulator > scrollUpdateSpan)
	{

		sf::Vector2f center = view.getCenter();

		float currentPos = lines.at(lines.size() - 1).getGlobalBounds().top - center.y + settings->height * 0.5f;

		/*std::cout << "currentPos: " << currentPos << std::endl;
		std::cout << "targetEndPosY: " << targetEndPosY << std::endl;*/

		if (!(currentPos < targetEndPosY))
		{
			/*std::cout << "center.y: " << center.y << std::endl;*/
			++scrollCtr;
			center.y += scrollSpeed;
			view.setCenter(center);
		}
		else
		{
			if (!endedScrolling)
			{
				endedScrolling = true;
				std::cout << "center.y: " << center.y << std::endl;
				std::cout << "scrollCtr: " << scrollCtr << std::endl;
			}
		}

		scrollUpdateAccumulator -= deltaTime;
	}

	if (totalAccumulator > totalDuration && !triggeredQuit)
	{
		game.music.setLoop(true);
		game.changeMusic("mainMenu", 1.0f, 0.f, 0.1f);
		triggeredQuit = true;
	}

	return;
}

void GameStateEnd::handleInput()
{
	sf::Event event;

	while (game.window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			game.window.close();
		}
		else if (event.type == sf::Event::KeyPressed || event.type == sf::Event::MouseButtonPressed)
		{
			if (!triggeredQuit)
			{
				game.music.setLoop(true);
				game.changeMusic("mainMenu", 1.0f, 0.f, 0.1f);
				triggeredQuit = true;
			}
		}
	}

	return;
}

void GameStateEnd::loadGui()
{
	view.setSize(static_cast<float>(settings->width), static_cast<float>(settings->height));
	view.setCenter(static_cast<float>(settings->width) * 0.5f, static_cast<float>(settings->height) * 0.5f);

	gui.removeAllWidgets();
	gui.setWindow(game.window);

	// set global font that all widgets can use by default
	gui.setFont("res/fonts/DejaVuSans.ttf");

	std::string endCaption;
	std::vector<std::string> linesTexts;
	totalDuration = 0.f;

	if (victory)
	{
		totalDuration = victoryDuration;
		endCaption = "Victory!";

		linesTexts.push_back("Congratulations, you rescued the world from .... uhhh ....");
		linesTexts.push_back("Well truth is, we don't really know what you rescued the world from.");
		linesTexts.push_back("Maybe all those creatures you killed weren't so bad after all");
		linesTexts.push_back("and you were just being a little paranoid...");
		linesTexts.push_back("Maybe you're even worse than them.");
		linesTexts.push_back("");
		linesTexts.push_back("");
		linesTexts.push_back("Anyways, people joyfully celebrate your victory!");
		linesTexts.push_back("Parents name their children after you,");
		linesTexts.push_back("boys want to become just as brave as you,");
		linesTexts.push_back("girls dream of being rescued by you,");
		linesTexts.push_back("men make you the topic of pub songs");
		linesTexts.push_back("and women shower you with bras.");
		linesTexts.push_back("");
		linesTexts.push_back("");
		linesTexts.push_back("All of them join together to praise you, cheering");
		linesTexts.push_back("'... and he shall live forever and ever ...");
		linesTexts.push_back("King of Kings and Lord of Lords!'");
	}
	else
	{
		totalDuration = defeatDuration;
		endCaption = "Defeat!";

		linesTexts.push_back("Shame on you, you failed to rescue the world from .... uhhh ....");
		linesTexts.push_back("Well truth is, we don't really know what you could have saved the world from.");
		linesTexts.push_back("And there is no way to find out because you suck!");
		linesTexts.push_back("You should have prepared better, young padawan.");
		linesTexts.push_back("");
		linesTexts.push_back("");
		linesTexts.push_back("As a punishment for your failure,");
		linesTexts.push_back("the four horsemen of the apocalypse have taken you to hell");
		linesTexts.push_back("where you will spend all eternity in miserable pain and torture.");
		linesTexts.push_back("");
		linesTexts.push_back("");
		linesTexts.push_back("Your misery will include endless iterations of tea parties with your stepmother,");
		linesTexts.push_back("hillwalking with Hansi Hinterseer");
		linesTexts.push_back("and Bingo evenings hosted by Peter Rapp.");
		linesTexts.push_back("For lunch you can choose between creamed spinach, soy products and Beuschel.");
		linesTexts.push_back("Hell's kitchen serves Pittinger beer only, the Radler tastes even worse.");
		linesTexts.push_back("");
		linesTexts.push_back("");
		linesTexts.push_back("To begin with, you have been selected for conducting Lucifer's furuncle care.");
		linesTexts.push_back("Off you go now!");
	}

	linesTexts.push_back(""); linesTexts.push_back(""); linesTexts.push_back(""); linesTexts.push_back("");
	linesTexts.push_back(""); linesTexts.push_back(""); linesTexts.push_back(""); linesTexts.push_back("");
	linesTexts.push_back(""); linesTexts.push_back(""); linesTexts.push_back(""); linesTexts.push_back("");
	linesTexts.push_back(""); linesTexts.push_back(""); linesTexts.push_back(""); linesTexts.push_back("");
	linesTexts.push_back(""); linesTexts.push_back(""); linesTexts.push_back(""); linesTexts.push_back("");
	linesTexts.push_back(""); linesTexts.push_back(""); linesTexts.push_back(""); linesTexts.push_back("");
	linesTexts.push_back(""); linesTexts.push_back(""); linesTexts.push_back(""); linesTexts.push_back("");
	linesTexts.push_back(""); linesTexts.push_back(""); linesTexts.push_back(""); linesTexts.push_back("");
	linesTexts.push_back("Room Duelling Game++");
	linesTexts.push_back("");
	linesTexts.push_back("");
	linesTexts.push_back("");
	linesTexts.push_back("");
	linesTexts.push_back("PRODUCERS");
	linesTexts.push_back("");
	linesTexts.push_back("Alexander Benesch");
	linesTexts.push_back("Florian Hilbinger");
	linesTexts.push_back("");
	linesTexts.push_back("");
	linesTexts.push_back("");
	linesTexts.push_back("CONCEPT");
	linesTexts.push_back("");
	linesTexts.push_back("Alexander Benesch");
	linesTexts.push_back("Florian Hilbinger");
	linesTexts.push_back("");
	linesTexts.push_back("");
	linesTexts.push_back("");
	linesTexts.push_back("LEAD DEVELOPERS");
	linesTexts.push_back("");
	linesTexts.push_back("Alexander Benesch");
	linesTexts.push_back("Florian Hilbinger");
	linesTexts.push_back("");
	linesTexts.push_back("");
	linesTexts.push_back("");
	linesTexts.push_back("SENIOR DEVELOPERS");
	linesTexts.push_back("");
	linesTexts.push_back("Alexander Benesch");
	linesTexts.push_back("Florian Hilbinger");
	linesTexts.push_back("");
	linesTexts.push_back("");
	linesTexts.push_back("");
	linesTexts.push_back("JUNIOR DEVELOPERS");
	linesTexts.push_back("");
	linesTexts.push_back("Alexander Benesch");
	linesTexts.push_back("Florian Hilbinger");
	linesTexts.push_back("");
	linesTexts.push_back("");
	linesTexts.push_back("");
	linesTexts.push_back("ART DIRECTOR");
	linesTexts.push_back("");
	linesTexts.push_back("Alexander Benesch");
	linesTexts.push_back("");
	linesTexts.push_back("");
	linesTexts.push_back("");
	linesTexts.push_back("GRAPHICS");
	linesTexts.push_back("");
	linesTexts.push_back("Alexander Benesch");
	linesTexts.push_back("Florian Hilbinger");
	linesTexts.push_back("");
	linesTexts.push_back("");
	linesTexts.push_back("");
	linesTexts.push_back("SOUND DIRECTOR");
	linesTexts.push_back("");
	linesTexts.push_back("Florian Hilbinger");
	linesTexts.push_back("");
	linesTexts.push_back("");
	linesTexts.push_back("");
	linesTexts.push_back("SOUND EFFECTS");
	linesTexts.push_back("");
	linesTexts.push_back("Alexander Benesch");
	linesTexts.push_back("Florian Hilbinger");
	linesTexts.push_back("");
	linesTexts.push_back("");
	linesTexts.push_back("");
	linesTexts.push_back("ARTIFICIAL INTELLIGENCE");
	linesTexts.push_back("");
	linesTexts.push_back("Alexander Benesch");
	linesTexts.push_back("Florian Hilbinger");
	linesTexts.push_back("");
	linesTexts.push_back("");
	linesTexts.push_back("");
	linesTexts.push_back("LEVEL DESIGN");
	linesTexts.push_back("");
	linesTexts.push_back("Alexander Benesch");
	linesTexts.push_back("Florian Hilbinger");
	linesTexts.push_back("");
	linesTexts.push_back("");
	linesTexts.push_back("");
	linesTexts.push_back("GUI DESIGN");
	linesTexts.push_back("");
	linesTexts.push_back("Alexander Benesch");
	linesTexts.push_back("Florian Hilbinger");
	linesTexts.push_back("");
	linesTexts.push_back("");
	linesTexts.push_back("");
	linesTexts.push_back("QUALITY ASSURANCE SUPERVISION");
	linesTexts.push_back("");
	linesTexts.push_back("Alexander Benesch");
	linesTexts.push_back("Florian Hilbinger");
	linesTexts.push_back("");
	linesTexts.push_back("");
	linesTexts.push_back("");
	linesTexts.push_back("QUALITY ASSURANCE TESTERS");
	linesTexts.push_back("");
	linesTexts.push_back("Alexander Benesch");
	linesTexts.push_back("Florian Hilbinger");
	linesTexts.push_back("");
	linesTexts.push_back("");
	linesTexts.push_back("");
	linesTexts.push_back("CREDITS");
	linesTexts.push_back("");
	linesTexts.push_back("Florian Hilbinger");
	linesTexts.push_back(""); linesTexts.push_back(""); linesTexts.push_back(""); linesTexts.push_back("");
	linesTexts.push_back(""); linesTexts.push_back(""); linesTexts.push_back(""); linesTexts.push_back("");
	linesTexts.push_back(""); linesTexts.push_back(""); linesTexts.push_back(""); linesTexts.push_back("");
	linesTexts.push_back(""); linesTexts.push_back(""); linesTexts.push_back(""); linesTexts.push_back("");
	linesTexts.push_back(""); linesTexts.push_back(""); linesTexts.push_back(""); linesTexts.push_back("");
	linesTexts.push_back(""); linesTexts.push_back(""); linesTexts.push_back(""); linesTexts.push_back("");
	linesTexts.push_back(""); linesTexts.push_back(""); linesTexts.push_back(""); linesTexts.push_back("");
	linesTexts.push_back(""); linesTexts.push_back(""); linesTexts.push_back(""); linesTexts.push_back("");
	linesTexts.push_back("SPECIAL THANKS TO");
	linesTexts.push_back("");
	linesTexts.push_back("Alexander Benesch");
	linesTexts.push_back("Florian Hilbinger");
	linesTexts.push_back("");
	linesTexts.push_back("");
	linesTexts.push_back("");
	linesTexts.push_back("");
	linesTexts.push_back("");
	linesTexts.push_back("");
	linesTexts.push_back("NO ANIMALS WERE HARMED IN THE MAKING OF THIS GAME");
	linesTexts.push_back("(except for the cat I knocked over while having a skype call with Alex)");
	linesTexts.push_back(""); linesTexts.push_back(""); linesTexts.push_back(""); linesTexts.push_back("");
	linesTexts.push_back(""); linesTexts.push_back(""); linesTexts.push_back(""); linesTexts.push_back("");
	linesTexts.push_back(""); linesTexts.push_back(""); linesTexts.push_back(""); linesTexts.push_back("");
	linesTexts.push_back(""); linesTexts.push_back(""); linesTexts.push_back(""); linesTexts.push_back("");
	linesTexts.push_back(""); linesTexts.push_back(""); linesTexts.push_back(""); linesTexts.push_back("");
	linesTexts.push_back(""); linesTexts.push_back(""); linesTexts.push_back(""); linesTexts.push_back("");
	linesTexts.push_back(""); linesTexts.push_back(""); linesTexts.push_back(""); linesTexts.push_back("");
	linesTexts.push_back(""); linesTexts.push_back(""); linesTexts.push_back(""); linesTexts.push_back("");
	linesTexts.push_back("Room Duelling Game++");
	linesTexts.push_back("");
	linesTexts.push_back("");
	linesTexts.push_back("Alexander Benesch & Florian Hilbinger");
	linesTexts.push_back("GADEL, 2015");

	float offsetY = settings->height * 1.f;
	float centerX = settings->width * 0.5f;
	float centerY = settings->height * 0.5f;
	float captionSize = 50.f;
	float textSize = 25.f;
	float lineSpace = 10.f * settings->heightScaleFactor;

	font.loadFromFile("res/fonts/DejaVuSans.ttf");

	endCaptionText.setString(endCaption);
	endCaptionText.setCharacterSize(static_cast<unsigned int>(captionSize * settings->heightScaleFactor)); //50.f * settings->heightScaleFactor
	endCaptionText.setFont(font);
	endCaptionText.setColor(sf::Color::White);

	sf::FloatRect rect = endCaptionText.getGlobalBounds();
	endCaptionText.setPosition(centerX - rect.width * 0.5f, offsetY);

	offsetY += rect.height;
	offsetY += lineSpace;
	offsetY += lineSpace;
	offsetY += lineSpace;
	offsetY += lineSpace;

	std::cout << "text: " << endCaptionText.getString().toAnsiString() << std::endl;

	for (std::string lineText : linesTexts)
	{
		sf::Text line;
		line.setString(lineText);
		line.setCharacterSize(static_cast<unsigned int>(textSize * settings->heightScaleFactor)); //50.f * settings->heightScaleFactor
		line.setFont(font);
		line.setColor(sf::Color::White);

		sf::FloatRect rect = line.getGlobalBounds();
		line.setPosition(centerX - rect.width * 0.5f, offsetY);

		offsetY += rect.height;
		offsetY += lineSpace;

		lines.push_back(line);
	}

	endPosY = lines.at(lines.size() - 1).getGlobalBounds().top;

	targetEndPosY = centerY + lines.at(lines.size() - 1).getGlobalBounds().height * 0.5f + 2*lineSpace;

	float nbrScrolls = (totalDuration - staticEndScreenDuration) / scrollUpdateSpan;
	float scrollSum = endPosY - targetEndPosY;
	
	std::cout << "nbrScrolls: " << nbrScrolls << std::endl;
	std::cout << "scrollSum: " << scrollSum << std::endl;

	scrollSpeed = scrollSum / nbrScrolls * 0.33f;

	std::cout << "scrollSpeed: " << scrollSpeed << std::endl;
}
