#ifndef LEVEL_H
#define LEVEL_H

#include <SFML/Graphics.hpp>

enum level_tileSide
{
	NORTH,
	EAST,
	SOUTH,
	WEST
};

typedef struct
{
	unsigned int value;
	level_tileSide side;
} TileData;

int level_init();
void level_end();
float level_rayCast(sf::Vector2f point, float direction, TileData* tileData);
void level_getDimensions(unsigned int* width, unsigned int* height);
unsigned int level_getGridValue(unsigned int x, unsigned int y);

#endif
