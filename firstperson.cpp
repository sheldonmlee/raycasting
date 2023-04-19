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

void firstperson_update(sf::RenderTarget* renderTarget)
{
	renderTexture.clear();
	sf::CircleShape shape(50);
	renderTexture.draw(shape);
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
