#include "level.h"	
#include <stdio.h>

#include "maths.h"

#define WIDTH  5
#define HEIGHT 5

static void drawGrid(sf::RenderTarget* renderTarget, unsigned int tileSize);
static void drawGridLine(sf::RenderTarget* renderTarget, float step, bool isHorizontal);
static sf::Vertex getGridLineVertex(float n, float maxDimension, bool isStart, bool isHorizontal);

static float castRay(sf::Vector2f point, float direction);
static void getGridIndex(sf::Vector2f point, int* x, int* y);

static unsigned int level[WIDTH * HEIGHT] = {
	0, 0, 1, 1, 1,
	0, 0, 0, 0, 0,
	1, 0, 1, 0, 1,
	1, 0, 0, 0, 1,
	1, 0, 1, 1, 1,
};

int level_init()
{
	printf("level_init()\n");
	return 1;
}

void level_update(sf::RenderTarget* renderTarget, unsigned int drawSize)
{
	if (!renderTarget) return;

	drawGrid(renderTarget, drawSize/WIDTH);
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

static void drawGrid(sf::RenderTarget* renderTarget, unsigned int tileSize)
{
	for (unsigned int x = 0; x < WIDTH; x++) {
		for (unsigned int y = 0; y < HEIGHT; y++) {
			if (!level[y * HEIGHT + x]) continue;

			sf::RectangleShape rectangle(sf::Vector2f(tileSize, tileSize));
			rectangle.setPosition((float)x * tileSize, (float)y * tileSize);
			renderTarget->draw(rectangle);
		}
	}

	drawGridLine(renderTarget, tileSize, true);
	drawGridLine(renderTarget, tileSize, false);
}

static void drawGridLine(sf::RenderTarget* renderTarget, float step, bool isHorizontal)
{
	unsigned int lines = isHorizontal? WIDTH : HEIGHT;

	for (unsigned int n = 0; n < lines; n++) {
		if (n == 0) continue;
		float offset = (float)n * step;
		float maxDimension = (float)lines * step;
		sf::Vertex line[] = 
		{
			getGridLineVertex(offset, maxDimension, true, isHorizontal),
			getGridLineVertex(offset, maxDimension, false, isHorizontal)
		};

		renderTarget->draw(line, 2, sf::Lines);
	}
}

static sf::Vertex getGridLineVertex(float offset, float maxDimension, bool isStart, bool isHorizontal)
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

	sf::Color color(100, 100, 100);
	start.color = color;
	end.color   = color;
	return isStart? start : end;
}

static float castRay(sf::Vector2f point, float direction)
{
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

	float horizontalDy = (float)(indexY + goingDown) - point.y;
	float horizontalDx = horizontalDy/tan(direction);

	float horizontalStepX = ((float)signDown * (1.f/tan(direction)));
	float horizontalStepY = (float)signDown;
	float horizontalProjectedX = point.x + horizontalDx;
	float horizontalProjectedY = indexY + goingDown;

	float horizontalDistCoeff = sin(direction);
	float horizontalRayDist = std::abs(horizontalDy/horizontalDistCoeff);

	direction = maths_modulo(direction + 0.5f*PI, 2.0f*PI); // rotate angle by 90 degrees for ease of calaculation
	bool goingRight = direction < PI;
	int signRight = goingRight? 1 : -1;

	float verticalDx = (float)(indexX + goingRight) - point.x;
	float verticalDy = -verticalDx/tan(direction); // y axis needs to be flipped 

	float verticalStepY = -((float)signRight * (1.f/tan(direction))); // y axis also flipped here
	float verticalStepX = (float)signRight;
	float verticalProjectedY = point.y + verticalDy;
	float verticalProjectedX = indexX + goingRight;

	float verticalDistCoeff = sin(direction);
	float verticalRayDist = std::abs(verticalDx/verticalDistCoeff);

	unsigned int tries = WIDTH * HEIGHT;
	while (tries--) {
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
	*x = point.x;
	*y = point.y;

	if (*x < 0 || WIDTH <= *x) *x = -1;
	if (*y < 0 || HEIGHT <= *y) *y = -1;
}
