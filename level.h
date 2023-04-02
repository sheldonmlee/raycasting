#ifndef LEVEL_H
#define LEVEL_H

#include <SFML/Graphics.hpp>

int level_init(sf::RenderWindow* renderWindow);
int level_update();
void level_end();
float level_rayCastDistance(sf::Vector2f point, float direction);

#endif
