#ifndef FIRSTPERSON_H
#define FIRSTPERSON_H

#include <SFML/Graphics.hpp>
#include "camera.h"

int firstperson_init(unsigned int width, unsigned int height);
void firstperson_update(sf::RenderTarget* renderTarget, Camera* camera);
void firstperson_setTexturePosition(float x, float y);

#endif
