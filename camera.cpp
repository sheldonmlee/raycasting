#include "camera.h"

#include "maths.h"
#include "level.h"

#define ROTATION_SPEED PI
#define TRANSLATIONAL_SPEED 1.f

static void move(Camera* camera, float t);

void camera_update(Camera* camera, float t)
{
	if (!camera) return; 

	move(camera, t);
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
	sf::Vector2f offset(magnitude * cos(camera->direction), magnitude *sin(camera->direction));

	camera->pos += offset;
	camera->direction += rotation * t * ROTATION_SPEED;
}
