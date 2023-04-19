#ifndef CAMERA_H
#define CAMERA_H

#include <SFML/Graphics.hpp>

typedef struct
{
	float direction;
	float distance;
} CameraRay;

typedef struct
{
	sf::Vector2f pos;
	float direction;
	unsigned int resolution;
	float fov;

	CameraRay* rays;
} Camera;

int camera_init(Camera* camera, sf::Vector2f pos, float direction, unsigned int resolution, float fov);
void camera_update(Camera* camera, float t);
void camera_destroy(Camera* camera);

#endif
