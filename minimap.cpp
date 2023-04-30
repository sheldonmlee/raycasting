#include "minimap.h"
#include <stdio.h>

#include "maths.h"
#include "level.h"
// Camera drawing
static void drawCamera(sf::RenderTarget* renderTarget, Camera* camera);
static void drawRays(sf::RenderTarget* renderTarget, Camera* camera);
static void drawLine(sf::RenderTarget* renderTarget, sf::Vector2f pos, float angle, float length, sf::Color color);
// Grid drawing
static void drawGrid(sf::RenderTarget* renderTarget, unsigned int tileSize);
static void drawGridLine(sf::RenderTarget* renderTarget, float step, bool isHorizontal);
static sf::Vertex getGridLineVertex(float n, float maxDimension, bool isStart, bool isHorizontal);

static sf::RenderTexture renderTexture;
static sf::Vector2f renderTexturePosition;

static bool init = false;
static unsigned int minimapSize;
static unsigned int gridWidth, gridHeight;
static float drawScale;

int minimap_init(unsigned int size)
{
	printf("minimap_init()\n");
	if (!renderTexture.create(size, size)) return 0;

	minimapSize = size;
	level_getDimensions(&gridWidth, &gridHeight);
	drawScale = (float)size/(float)gridWidth;

	init = true;
	return 1;
}

void minimap_update(sf::RenderTarget* renderTarget, Camera* camera)
{
	if (!init) return;
	if (!renderTarget || !camera) return;

	renderTexture.clear();
	drawGrid(&renderTexture, minimapSize/gridWidth);
	drawCamera(&renderTexture, camera);
	renderTexture.display();

	sf::Sprite sprite(renderTexture.getTexture());
	renderTarget->draw(sprite);
}

void minimap_setTexturePosition(float x, float y)
{
	renderTexturePosition.x = x;
	renderTexturePosition.y = y;
}

static void drawCamera(sf::RenderTarget* renderTarget, Camera* camera)
{
	const sf::Vector2f scaledPos = camera->pos * drawScale;
	const float circleRadius = 0.02f * minimapSize;
	sf::CircleShape circle(circleRadius);
	circle.setPosition(scaledPos);
	circle.setOrigin(circleRadius, circleRadius);
	circle.setFillColor(sf::Color::Green);

	drawRays(renderTarget, camera);
	drawLine(renderTarget, scaledPos, camera->direction, minimapSize / 5.f, sf::Color::Red);
	renderTarget->draw(circle);
}

static void drawRays(sf::RenderTarget* renderTarget, Camera* camera)
{
	const sf::Vector2f scaledPos = camera->pos * drawScale;

	for (unsigned int i = 0; i < camera->resolution; i++) {
		CameraRay* ray = &camera->rays[i];
		drawLine(renderTarget, scaledPos, ray->direction, ray->distance * drawScale, sf::Color(150, 150, 100));
	}
}

static void drawLine(sf::RenderTarget* renderTarget, sf::Vector2f pos, float angle, float length, sf::Color color)
{
	if (!renderTarget) return;

	sf::Vector2f endOffset(length * cos(angle), length * sin(angle));
	sf::Vertex start(pos);
	sf::Vertex end(pos + endOffset);

	start.color = color;
	end.color = color;

	sf::Vertex line[] = { start, end };

	renderTarget->draw(line, 2, sf::Lines);
}

static void drawGrid(sf::RenderTarget* renderTarget, unsigned int tileSize)
{
	for (unsigned int x = 0; x < gridWidth; x++) {
		for (unsigned int y = 0; y < gridHeight; y++) {
			unsigned int value = level_getGridValue(x, y);
			if (!value) continue;

			sf::Color color(255, 255, 255);
			switch (value) {
				case 2:
					color = sf::Color::Red;
					break;
				case 3:
					color = sf::Color::Green;
					break;
				case 4:
					color = sf::Color::Blue;
					break;
			};

			sf::RectangleShape rectangle(sf::Vector2f(tileSize, tileSize));
			rectangle.setPosition((float)x * tileSize, (float)y * tileSize);
			rectangle.setFillColor(color);
			renderTarget->draw(rectangle);
		}
	}

	drawGridLine(renderTarget, tileSize, true);
	drawGridLine(renderTarget, tileSize, false);
}

static void drawGridLine(sf::RenderTarget* renderTarget, float step, bool isHorizontal)
{
	unsigned int lines = isHorizontal? gridWidth : gridHeight;

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
