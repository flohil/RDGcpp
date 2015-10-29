#ifndef OUTPUT_FORMATTER
#define OUTPUT_FORMATTER
#define _CRT_SECURE_NO_WARNINGS

#include <string>
#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>

class OutputFormatter
{
public:

	static void chat(tgui::ChatBox::Ptr chatBox, std::string str, sf::Color color);
};

#endif /* OUTPUT_FORMATTER */