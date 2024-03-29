#include "firstperson.h"
#include <stdio.h>

#include "maths.h"

static sf::RenderTexture renderTexture;
static sf::Vector2f renderTexturePosition;

static bool init = false;
static unsigned int width;
static unsigned int height;

int firstperson_init(unsigned int _width, unsigned int _height)
{
	printf("firstperson_init()\n");
	if (!renderTexture.create(_width, _height)) return 0;
	width = _width;
	height = _height;
	init = true;
	return 1;
}

void firstperson_update(sf::RenderTarget* renderTarget, Camera* camera)
{
	const float columnWidth = (float)width/(float)camera->resolution;
	float columnHeight = 0;
	renderTexture.clear();

	for (unsigned int i = 0; i < camera->resolution; i++) {
		// Get distance of ray intersection in the direction of the camera,
		// instead of the actual ray distance to avoid fish eye effect.
		float angleDiff = camera->rays[i].direction - camera->direction;
		float distance = camera->rays[i].distance * cos(angleDiff);

		if (distance > 0.f) columnHeight = 0.5f * (float)height/distance;

		float centeredHeight = (float)height/2.f - columnHeight/2.f;
		float distanceScale = distance/10.f;
		distanceScale = (distanceScale > 1.f)? 1.f : distanceScale;
		float brightness = 255.f*(1.f-distanceScale);

		switch (camera->rays[i].tileData.side) {
			case NORTH:
				break;
			case EAST:
			case WEST:
				brightness *= 0.9;
				break;
			case SOUTH:
				brightness *= 0.8;
				break;
		}

		sf::Color color(brightness, brightness, brightness);
		switch (camera->rays[i].tileData.value) {
			case 2:
				color *= sf::Color::Red; 
				break;
			case 3:
				color *= sf::Color::Green;
				break;
			case 4:
				color *= sf::Color::Blue;
				break;
		};

		sf::RectangleShape rectangle(sf::Vector2f(columnWidth, columnHeight));
		rectangle.setPosition(sf::Vector2f(i*columnWidth, centeredHeight));
		rectangle.setFillColor(color);
		renderTexture.draw(rectangle);
	}

	renderTexture.display();
	sf::Sprite sprite(renderTexture.getTexture());
	sprite.setPosition(renderTexturePosition);
	renderTarget->draw(sprite);

}

void firstperson_setTexturePosition(float x, float y)
{
	renderTexturePosition.x = x;
	renderTexturePosition.y = y;
}
