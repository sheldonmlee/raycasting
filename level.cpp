#include "level.h"
#include <stdio.h>

#define WIDTH  5
#define HEIGHT 5

static sf::RenderWindow* window = nullptr;

static void drawGrid();

static unsigned int level[WIDTH * HEIGHT] = {
	1, 1, 1, 1, 1,
	0, 0, 0, 0, 0,
	1, 0, 1, 0, 1,
	0, 0, 0, 0, 0,
	1, 0, 1, 0, 1,
};


int level_init(sf::RenderWindow* renderWindow)
{
	printf("level_init()\n");
	window = renderWindow;
	return 1;
}

int level_update()
{
	if (!window) return 0;
	drawGrid();
	return 1;
}

void level_end()
{
	printf("level_end()\n");
	return;
}

static void drawGrid()
{
	const unsigned int padding = 5;
	const sf::Vector2u windowSize = window->getSize();
	const unsigned int stepX = windowSize.x/WIDTH;
	const unsigned int stepY = windowSize.y/HEIGHT;

	for (unsigned int x = 0; x < WIDTH; x++) {
		if (x == 0) continue;
		sf::Vertex line[] = 
		{
			sf::Vertex(sf::Vector2f(x * stepX, 0)),
			sf::Vertex(sf::Vector2f(x * stepX, windowSize.x))
		};

		window->draw(line, 2, sf::Lines);
	}

	for (unsigned int y = 0; y < HEIGHT; y++) {
		if (y == 0) continue;
		sf::Vertex line[] = 
		{
			sf::Vertex(sf::Vector2f(0,            y * stepY)),
			sf::Vertex(sf::Vector2f(windowSize.y, y * stepY))
		};

		window->draw(line, 2, sf::Lines);
	}

	for (unsigned int x = 0; x < WIDTH; x++) {
		for (unsigned int y = 0; y < HEIGHT; y++) {
			if (!level[y * HEIGHT + x]) continue;

			sf::RectangleShape rectangle(sf::Vector2f(stepY - padding*2, stepY - padding*2));
			rectangle.setPosition(x * stepX + padding, y * stepY + padding);

			window->draw(rectangle);
		}
	}
}
