//
// Created by Kalle on 15.10.2025.
//

#include "entity.h"

#include "SFML/System/Clock.hpp"

void Entity::move() {
    this->sprite.move({
        static_cast<float>(horizontal_direction) * horizontal_speed,
        static_cast<float>(vertical_direction) * vertical_speed
    });
}
