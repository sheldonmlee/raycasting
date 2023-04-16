#include "view.h"
#include <SFML/Graphics.hpp>
#include <stdio.h>

#include "maths.h"
#include "camera.h"
#include "minimap.h"

#define MINIMAP_SIZE 250
#define HALF_MINIMAP_SIZE MINIMAP_SIZE/2.f
#define DRAW_SCALE MINIMAP_SIZE/5.f

static int handleKeyCode(sf::Keyboard::Key key);

static Camera camera = { sf::Vector2f(5.f/2.f, 5.f/2.f), 0.f, 300, 0.5f*PI };

static sf::Uint32 style = sf::Style::Titlebar;
static sf::RenderWindow window(sf::VideoMode(MINIMAP_SIZE + camera.resolution, MINIMAP_SIZE), "Raycasting", style);
static sf::Clock timer;

int view_init()
{
	printf("view_init()\n");
	minimap_init(MINIMAP_SIZE);
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

	sf::Time t = timer.restart();

	camera_update(&camera, t.asSeconds());

	window.clear();
	minimap_update(&window, &camera);
	window.display();

	return 1;
}

void view_end()
{
	printf("view_end()\n");

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
