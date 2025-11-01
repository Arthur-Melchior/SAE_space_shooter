#include "player.h"

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
            auto x = ship_sprite.getPosition().x + ship_sprite.getGlobalBounds().size.x / 4;
            auto y = ship_sprite.getPosition().y;
            Entity entity(bullet_sprite,
                          HorizontalDirection::None,
                          VerticalDirection::Top,
                          3, 3);
            entity.center_sprite();
            entity.sprite.setPosition({x, y});
            Entity entity2(bullet_sprite,
                           HorizontalDirection::Left,
                           VerticalDirection::Top,
                           3, 3);
            entity2.center_sprite();
            entity2.sprite.setPosition({x, y});
            Entity entity3(bullet_sprite,
                           HorizontalDirection::Right,
                           VerticalDirection::Top,
                           3, 3);
            entity3.center_sprite();
            entity3.sprite.setPosition({x, y});
            existing_bullets.emplace_back(entity);
            existing_bullets.emplace_back(entity2);
            existing_bullets.emplace_back(entity3);
        }
    }
}
