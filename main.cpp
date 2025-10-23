#include <cmath>
#include <fstream>
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

enum class road_direction {
    bottom,
    top,
    left,
    right,
};

struct Road {
    sf::Sprite sprite;
    road_direction road_start;
    road_direction road_end;
};

int main() {
    sf::RenderWindow window(sf::VideoMode({800, 600}), "space shooter");
    window.setFramerateLimit(60);
    float window_size = static_cast<float>(window.getSize().x);

    std::string assets_folder = "./_assets/kenney_pixel-shmup";
    float tile_size = 16;
    float tiles_per_window = 20;
    float distance_per_tile = window_size / tiles_per_window;
    float tile_scale = distance_per_tile / tile_size;
    float road_index = std::round(tiles_per_window / 2);

    std::vector<Road> roads;
    std::vector<sf::Sprite> temp;

    for (auto road: std::filesystem::directory_iterator(std::format("{}/Roads", assets_folder))) {
        sf::Texture road_texture;
        if (!road_texture.loadFromFile(road.path())) {
            return -1;
        }

        sf::Sprite road_sprite(road_texture);
        temp.emplace_back(road_sprite);
    }

    roads.emplace_back(Road(temp[0], road_direction::bottom, road_direction::top));
    roads.emplace_back(Road(temp[1], road_direction::bottom, road_direction::top));
    roads.emplace_back(Road(temp[2], road_direction::bottom, road_direction::left));
    roads.emplace_back(Road(temp[3], road_direction::right, road_direction::left));
    roads.emplace_back(Road(temp[4], road_direction::right, road_direction::top));
    roads.emplace_back(Road(temp[5], road_direction::bottom, road_direction::right));
    roads.emplace_back(Road(temp[6], road_direction::left, road_direction::right));
    roads.emplace_back(Road(temp[7], road_direction::left, road_direction::top));

    for (auto road: roads) {
        road.sprite.setScale({
            tile_scale,
            tile_scale
        });
    }

    sf::Texture grass_tile;
    if (!grass_tile.loadFromFile(std::format("{}/Tiles/tile_0110.png", assets_folder))) {
        return -1;
    };
    sf::Sprite grass(grass_tile);
    grass.setScale({
        tile_scale,
        tile_scale
    });

    std::vector<std::vector<sf::Sprite> > board_tiles;

    for (int x = 0; x < tiles_per_window; ++x) {
        board_tiles.emplace_back();
        for (int y = 0; y < tiles_per_window; ++y) {
            board_tiles[x].emplace_back(sf::Sprite(grass));
        }
    }

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
    float test = 0;
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

        test += 0.5;
        for (int x = 0; x < board_tiles.size(); ++x) {
            for (int y = 0; y < board_tiles.size(); ++y) {
                auto &tile = board_tiles[x][y];
                tile.setPosition({
                    x * distance_per_tile,
                    y * distance_per_tile + test
                });
                window.draw(tile);
            }
        }

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
