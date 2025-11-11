#include "../include/player.h"

#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Window/Keyboard.hpp"

void Player::move(const sf::RenderWindow &window) {
    const auto ship_position = ship_sprite.getPosition();
    const auto window_size = window.getSize();

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) && ship_position.y < window_size.y - ship_sprite.
        getGlobalBounds().size.y) {
        ship_sprite.move({0, 5});
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) && ship_position.y > 0) {
        ship_sprite.move({0, -5});
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) && ship_position.x < window_size.x - ship_sprite.
        getGlobalBounds().size.x) {
        ship_sprite.move({5, 0});
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) && ship_position.x > 0) {
        ship_sprite.move({-5, 0});
    }
}

bool Player::can_shoot() {
    delta_time += clock.restart().asSeconds();

    if (delta_time >= reload_speed) {
        delta_time = 0;
        return true;
    }

    return false;
}


void Player::shoot(std::pmr::vector<Entity> &existing_bullets) {
    {
        if (can_shoot()) {
            auto x = ship_sprite.getPosition().x + ship_sprite.getGlobalBounds().size.x / 4;
            auto y = ship_sprite.getPosition().y;
            Entity entity(bullet_sprite,
                          HorizontalDirection::None,
                          VerticalDirection::Top,
                          3, 4);
            entity.center_origin();
            entity.sprite.setPosition({x, y - 30});
            Entity entity2(bullet_sprite,
                           HorizontalDirection::Left,
                           VerticalDirection::Top,
                           3, 4);
            entity2.center_origin();
            entity2.sprite.setPosition({x, y - 30});
            Entity entity3(bullet_sprite,
                           HorizontalDirection::Right,
                           VerticalDirection::Top,
                           3, 4);
            entity3.center_origin();
            entity3.sprite.setPosition({x, y - 30});
            existing_bullets.emplace_back(entity);
            existing_bullets.emplace_back(entity2);
            existing_bullets.emplace_back(entity3);
        }
    }
}
