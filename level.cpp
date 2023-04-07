#include "level.h"	
#include <stdio.h>

#include "maths.h"

#define WIDTH  5
#define HEIGHT 5

static void drawGrid();
static void drawGridLine(unsigned int step, bool isHorizontal);
static sf::Vertex getGridLineVertex(unsigned int n, unsigned int maxDimension, bool isStart, bool isHorizontal);

static float castRay(sf::Vector2f point, float direction);
static void getGridIndex(sf::Vector2f point, int* x, int* y);

static sf::RenderWindow* window = nullptr;

static unsigned int level[WIDTH * HEIGHT] = {
	1, 1, 1, 1, 1,
	0, 0, 0, 0, 0,
	0, 0, 1, 0, 1,
	0, 0, 0, 0, 0,
	0, 0, 1, 0, 1,
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
	return castRay(point, direction);
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

static float castRay(sf::Vector2f point, float direction)
{
	const sf::Vector2u windowSize = window->getSize();
	const unsigned int tileWidth = windowSize.x/WIDTH;
	const unsigned int tileHeight = windowSize.y/HEIGHT;

	int indexX, indexY;
	getGridIndex(point, &indexX, &indexY);

	// The horizontal* and vertical* variables correspond to variables, that
	// are used to calculate the horizontal and vertical grid intersection points
	// respectively. The horizontal and vertical grid intersections are done 
	// separately.
	// 
	// The *Dy and *Dx variables are the deltas to the nearest grid boundary.
	//
	// The *StepX and *StepY variables are the regular x and y steps from the 
	// initial boundary intersection along the ray.
	//
	// The *ProjectedX and *ProjectedY variables are projected coordinates of the 
	// grid intersections along the ray.
	// 
	// The *DistCoeff variables store the coefficient of sin(direction) used to 
	// calculate distance travelled along the ray, without having to do extra
	// calls to sin(), as the direction doesn't change.
	direction = maths_modulo(direction, 2.0f*PI); // modulo to keep the angle between 0 and 2 PI radians
	bool goingDown = direction < PI;
	int signDown = goingDown? 1 : -1;

	float horizontalDy = (float)((indexY + goingDown) * tileHeight) - point.y;
	float horizontalDx = horizontalDy/tan(direction);

	float horizontalStepX = (float)(signDown * (tileWidth/tan(direction)));
	float horizontalStepY = (float)(signDown * (int)tileHeight);
	float horizontalProjectedX = point.x + horizontalDx;
	float horizontalProjectedY = (indexY + goingDown) * tileHeight;

	float horizontalDistCoeff = sin(direction);
	float horizontalRayDist = std::abs(horizontalDy/horizontalDistCoeff);

	direction = maths_modulo(direction + 0.5f*PI, 2.0f*PI); // rotate angle by 90 degrees for ease of calaculation
	bool goingRight = direction < PI;
	int signRight = goingRight? 1 : -1;

	float verticalDx = (float)((indexX + goingRight) * tileWidth) - point.x;
	float verticalDy = -verticalDx/tan(direction); // y axis needs to be flipped 

	float verticalStepY = -(float)(signRight * (tileHeight/tan(direction))); // y axis also flipped here
	float verticalStepX = (float)(signRight * (int)tileHeight);
	float verticalProjectedY = point.y + verticalDy;
	float verticalProjectedX = (indexX + goingRight) * tileWidth;

	float verticalDistCoeff = sin(direction);
	float verticalRayDist = std::abs(verticalDx/verticalDistCoeff);

	while (true) {
		int indexX0, indexY0; // store grid indices for horizontal intersections
		int indexX1, indexY1; // store grid indices for vertical intersections
		getGridIndex(sf::Vector2f(horizontalProjectedX, horizontalProjectedY), &indexX0, &indexY0);
		getGridIndex(sf::Vector2f(verticalProjectedX,   verticalProjectedY),   &indexX1, &indexY1);

		// If the ray going up or to left, the intersection points will give an index 
		// of the cells below or to the right of the cell boundaries. For those cases, 
		// the appropriate indices will be reduced by one.
		indexY0 -= !goingDown;
		indexX1 -= !goingRight;

		bool inLevel0 = indexX0 != -1 && indexY0 != -1;
		bool inLevel1 = indexX1 != -1 && indexY1 != -1;

		if (!(inLevel0 || inLevel1)) break; 

		if (horizontalRayDist < verticalRayDist) {
			if (level[indexY0 * WIDTH + indexX0]) return horizontalRayDist;

			horizontalProjectedX += horizontalStepX;
			horizontalProjectedY += horizontalStepY;

			horizontalRayDist += std::abs(horizontalStepY/horizontalDistCoeff);
		}
		else {
			if (level[indexY1 * WIDTH + indexX1]) return verticalRayDist;

			verticalProjectedX += verticalStepX;
			verticalProjectedY += verticalStepY;

			verticalRayDist += std::abs(verticalStepX/verticalDistCoeff);
		}
	};

	return 1000.f;
}

static void getGridIndex(sf::Vector2f point, int* x, int* y)
{
	const sf::Vector2u windowSize = window->getSize();

	*x = point.x / (int)(windowSize.x / WIDTH);
	*y = point.y / (int)(windowSize.y / HEIGHT);

	if (*x < 0 || WIDTH <= *x) *x = -1;
	if (*y < 0 || HEIGHT <= *y) *y = -1;
}
