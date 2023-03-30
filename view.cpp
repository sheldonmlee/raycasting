#include <SFML/Graphics.hpp>
#include "view.h"

static sf::Uint32 style = sf::Style::Titlebar;
static sf::RenderWindow window(sf::VideoMode(1600, 900), "SFML works!", style);
static sf::CircleShape shape(450.f);

int view_init()
{
	shape.setFillColor(sf::Color::Green);
	return 1;
}

int view_update()
{
    if (!window.isOpen()) return 0;

	sf::Event event;
	while (window.pollEvent(event)) {
		if (event.type == sf::Event::Closed) {
			window.close();
			return 0;
		}
	}

	window.clear();
	window.draw(shape);
	window.display();

	return 1;
}

