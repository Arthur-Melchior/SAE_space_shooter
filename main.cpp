#include <iostream>
#include <SFML/Graphics.hpp>

int main() {
    sf::RenderWindow window(sf::VideoMode({800, 600}), "space shooter");
    std::string assets_folder = "./_assets/kenney_pixel-shmup";

    sf::Texture texture;
    if (!texture.loadFromFile(std::format("{}/Ships/ship_0000.png", assets_folder))) {
        return -1;
    }

    sf::Sprite sprite(texture);
    sprite.setPosition({100.f, 100.f});

    while (window.isOpen()) {
        // check all the window's events that were triggered since the last iteration of the loop
        while (const std::optional event = window.pollEvent()) {
            // "close requested" event: we close the window
            if (event->is<sf::Event::Closed>())
                window.close();
        }
        window.clear(sf::Color::Black);
        window.draw(sprite);
        window.display();
    }

    return 0;
}
