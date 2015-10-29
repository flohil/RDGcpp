#include "OutputFormatter.hpp"
#include <ctime>

void OutputFormatter::chat(tgui::ChatBox::Ptr chatBox, std::string str, sf::Color color)
{
	time_t now = time(0);
	std::string fulltime = ctime(&now);
	std::string time = fulltime.substr(11, 8);

	chatBox->addLine(" " + time + " - " + str, color);
}