//
// Created by Kalle on 15.10.2025.
//

#ifndef SPACE_SHOOTER_ENTITY_H
#define SPACE_SHOOTER_ENTITY_H
#include "SFML/Graphics/Sprite.hpp"

enum class HorizontalDirection {
    Left = -1,
    None = 0,
    Right = 1,
};

enum class VerticalDirection {
    Top = -1,
    None = 0,
    Bottom = 1,
};

class Entity {
public:
    void center_sprite();
    sf::Sprite sprite;
    HorizontalDirection horizontal_direction;
    VerticalDirection vertical_direction;
    float horizontal_speed;
    float vertical_speed;
    float delta_time;
    void move();
};


#endif //SPACE_SHOOTER_ENTITY_H
