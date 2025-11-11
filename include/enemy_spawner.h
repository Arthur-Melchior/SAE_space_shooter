//
// Created by Kalle on 11.11.2025.
//

#ifndef SPACE_SHOOTER_ENEMY_SPAWNER_H
#define SPACE_SHOOTER_ENEMY_SPAWNER_H
#include <random>

#include "entity.h"
#include "resources_manager.h"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/System/Vector2.hpp"

class EnemySpawner {
public:
  EnemySpawner(const sf::RenderWindow& window);
  Entity spawnEnemy(const sf::Sprite &enemy_sprite,
    HorizontalDirection horizontal_direction = HorizontalDirection::None,
    VerticalDirection vertical_direction = VerticalDirection::Bottom,
    float horizontal_speed = 1,
    float vertical_speed = 2,
    sf::Vector2f position = {0,0});
private:
  std::random_device rd_;
  std::mt19937 gen_;
  std::uniform_int_distribution<> distrib_;
};

#endif //SPACE_SHOOTER_ENEMY_SPAWNER_H