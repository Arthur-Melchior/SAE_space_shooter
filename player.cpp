#include "player.h"

#include "SFML/Window/Keyboard.hpp"

void Player::move() {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
        ship_sprite.move({0, 5});
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
        ship_sprite.move({0, -5});
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
        ship_sprite.move({5, 0});
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
        ship_sprite.move({-5, 0});
    }
}

bool Player::can_shoot() {
    time_elapsed += clock.restart().asSeconds();

    if (time_elapsed >= reload_speed) {
        clock.reset();
        time_elapsed = 0;
        return true;
    }

    return false;
}


void Player::shoot(std::pmr::vector<Entity> &existing_bullets) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::B)) {
        if (can_shoot()) {
            auto bullet_position = sf::Vector2f(ship_sprite.getPosition().x + ship_sprite.getGlobalBounds().size.x / 2,
                                                ship_sprite.getPosition().y);
            Entity entity(bullet_sprite,
                          HorizontalDirection::None,
                          VerticalDirection::Top,
                          1, 3);
            entity.sprite.setOrigin({
                entity.sprite.getGlobalBounds().size.x / 2, entity.sprite.getGlobalBounds().size.y / 2
            });
            entity.sprite.setPosition(bullet_position);
            Entity entity2(bullet_sprite,
                           HorizontalDirection::Left,
                           VerticalDirection::Top,
                           3, 3);
            entity2.sprite.setOrigin({
                entity2.sprite.getGlobalBounds().size.x / 2, entity2.sprite.getGlobalBounds().size.y / 2
            });
            entity2.sprite.setRotation(sf::degrees(-45));
            entity2.sprite.setPosition(bullet_position);
            Entity entity3(bullet_sprite,
                           HorizontalDirection::Right,
                           VerticalDirection::Top,
                           3, 3);
            entity3.sprite.setOrigin({
                entity3.sprite.getGlobalBounds().size.x / 2, entity3.sprite.getGlobalBounds().size.y / 2
            });
            entity3.sprite.setRotation(sf::degrees(45));
            entity3.sprite.setPosition(bullet_position);
            existing_bullets.emplace_back(entity);
            existing_bullets.emplace_back(entity2);
            existing_bullets.emplace_back(entity3);
        }
    }
}
