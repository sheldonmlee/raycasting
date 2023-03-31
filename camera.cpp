#include "camera.h"
#include <cmath>

#include <SFML/Graphics/Color.hpp>

#define PI 3.14159265

static void drawLine(sf::RenderWindow* window, sf::Vector2f pos, float angle, float length, sf::Color color);

void camera_update(Camera* camera, sf::RenderWindow* window)
{
	if (!camera || !window) return; 

	drawLine(window, camera->pos, camera->direction, 100, sf::Color::Red);
}

static void drawLine(sf::RenderWindow* window, sf::Vector2f pos, float angle, float length, sf::Color color)
{
	if (!window) return;

	sf::Vector2f endOffset(length * cos(angle * PI/180.f), length * sin(angle * PI/180.3));
	sf::Vertex start(pos);
	sf::Vertex end(pos + endOffset);

	start.color = color;
	end.color = color;

	sf::Vertex line[] = { start, end };

	window->draw(line, 2, sf::Lines);
}
