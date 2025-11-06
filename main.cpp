#include <cmath>
#include <fstream>
#include <iostream>
#include <SFML/Graphics.hpp>

#include "background.h"
#include "entity.h"
#include "player.h"
#include "resources_manager.h"
#include "state_manager.h"

void delete_out_of_bounds(const sf::RenderWindow &window, std::pmr::vector<Entity> &entities) {
    int i = 0;
    while (i < entities.size()) {
        if (auto &projectile = entities[i];
            projectile.sprite.getPosition().x < 0
            || projectile.sprite.getPosition().x > static_cast<float>(window.getSize().x)
            || projectile.sprite.getPosition().y < 0
            || projectile.sprite.getPosition().y > static_cast<float>(window.getSize().y)) {
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

    std::pmr::string assets_folder = "./_assets/kenney_pixel-shmup";
    ResourceManager resource_manager(assets_folder);
    StateManager state_manager(0);

    auto *ship_sprite = resource_manager.find_sprite("ship_0000");
    auto *explosion_sprite = resource_manager.find_sprite("explosion");

    sf::Clock clock;
    Player player(*resource_manager.find_sprite("ship_0000"),
                  *resource_manager.find_sprite("tile_0000"),
                  clock,
                  0.5,
                  0);
    player.ship_sprite.setPosition({100.f, 100.f});
    player.ship_sprite.setOrigin({
        player.ship_sprite.getGlobalBounds().size.x / 4,
        player.ship_sprite.getGlobalBounds().size.y / 4
    });

    std::pmr::vector<Road> roads;
    std::vector<sf::Sprite> temp;

    for (int i = 1; i < 9; ++i) {
        sf::Sprite sprite(*resource_manager.find_sprite(std::format("road_00{}", i)));
        temp.emplace_back(sprite);
    }

    roads.emplace_back(Road(temp[0], road_direction::bottom, road_direction::top));
    roads.emplace_back(Road(temp[1], road_direction::bottom, road_direction::top));
    roads.emplace_back(Road(temp[2], road_direction::bottom, road_direction::left));
    roads.emplace_back(Road(temp[3], road_direction::right, road_direction::left));
    roads.emplace_back(Road(temp[4], road_direction::right, road_direction::top));
    roads.emplace_back(Road(temp[5], road_direction::bottom, road_direction::right));
    roads.emplace_back(Road(temp[6], road_direction::left, road_direction::right));
    roads.emplace_back(Road(temp[7], road_direction::left, road_direction::top));

    Background background(window,
                          roads,
                          clock,
                          assets_folder,
                          *resource_manager.find_sprite("tile_0110"),
                          16, 11,
                          1);

    auto explosion_scale = sf::Vector2f(16 / explosion_sprite->getGlobalBounds().size.x,
                                        16 / explosion_sprite->getGlobalBounds().size.y);

    std::pmr::vector<Entity> enemies;
    std::pmr::vector<Entity> bullets;
    std::pmr::vector<Entity> explosions;

    float time = 0;
    sf::Text score(resource_manager.font);
    score.setPosition({400, 100});

    bool game_over = false;

    while (window.isOpen()) {
        // check all the window's events that were triggered since the last iteration of the loop
        while (const std::optional event = window.pollEvent()) {
            // "close requested" event: we close the window
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        window.clear(sf::Color::Black);

        //Display background
        background.draw(window);

        //Player logic
        player.move(window);
        player.shoot(bullets);
        window.draw(player.ship_sprite);

        //Bullets logic
        delete_out_of_bounds(window, bullets);
        for (auto &bullet: bullets) {
            //Check for collision with enemies
            for (int i = 0; i < enemies.size(); ++i) {
                if (auto &enemy = enemies[i]; enemy.sprite.getGlobalBounds().findIntersection(
                    bullet.sprite.getGlobalBounds())) {
                    //increase score, creates an explosion and removes enemy
                    state_manager.score++;
                    auto explosion = Entity(*explosion_sprite,
                                            HorizontalDirection::None,
                                            VerticalDirection::None,
                                            0, 0,
                                            0);
                    explosion.sprite.setScale(explosion_scale);
                    explosion.sprite.setPosition(enemy.sprite.getPosition());
                    explosions.emplace_back(explosion);
                    enemies[i] = enemies.back();
                    enemies.pop_back();
                }
            }
            bullet.move();
            window.draw(bullet.sprite);
        }

        //HUD logic
        score.setString(std::format("score : {}", state_manager.score));

        //Enemies logic
        delete_out_of_bounds(window, enemies);

        if (time > 1) {
            Entity entity(*ship_sprite, HorizontalDirection::Left, VerticalDirection::Bottom, 1, 1);
            entity.sprite.setPosition({400, 0});
            Entity entity2(*ship_sprite, HorizontalDirection::Right, VerticalDirection::Bottom, 1, 1);
            entity2.sprite.setPosition({400, 0});
            enemies.emplace_back(entity);
            enemies.emplace_back(entity2);
            time = 0;
            clock.reset();
        }

        for (auto &enemy: enemies) {
            //check for collision with player
            if (enemy.sprite.getGlobalBounds().findIntersection(player.ship_sprite.getGlobalBounds())) {
                game_over = true;
            }
            enemy.move();
            window.draw(enemy.sprite);
        }

        //Explosions logic
        for (auto explosion: explosions) {
            explosion.delta_time += clock.getElapsedTime().asSeconds();
            if (explosion.delta_time < 1) {
                window.draw(explosion.sprite);
            }
        }

        time += clock.restart().asSeconds();
        window.display();
    }

    return 0;
}
