#include "level.h"
#include <stdio.h>

#define WIDTH  5
#define HEIGHT 5

static void drawGrid();
static void drawGridLine(unsigned int step, bool isHorizontal);
static sf::Vertex getGridLineVertex(unsigned int n, unsigned int maxDimension, bool isStart, bool isHorizontal);

static sf::RenderWindow* window = nullptr;

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
	const sf::Vector2u windowSize = window->getSize();
	unsigned int padding = 5;
	unsigned int stepX = windowSize.x/WIDTH;
	unsigned int stepY = windowSize.y/HEIGHT;

	drawGridLine(stepX, true);
	drawGridLine(stepY, false);

	for (unsigned int x = 0; x < WIDTH; x++) {
		for (unsigned int y = 0; y < HEIGHT; y++) {
			if (!level[y * HEIGHT + x]) continue;

			sf::RectangleShape rectangle(sf::Vector2f(stepY - padding*2, stepY - padding*2));
			rectangle.setPosition(x * stepX + padding, y * stepY + padding);

			window->draw(rectangle);
		}
	}
}

static void drawGridLine(unsigned int step, bool isHorizontal)
{
	unsigned int lines = isHorizontal? WIDTH : HEIGHT;

	for (unsigned int n = 0; n < lines; n++) {
		if (n == 0) continue;
		unsigned int offset = n * step;
		unsigned int maxDimension = lines * step;
		sf::Vertex line[] = 
		{
			getGridLineVertex(offset, maxDimension, true, isHorizontal),
			getGridLineVertex(offset, maxDimension, false, isHorizontal)
		};

		window->draw(line, 2, sf::Lines);
	}
}

static sf::Vertex getGridLineVertex(unsigned int offset, unsigned int maxDimension, bool isStart, bool isHorizontal)
{
	sf::Vertex start; 
	sf::Vertex end; 

	if (isHorizontal) {
		start = sf::Vertex(sf::Vector2f(offset, 0));
		end   = sf::Vertex(sf::Vector2f(offset, maxDimension));
	}
	else {
		start = sf::Vertex(sf::Vector2f(0,            offset));
		end   = sf::Vertex(sf::Vector2f(maxDimension, offset));
	}

	return isStart? start : end;
}
