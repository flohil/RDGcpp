#include "OutputFormatter.hpp"
#include <ctime>

void OutputFormatter::chat(tgui::ChatBox::Ptr chatBox, std::string str, sf::Color color)
{
	time_t now = time(0);
	std::string fulltime = ctime(&now);
	std::string time = fulltime.substr(11, 8);

	chatBox->addLine(" " + time + " - " + str, color);
}

// returns float with max 2 decimals
std::string OutputFormatter::shortFloat(float nbr)
{
	std::string rawStr = std::to_string(nbr);
	unsigned int dotPos = rawStr.find(".");

	return rawStr.substr(0, dotPos + 3);
}