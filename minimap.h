#ifndef MINIMAP_H
#define MINIMAP_H

#include <SFML/Graphics.hpp>
#include "camera.h"

int minimap_init(unsigned int size);
void minimap_update(sf::RenderTarget* renderTarget, Camera* camera);
void minimap_setTexturePosition(float x, float y);

#endif
