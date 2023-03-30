#include <SFML/Graphics.hpp>
#include <stdio.h>

int main()
{
	sf::Uint32 style = sf::Style::Titlebar;
	sf::RenderWindow window(sf::VideoMode(1600, 900), "SFML works!", style);
    sf::CircleShape shape(450.f);
    shape.setFillColor(sf::Color::Green);

	printf("Hello meme!\n");

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(shape);
        window.display();
    }

    return 0;
}
