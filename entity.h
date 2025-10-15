//
// Created by Kalle on 15.10.2025.
//

#ifndef SPACE_SHOOTER_ENTITY_H
#define SPACE_SHOOTER_ENTITY_H
#include "SFML/Graphics/Sprite.hpp"

enum class HorizontalDirection {
    Left,
    Right,
    None,
};

enum class VerticalDirection {
    Top,
    Bottom,
};

class Entity {
public:
    sf::Sprite sprite;
    HorizontalDirection horizontal_direction;
    VerticalDirection vertical_direction;
    float horizontal_speed;
    float vertical_speed;
    void move();
};


#endif //SPACE_SHOOTER_ENTITY_H
