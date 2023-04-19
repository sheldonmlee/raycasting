#include "view.h"
#include <SFML/Graphics.hpp>
#include <stdio.h>

#include "maths.h"
#include "camera.h"
#include "minimap.h"
#include "firstperson.h"

#define MINIMAP_SIZE 460
#define VIEW_SIZE MINIMAP_SIZE*2

static int handleKeyCode(sf::Keyboard::Key key);

static Camera camera;

static sf::Uint32 style = sf::Style::Titlebar;
static sf::RenderWindow window(sf::VideoMode(MINIMAP_SIZE + VIEW_SIZE, MINIMAP_SIZE), "Raycasting", style);
static sf::Clock timer;

int view_init()
{
	printf("view_init()\n");
	if (!camera_init(&camera, sf::Vector2f(5.f/2.f, 5.f/2.f), 0.f, 100, 0.5f*PI)) return 0;
	if (!minimap_init(MINIMAP_SIZE)) return 0;
	if (!firstperson_init(VIEW_SIZE, MINIMAP_SIZE)) return 0;

	minimap_setTexturePosition(0.f, 0.f);
	firstperson_setTexturePosition(MINIMAP_SIZE, 0.f);
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
	firstperson_update(&window);
	window.display();

	return 1;
}

void view_end()
{
	camera_destroy(&camera);
	window.close();
	printf("view_end()\n");
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
