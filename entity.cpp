//
// Created by Kalle on 15.10.2025.
//

#include "entity.h"

#include "SFML/System/Clock.hpp"

void Entity::move() {
    if (this->horizontal_direction == HorizontalDirection::Right) {
        this->sprite.move({1 * this->horizontal_speed, 0});
    } else if (this->horizontal_direction == HorizontalDirection::Left) {
        this->sprite.move({-1 * this->horizontal_speed, 0});
    }

    if (this->vertical_direction == VerticalDirection::Top) {
        this->sprite.move({0, -1 * this->vertical_speed});
    } else {
        this->sprite.move({0, 1 * this->vertical_speed});
    }
}
