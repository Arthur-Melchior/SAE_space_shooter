#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "entity.h"
#include "player.h"

void delete_out_of_bounds(const sf::RenderWindow &window, std::pmr::vector<Entity> &entities) {
    int i = 0;
    while (i < entities.size()) {
        if (auto &projectile = entities[i];
            projectile.sprite.getPosition().x < 0
            || projectile.sprite.getPosition().x > window.getSize().x
            || projectile.sprite.getPosition().y < 0
            || projectile.sprite.getPosition().y > window.getSize().y) {
            //found on the internet
            //destroys the last element to avoid shifting every element with vector.erase()
            entities[i] = entities.back();
            entities.pop_back();
        } else {
            ++i;
        }
    }
}

int main() {
    sf::RenderWindow window(sf::VideoMode({800, 600}), "space shooter");
    window.setFramerateLimit(60);
    std::string assets_folder = "./_assets/kenney_pixel-shmup";

    sf::Texture ship_texture;
    if (!ship_texture.loadFromFile(std::format("{}/Ships/ship_0000.png", assets_folder))) {
        return -1;
    }

    sf::Texture bullet_texture;
    if (!bullet_texture.loadFromFile(std::format("{}/Tiles/tile_0000.png", assets_folder))) {
        return -1;
    }

    Player player(sf::Sprite(ship_texture), sf::Sprite(bullet_texture), sf::Clock(), 0.5, 0);
    player.ship_sprite.setPosition({100.f, 100.f});

    std::pmr::vector<Entity> enemies;
    std::pmr::vector<Entity> bullets;
    for (float i = 0; i < 50; ++i) {
        enemies.emplace_back(Entity(sf::Sprite(ship_texture),
                                    HorizontalDirection::None,
                                    VerticalDirection::Bottom,
                                    i * 0.5,
                                    1)).sprite.setPosition({i * 5, 0});
    }
    while (window.isOpen()) {
        // check all the window's events that were triggered since the last iteration of the loop
        while (const std::optional event = window.pollEvent()) {
            // "close requested" event: we close the window
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        player.move();
        player.shoot(bullets);

        window.clear(sf::Color::Black);
        window.draw(player.ship_sprite);

        delete_out_of_bounds(window, enemies);
        delete_out_of_bounds(window, bullets);

        for (auto &bullet: bullets) {
            for (int i = 0; i < enemies.size(); ++i) {
                if (auto &entity = enemies[i]; entity.sprite.getGlobalBounds().findIntersection(
                    bullet.sprite.getGlobalBounds())) {
                    enemies[i] = enemies.back();
                    enemies.pop_back();
                }
            }
            bullet.move();
            window.draw(bullet.sprite);
        }

        for (auto &enemy: enemies) {
            enemy.move();
            window.draw(enemy.sprite);
        }

        window.display();
    }

    return 0;
}
