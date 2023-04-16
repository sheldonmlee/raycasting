#include "minimap.h"
#include <stdio.h>

#include "maths.h"
#include "level.h"

static void drawCamera(sf::RenderTarget* renderTarget, Camera* camera);
static void drawRays(sf::RenderTarget* renderTarget, Camera* camera);
static void drawLine(sf::RenderTarget* renderTarget, sf::Vector2f pos, float angle, float length, sf::Color color);

static sf::RenderTexture minimap;

static bool init = false;
static unsigned int minimapSize;
static float drawScale;

int minimap_init(unsigned int size)
{
	printf("minimap_init()\n");
	if (!minimap.create(size, size)) return 0;
	level_init();

	unsigned int width, height;
	level_getDimensions(&width, &height);

	minimapSize = size;
	drawScale = (float)size/(float)width;

	init = true;
	return 1;
}

void minimap_update(sf::RenderTarget* renderTarget, Camera* camera)
{
	if (!init) return;
	if (!renderTarget || !camera) return;
	minimap.clear();
	level_update(&minimap, minimapSize);
	drawCamera(&minimap, camera);
	minimap.display();

	sf::Sprite sprite(minimap.getTexture());
	renderTarget->draw(sprite);
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

	float rayDirection = 0;
	float rayDirectionStep = camera->fov / (float)camera->resolution;
	bool isOddResolution = (camera->resolution % 2);
	float rayDirectionOffset = isOddResolution? 0 : rayDirectionStep / 2.f;

	for (unsigned int i = 0; i < camera->resolution; i++) {
		if (isOddResolution && i == 0) 
			rayDirection = camera->direction;
		else if (i % 2) 
			rayDirection = camera->direction - rayDirectionOffset;
		else 
			rayDirection = camera->direction + rayDirectionOffset;

		float distance = level_rayCastDistance(camera->pos, rayDirection) * drawScale;

		drawLine(renderTarget, scaledPos, rayDirection, distance, sf::Color(150, 150, 100));

		if ((i + isOddResolution) % 2) rayDirectionOffset += rayDirectionStep;
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

