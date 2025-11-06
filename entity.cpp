//
// Created by Kalle on 15.10.2025.
//

#include "entity.h"

#include <cmath>
#include <iostream>

#include "resources_manager.h"
#include "SFML/System/Clock.hpp"

void Entity::center_origin() {
    sprite.setOrigin({
        sprite.getGlobalBounds().size.x / 2,
        sprite.getGlobalBounds().size.y / 2
    });
}

void Entity::shoot(const float elapsed_time, const float reload_speed, std::pmr::vector<Entity> &bullets,
                   const sf::Sprite &bullet_sprite) {
    delta_time += elapsed_time;
    if (delta_time > reload_speed) {
        Entity bullet(bullet_sprite, HorizontalDirection::None, VerticalDirection::Bottom, 1, 5);
        bullet.center_origin();
        bullet.sprite.setPosition({
            sprite.getPosition().x, sprite.getGlobalBounds().position.y + sprite.getGlobalBounds().size.y + 10
        });
        bullets.emplace_back(bullet);
        delta_time = 0;
    }
}

void Entity::move() {
    if (zig_zag) {
        if (test > std::abs(horizontal_speed * 20) || test < -std::abs(horizontal_speed * 20)) {
            horizontal_speed *= -1;
        }
        test += horizontal_speed;
        sprite.move({
            test / 10,
            static_cast<float>(vertical_direction) * vertical_speed
        });
    } else {
        sprite.move({

            static_cast<float>(horizontal_direction) * horizontal_speed,
            static_cast<float>(vertical_direction) * vertical_speed
        });
    }
}
