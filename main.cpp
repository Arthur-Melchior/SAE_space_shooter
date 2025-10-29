#include <cmath>
#include <fstream>
#include <SFML/Graphics.hpp>

#include "background.h"
#include "entity.h"
#include "player.h"

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

    sf::Texture ship_texture;
    if (!ship_texture.loadFromFile(std::format("{}/Ships/ship_0000.png", assets_folder))) {
        return -1;
    }

    sf::Texture bullet_texture;
    if (!bullet_texture.loadFromFile(std::format("{}/Tiles/tile_0000.png", assets_folder))) {
        return -1;
    }

    sf::Texture grass_texture;
    if (!grass_texture.loadFromFile(std::format("{}/Tiles/tile_0110.png", assets_folder))) {
        throw std::invalid_argument(std::format("no files found in {}", assets_folder));
    };

    const sf::Clock clock;
    Player player(sf::Sprite(ship_texture), sf::Sprite(bullet_texture), clock, 0.5, 0);
    player.ship_sprite.setPosition({100.f, 100.f});

    std::pmr::vector<Road> roads;
    std::vector<sf::Sprite> temp;
    std::vector<sf::Texture *> temp2;

    sf::Texture road_texture1;
    if (!road_texture1.loadFromFile(std::format("{}/Roads/road_001.png", assets_folder))) {
        throw std::invalid_argument(std::format("no files found in {}", assets_folder));
    }
    sf::Sprite road_spite1(road_texture1);
    sf::Texture road_texture2;
    if (!road_texture2.loadFromFile(std::format("{}/Roads/road_002.png", assets_folder))) {
        throw std::invalid_argument(std::format("no files found in {}", assets_folder));
    }
    sf::Sprite road_spite2(road_texture2);
    sf::Texture road_texture3;
    if (!road_texture3.loadFromFile(std::format("{}/Roads/road_003.png", assets_folder))) {
        throw std::invalid_argument(std::format("no files found in {}", assets_folder));
    }
    sf::Sprite road_spite3(road_texture3);
    sf::Texture road_texture4;
    if (!road_texture4.loadFromFile(std::format("{}/Roads/road_004.png", assets_folder))) {
        throw std::invalid_argument(std::format("no files found in {}", assets_folder));
    }
    sf::Sprite road_spite4(road_texture4);
    sf::Texture road_texture5;
    if (!road_texture5.loadFromFile(std::format("{}/Roads/road_005.png", assets_folder))) {
        throw std::invalid_argument(std::format("no files found in {}", assets_folder));
    }
    sf::Sprite road_spite5(road_texture5);
    sf::Texture road_texture6;
    if (!road_texture6.loadFromFile(std::format("{}/Roads/road_006.png", assets_folder))) {
        throw std::invalid_argument(std::format("no files found in {}", assets_folder));
    }
    sf::Sprite road_spite6(road_texture6);
    sf::Texture road_texture7;
    if (!road_texture7.loadFromFile(std::format("{}/Roads/road_007.png", assets_folder))) {
        throw std::invalid_argument(std::format("no files found in {}", assets_folder));
    }
    sf::Sprite road_spite7(road_texture7);
    sf::Texture road_texture8;
    if (!road_texture8.loadFromFile(std::format("{}/Roads/road_008.png", assets_folder))) {
        throw std::invalid_argument(std::format("no files found in {}", assets_folder));
    }
    sf::Sprite road_spite8(road_texture8);


    roads.emplace_back(Road(road_spite1, road_texture1, road_direction::bottom, road_direction::top));
    roads.emplace_back(Road(road_spite2, road_texture2, road_direction::bottom, road_direction::top));
    roads.emplace_back(Road(road_spite3, road_texture3, road_direction::bottom, road_direction::left));
    roads.emplace_back(Road(road_spite4, road_texture4, road_direction::right, road_direction::left));
    roads.emplace_back(Road(road_spite5, road_texture5, road_direction::right, road_direction::top));
    roads.emplace_back(Road(road_spite6, road_texture6, road_direction::bottom, road_direction::right));
    roads.emplace_back(Road(road_spite7, road_texture7, road_direction::left, road_direction::right));
    roads.emplace_back(Road(road_spite8, road_texture8, road_direction::left, road_direction::top));

    Background background(window, roads, clock, assets_folder, sf::Sprite(grass_texture), 16, 11, 1);

    std::pmr::vector<Entity> enemies;
    std::pmr::vector<Entity> bullets;
    while (window.isOpen()) {
        // check all the window's events that were triggered since the last iteration of the loop
        while (const std::optional event = window.pollEvent()) {
            // "close requested" event: we close the window
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        player.move(window);
        player.shoot(bullets);

        window.clear(sf::Color::Black);

        //toutes les x secondes rajouté 2 nouvelles lignes et supprimer les 2 dernières

        background.draw(window);
        background.draw_road(window);
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
