//
// Created by Kalle on 15.10.2025.
//

#include "entity.h"

#include "SFML/System/Clock.hpp"

void Entity::center_sprite() {
    sprite.setOrigin({
        sprite.getGlobalBounds().size.x / 2,
        sprite.getGlobalBounds().size.y / 2
    });
}


void Entity::move() {
    sprite.move({
        static_cast<float>(horizontal_direction) * horizontal_speed,
        static_cast<float>(vertical_direction) * vertical_speed
    });
    sprite.setRotation(
        sf::degrees(45 * static_cast<float>(horizontal_direction)
                    + -180 * static_cast<float>(vertical_direction)));
}
