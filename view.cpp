#include "view.h"
#include <SFML/Graphics.hpp>
#include <stdio.h>

#include "level.h"

static sf::Uint32 style = sf::Style::Titlebar;
static sf::RenderWindow window(sf::VideoMode(500, 500), "Raycasting", style);

int view_init()
{
	printf("view_init()\n");
	level_init(&window);
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
	if (!level_update()) return 0;
	window.display();

	return 1;
}

void view_end()
{
	printf("view_end()\n");
	level_end();
	return;
}
