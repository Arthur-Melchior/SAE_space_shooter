//
// Created by Kalle on 11.11.2025.
//

#include "../include/enemy_spawner.h"

#include "../include/entity.h"

EnemySpawner::EnemySpawner(const sf::RenderWindow &window) {
    gen_ = std::mt19937(rd_());
    distrib_ = std::uniform_int_distribution<>(20, window.getSize().x - 20);
}

Entity EnemySpawner::spawnEnemy(const sf::Sprite &enemy_sprite,
                                const HorizontalDirection horizontal_direction,
                                const VerticalDirection vertical_direction,
                                const float horizontal_speed,
                                const float vertical_speed,
                                sf::Vector2f position) {
    if (position.x == 0 && position.y == 0) {
        position.x = distrib_(gen_);
    }

    Entity enemy(enemy_sprite,
                 horizontal_direction,
                 vertical_direction,
                 horizontal_speed,
                 vertical_speed,
                 0);
    enemy.sprite.setRotation(sf::degrees(180));
    enemy.sprite.setPosition(position);
    return enemy;
}
