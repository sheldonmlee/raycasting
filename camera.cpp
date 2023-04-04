#include "camera.h"

#include "maths.h"
#include "level.h"

#define ROTATION_SPEED PI
#define TRANSLATIONAL_SPEED 100.f

static void draw(Camera* camera, sf::RenderWindow* window);
static void drawRays(Camera* camera, sf::RenderWindow* window);
static void drawLine(sf::RenderWindow* window, sf::Vector2f pos, float angle, float length, sf::Color color);
static void move(Camera* camera, float t);

void camera_update(Camera* camera, sf::RenderWindow* window, float t)
{
	if (!camera || !window) return; 

	draw(camera, window);
	move(camera, t);
}

static void draw(Camera* camera, sf::RenderWindow* window)
{
	const float circleRadius = 5.f;
	sf::CircleShape circle(circleRadius);
	circle.setPosition(camera->pos);
	circle.setOrigin(circleRadius, circleRadius);
	circle.setFillColor(sf::Color::Green);

	drawRays(camera, window);
	drawLine(window, camera->pos, camera->direction, 100, sf::Color::Red);
	window->draw(circle);
}

static void drawRays(Camera* camera, sf::RenderWindow* window)
{
	float rayDirection = 0;
	float rayDirectionStep = camera->fov / (float)camera->resolution;
	bool isOddResolution = (camera->resolution % 2);
	float rayDirectionOffset = isOddResolution? 0 : rayDirectionStep / 2.f;

	for (unsigned int i = 0; i < camera->resolution; i++) {
		if (isOddResolution && i == 0) 
			rayDirection = camera->direction;
		else if (i % 2) 
			rayDirection = camera->direction - rayDirectionOffset;
		else 
			rayDirection = camera->direction + rayDirectionOffset;

		float distance = level_rayCastDistance(camera->pos, rayDirection);

		drawLine(window, camera->pos, rayDirection, distance, sf::Color::Blue);

		if ((i + isOddResolution) % 2) rayDirectionOffset += rayDirectionStep;
	}
}

static void drawLine(sf::RenderWindow* window, sf::Vector2f pos, float angle, float length, sf::Color color)
{
	if (!window) return;

	sf::Vector2f endOffset(length * cos(angle), length * sin(angle));
	sf::Vertex start(pos);
	sf::Vertex end(pos + endOffset);

	start.color = color;
	end.color = color;

	sf::Vertex line[] = { start, end };

	window->draw(line, 2, sf::Lines);
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
