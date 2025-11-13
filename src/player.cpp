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
            createBullet(current_bullet_type, existing_bullets);
        }
    }
}

void Player::createBullet(BulletType bullet_type, std::pmr::vector<Entity> &bullets) const {
    const sf::Vector2f bullet_position = {
        ship_sprite.getPosition().x + ship_sprite.getGlobalBounds().size.x / 5,
        ship_sprite.getPosition().y - ship_sprite.getGlobalBounds().size.y
    };
    auto make_bullet = [&](const HorizontalDirection h_dir,
                           const VerticalDirection v_dir = VerticalDirection::Top,
                           const sf::Vector2f offset = {0.f, 0.f}) {
        Entity bullet(
            bullet_sprite,
            h_dir, v_dir,
            1.f, 5.f,
            0
        );
        bullet.sprite.setPosition(bullet_position + offset);
        bullets.emplace_back(std::move(bullet));
    };
    switch (bullet_type) {
        case BulletType::simple:
            make_bullet(HorizontalDirection::None);
            break;

        case BulletType::simple_cone:
            make_bullet(HorizontalDirection::None);
            make_bullet(HorizontalDirection::Left);
            make_bullet(HorizontalDirection::Right);
            break;

        case BulletType::pair:
            make_bullet(HorizontalDirection::None, VerticalDirection::Top, {-10.f, 0.f});
            make_bullet(HorizontalDirection::None, VerticalDirection::Top, {10.f, 0.f});
            break;

        case BulletType::pair_cone:
            make_bullet(HorizontalDirection::None, VerticalDirection::Top, {-10.f, 0});
            make_bullet(HorizontalDirection::None, VerticalDirection::Top, {10.f, 0});
            make_bullet(HorizontalDirection::Left, VerticalDirection::Top, {-40.f, 0.f});
            make_bullet(HorizontalDirection::Left, VerticalDirection::Top, {-20.f, 0.f});
            make_bullet(HorizontalDirection::Right, VerticalDirection::Top, {30.f, 0.f});
            make_bullet(HorizontalDirection::Right, VerticalDirection::Top, {50.f, 0.f});
            break;

        case BulletType::triple:
            make_bullet(HorizontalDirection::None);
            make_bullet(HorizontalDirection::None, VerticalDirection::Top, {-15.f, 0.f});
            make_bullet(HorizontalDirection::None, VerticalDirection::Top, {15.f, 0.f});
            break;

        case BulletType::triple_cone:
            make_bullet(HorizontalDirection::None, VerticalDirection::Top, {-15.f, 0});
            make_bullet(HorizontalDirection::None, VerticalDirection::Top, {0.f, 0});
            make_bullet(HorizontalDirection::None, VerticalDirection::Top, {15.f, 0});
            make_bullet(HorizontalDirection::Left, VerticalDirection::Top, {-60.f, 0.f});
            make_bullet(HorizontalDirection::Left, VerticalDirection::Top, {-45.f, 0.f});
            make_bullet(HorizontalDirection::Left, VerticalDirection::Top, {-30.f, 0.f});
            make_bullet(HorizontalDirection::Right, VerticalDirection::Top, {60.f, 0.f});
            make_bullet(HorizontalDirection::Right, VerticalDirection::Top, {45.f, 0.f});
            make_bullet(HorizontalDirection::Right, VerticalDirection::Top, {30.f, 0.f});
            break;

        default:
            break;
    }
}
