#include <iostream>
#include <SFML/Graphics.hpp>

#include "entity.h"
#include "player.h"
#include "cmake-build-debug/_deps/sfml-src/src/SFML/Window/InputImpl.hpp"

void delete_out_of_bounds(const sf::RenderWindow &window, std::pmr::vector<Entity> entities) {
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

    Player player(sf::Sprite(ship_texture), sf::Clock(), 0.5, 0);
    player.sprite.setPosition({100.f, 100.f});

    std::pmr::vector<Entity> enemies;
    std::pmr::vector<Entity> bullets;
    for (int i = 0; i < 10; ++i) {
        enemies.emplace_back(Entity(sf::Sprite(ship_texture),
                                    HorizontalDirection::Right,
                                    VerticalDirection::None,
                                    i * 0.5,
                                    1));
    }
    while (window.isOpen()) {
        // check all the window's events that were triggered since the last iteration of the loop
        while (const std::optional event = window.pollEvent()) {
            // "close requested" event: we close the window
            if (event->is<sf::Event::Closed>())
                window.close();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
            player.sprite.move({0, 5});
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
            player.sprite.move({0, -5});
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
            player.sprite.move({5, 0});
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
            player.sprite.move({-5, 0});
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::B)) {
            if (player.can_shoot()) {
                sf::Sprite projectile(ship_texture);
                Entity entity(projectile,
                              HorizontalDirection::None,
                              VerticalDirection::Top,
                              1, 3);
                entity.sprite.setPosition({player.sprite.getPosition().x, player.sprite.getPosition().y});
                Entity entity2(projectile,
                               HorizontalDirection::Left,
                               VerticalDirection::Top,
                               1, 3);
                entity2.sprite.setPosition({player.sprite.getPosition().x, player.sprite.getPosition().y});
                Entity entity3(projectile,
                               HorizontalDirection::Right,
                               VerticalDirection::Top,
                               1, 3);
                entity3.sprite.setPosition({player.sprite.getPosition().x, player.sprite.getPosition().y});
                bullets.emplace_back(entity);
                bullets.emplace_back(entity2);
                bullets.emplace_back(entity3);
            }
        }

        window.clear(sf::Color::Black);
        window.draw(player.sprite);

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
