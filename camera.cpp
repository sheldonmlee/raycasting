#include "camera.h"
#include <stdio.h>

#include "maths.h"
#include "level.h"

#define ROTATION_SPEED PI
#define TRANSLATIONAL_SPEED 1.f

static void move(Camera* camera, float t);
static void castRays(Camera* camera);

int camera_init(Camera* camera, sf::Vector2f pos, float direction, unsigned int resolution, float fov)
{
	camera->pos = pos;
	camera->direction = direction;
	camera->resolution = resolution;
	camera->fov = fov;

	camera->rays = (CameraRay*)malloc(sizeof(CameraRay)*resolution);
	return 1;
}

void camera_update(Camera* camera, float t)
{
	if (!camera) return; 
	move(camera, t);
	castRays(camera);
}

static void move(Camera* camera, float t)
{
	int forward = 0;
	int rotation = 0;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::F))
		forward += 1;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		forward -= 1;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::T))
		rotation +=1;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
		rotation -=1;

	float magnitude = forward * t * TRANSLATIONAL_SPEED;
	sf::Vector2f offset(magnitude * cos(camera->direction), magnitude * sin(camera->direction));

	camera->pos += offset;
	camera->direction += rotation * t * ROTATION_SPEED;
}

void camera_destroy(Camera *camera)
{
	free(camera->rays);
}

static void castRays(Camera* camera)
{
	float rayDirectionStep = camera->fov / (float)camera->resolution;
	float rayDirection = camera->direction - camera->fov/2.f + rayDirectionStep/2.f;
	for (unsigned int i = 0; i < camera->resolution; i++) {
		camera->rays[i].direction = rayDirection;
		camera->rays[i].distance = level_rayCastDistance(camera->pos, rayDirection);

		rayDirection += rayDirectionStep;
	}
}
