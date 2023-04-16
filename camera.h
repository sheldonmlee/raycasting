#ifndef CAMERA_H
#define CAMERA_H

#include <SFML/Graphics.hpp>

typedef struct
{
	sf::Vector2f pos;
	float direction;
	unsigned int resolution;
	float fov;
} Camera;

void camera_update(Camera* camera, float t);

#endif
