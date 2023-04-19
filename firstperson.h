#ifndef FIRSTPERSON_H
#define FIRSTPERSON_H

#include <SFML/Graphics.hpp>

int firstperson_init(unsigned int width, unsigned int height);
void firstperson_update(sf::RenderTarget* renderTarget);
void firstperson_setTexturePosition(float x, float y);

#endif
