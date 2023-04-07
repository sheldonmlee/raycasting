#include "view.h"
#include <SFML/Graphics.hpp>
#include <stdio.h>

#include "level.h"
#include "camera.h"
#include "maths.h"

static int handleKeyCode(sf::Keyboard::Key key);

static sf::Uint32 style = sf::Style::Titlebar;
static sf::RenderWindow window(sf::VideoMode(500, 500), "Raycasting", style);
static sf::Clock timer;

static Camera camera = { sf::Vector2f(300.f, 250.f), 0.f, 100, 2.0f*PI };

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
		switch (event.type) {
			case sf::Event::Closed:
				return 0;
			case sf::Event::KeyPressed:
				if (handleKeyCode(event.key.code)) continue;
				return 0;
			default:
				continue;
		}
	}

	window.clear();
	
	sf::Time t = timer.restart();
	if (!level_update()) return 0;
	camera_update(&camera, &window, t.asSeconds());
	window.display();

	return 1;
}

void view_end()
{
	printf("view_end()\n");
	level_end();

	window.close();
}

static int handleKeyCode(sf::Keyboard::Key key)
{
	switch (key) {
		case sf::Keyboard::Escape:
		case sf::Keyboard::Q:
			return 0;
		default:
			return 1;
	}
}
