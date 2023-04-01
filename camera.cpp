#include "camera.h"
#include <cmath>

#define PI 3.14159265
#define DEG_RAD PI/180.f
#define ROTATION_SPEED 180
#define TRANSLATIONAL_SPEED 100.f

static void drawLine(sf::RenderWindow* window, sf::Vector2f pos, float angle, float length, sf::Color color);
static void draw(Camera* camera, sf::RenderWindow* window);
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

	drawLine(window, camera->pos, camera->direction, 100, sf::Color::Red);
	window->draw(circle);
}

static void drawLine(sf::RenderWindow* window, sf::Vector2f pos, float angle, float length, sf::Color color)
{
	if (!window) return;

	sf::Vector2f endOffset(length * cos(angle * DEG_RAD), length * sin(angle * DEG_RAD));
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
	sf::Vector2f offset(magnitude * cos(camera->direction * DEG_RAD), magnitude *sin(camera->direction * DEG_RAD));

	camera->pos += offset;
	camera->direction += rotation * t * ROTATION_SPEED;
}
