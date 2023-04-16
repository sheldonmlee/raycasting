#ifndef LEVEL_H
#define LEVEL_H

#include <SFML/Graphics.hpp>

int level_init();
void level_update(sf::RenderTarget* renderTarget, unsigned int drawSize);
void level_end();
float level_rayCastDistance(sf::Vector2f point, float direction);
void level_getDimensions(unsigned int* width, unsigned int* height);

#endif
