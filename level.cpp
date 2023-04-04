#include "level.h"	
#include <stdio.h>

#include "maths.h"

#define WIDTH  5
#define HEIGHT 5

static void drawGrid();
static void drawGridLine(unsigned int step, bool isHorizontal);
static sf::Vertex getGridLineVertex(unsigned int n, unsigned int maxDimension, bool isStart, bool isHorizontal);

static void castRay(sf::Vector2f point, float direction);
static void getGridIndex(sf::Vector2f point, int* x, int* y);

static sf::RenderWindow* window = nullptr;

static unsigned int level[WIDTH * HEIGHT] = {
	1, 1, 1, 1, 1,
	1, 0, 0, 0, 0,
	1, 0, 1, 0, 1,
	1, 0, 0, 0, 0,
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

float level_rayCastDistance(sf::Vector2f point, float direction)
{
	castRay(point, direction);
	return 1000.f;
}

static void drawGrid()
{
	const sf::Vector2u windowSize = window->getSize();
	const unsigned int stepX = windowSize.x/WIDTH;
	const unsigned int stepY = windowSize.y/HEIGHT;

	for (unsigned int x = 0; x < WIDTH; x++) {
		for (unsigned int y = 0; y < HEIGHT; y++) {
			if (!level[y * HEIGHT + x]) continue;

			sf::RectangleShape rectangle(sf::Vector2f(stepY, stepY));
			rectangle.setPosition(x * stepX, y * stepY);

			window->draw(rectangle);
		}
	}

	drawGridLine(stepX, true);
	drawGridLine(stepY, false);
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

	start.color = sf::Color(100, 100, 100);
	end.color = sf::Color(100, 100, 100);
	return isStart? start : end;
}

static void castRay(sf::Vector2f point, float direction)
{
	const sf::Vector2u windowSize = window->getSize();
	const unsigned int tileWidth = windowSize.x/WIDTH;
	const unsigned int tileHeight = windowSize.y/HEIGHT;

	// dx and dy are the delta x and delta y of closest grid intersection
	int indexX, indexY;
	float dx, dy;

	getGridIndex(point, &indexX, &indexY);

	direction = maths_modulo(direction, 2.0f*PI);
	bool goingDown = direction < PI;
	int signDown = goingDown? 1 : -1;

	dy = (float)((indexY + goingDown) * tileHeight) - point.y;
	dx = dy/tan(direction);

	float horizontalStepX = (float)(signDown * (tileWidth/tan(direction)));
	float horizontalStepY = (float)(signDown * (int)tileHeight);
	float horizontalProjectedX = point.x + dx;
	float horizontalProjectedY = (indexY + goingDown) * tileHeight;

	direction = maths_modulo(direction + 0.5f*PI, 2.0f*PI);
	bool goingRight = direction < PI;
	int signRight = goingRight? 1 : -1;

	dx = (float)((indexX + goingRight) * tileWidth) - point.x;
	dy = -dx/tan(direction);

	float verticalStepY = -(float)(signRight * (tileHeight/tan(direction)));
	float verticalStepX = (float)(signRight * (int)tileHeight);
	float verticalProjectedY = point.y + dy;
	float verticalProjectedX = (indexX + goingRight) * tileWidth;

	bool inLevel; 
	do {
		const float circleRadius = 3.f;
		sf::CircleShape circle(circleRadius);

		circle.setFillColor(sf::Color::Red);

		circle.setPosition(sf::Vector2f(horizontalProjectedX, horizontalProjectedY));
		circle.setOrigin(circleRadius, circleRadius);
		window->draw(circle);

		circle.setPosition(sf::Vector2f(verticalProjectedX, verticalProjectedY));
		circle.setOrigin(circleRadius, circleRadius);
		window->draw(circle);

		horizontalProjectedX += horizontalStepX;
		horizontalProjectedY += horizontalStepY;

		verticalProjectedX += verticalStepX;
		verticalProjectedY += verticalStepY;

		int indexX0, indexY0;
		int indexX1, indexY1;
		getGridIndex(sf::Vector2f(horizontalProjectedX, horizontalProjectedY), &indexX0, &indexY0);
		getGridIndex(sf::Vector2f(verticalProjectedX,   verticalProjectedY),   &indexX1, &indexY1);

		bool inLevel0 = ((indexX0 >= 0 && indexX0 < WIDTH) && (indexY0 >= 0 && indexY0 < HEIGHT));
		bool inLevel1 = ((indexX1 >= 0 && indexX1 < WIDTH) && (indexY1 >= 0 && indexY1 < HEIGHT));

		inLevel = inLevel0 || inLevel1;
	} while (inLevel);
}

static void getGridIndex(sf::Vector2f point, int* x, int* y)
{
	const sf::Vector2u windowSize = window->getSize();

	*x = point.x / (int)(windowSize.x / WIDTH);
	*y = point.y / (int)(windowSize.y / HEIGHT);

	if (*x < 0 || WIDTH <= *x) *x = -1;
	if (*y < 0 || HEIGHT <= *y) *y = -1;
}
