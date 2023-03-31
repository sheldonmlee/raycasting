#ifndef CAMERA_H
#define CAMERA_H

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>

typedef struct
{
	sf::Vector2f pos;
	float direction;
	float resolution;
	float fov;
} Camera;

void camera_update(Camera* camera, sf::RenderWindow* window);

#endif
