//
// Created by Kalle on 06.11.2025.
//

#include "game.h"

#include "background.h"
#include "player.h"
#include "resources_manager.h"
#include "state_manager.h"
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Graphics/Text.hpp"


void game::setup() {
    window_ = sf::RenderWindow(sf::VideoMode({800, 600}), "space shooter");
    window_.setFramerateLimit(60);

    const std::pmr::string assets_folder = "./_assets/kenney_pixel-shmup";
    resource_manager_ = ResourceManager(assets_folder);
    state_manager_ = StateManager();

    clock_ = sf::Clock();
    player_ = Player(*resource_manager_.find_sprite("ship_0000"),
                     *resource_manager_.find_sprite("tile_0000"),
                     clock_,
                     0.5,
                     0);
    player_->ship_sprite.setPosition({100.f, 100.f});
    player_->ship_sprite.setOrigin({
        player_->ship_sprite.getGlobalBounds().size.x / 4,
        player_->ship_sprite.getGlobalBounds().size.y / 4
    });

    background_ = Background(window_,
                             clock_,
                             assets_folder,
                             *resource_manager_.find_sprite("tile_0110"),
                             16, 11,
                             1);

    enemies_ = std::pmr::vector<Entity>();
    bullets_ = std::pmr::vector<Entity>();
    explosions_ = std::pmr::vector<Entity>();

    show_bounding_box_ = false;
    game_over_ = false;
}

bool game::loop() {
    auto *explosion_sprite = resource_manager_.find_sprite("explosion");
    auto score = sf::Text(resource_manager_.font);
    float explosion_scale = 1;

    while (window_.isOpen()) {
        // check all the window_'s events that were triggered since the last iteration of the loop
        while (const std::optional event = window_.pollEvent()) {
            // "close requested" event: we close the window_
            if (event->is<sf::Event::Closed>())
                window_.close();
            if (event->is<sf::Event::KeyPressed>()) {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::X)) {
                    show_bounding_box_ = !show_bounding_box_;
                }
            }
        }


        window_.clear(sf::Color::Black);

        //Display background_
        background_->draw(window_);

        //Player logic
        player_->move(window_);
        window_.draw(player_->ship_sprite);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::B)) {
            player_->shoot(bullets_);
        }

        //Bullets logic
        deleteOutOfBounds(bullets_);
        for (auto &bullet: bullets_) {
            //Check for collision with enemies_
            for (int i = 0; i < enemies_.size(); ++i) {
                if (auto &enemy = enemies_[i]; enemy.sprite.getGlobalBounds().findIntersection(
                    bullet.sprite.getGlobalBounds())) {
                    //increase score, creates an explosion and removes enemy
                    state_manager_.score++;
                    auto explosion = Entity(*explosion_sprite,
                                            HorizontalDirection::None,
                                            VerticalDirection::None,
                                            0, 0,
                                            0);
                    explosion.sprite.setScale({explosion_scale, explosion_scale});
                    explosion.sprite.setPosition(enemy.sprite.getPosition());
                    explosions_.emplace_back(explosion);
                    enemies_[i] = enemies_.back();
                    enemies_.pop_back();
                }
            }
            bullet.move();
            window_.draw(bullet.sprite);
        }

        //HUD logic
        score.setString(std::format("score : {}", state_manager_.score));

        //Enemies logic
        deleteOutOfBounds(enemies_);

        if (show_bounding_box_) {
            for (const auto &enemy: enemies_) {
                auto sprite = enemy.sprite;
                sf::FloatRect bounds = sprite.getGlobalBounds();

                sf::RectangleShape boundingBox;
                boundingBox.setPosition(bounds.position);
                boundingBox.setSize(bounds.size);
                boundingBox.setFillColor(sf::Color::Transparent);
                boundingBox.setOutlineColor(sf::Color::Red);
                boundingBox.setOutlineThickness(1.f);

                window_.draw(boundingBox);
            }
        }

        for (auto &enemy: enemies_) {
            //check for collision with player_
            if (enemy.sprite.getGlobalBounds().findIntersection(player_->ship_sprite.getGlobalBounds())) {
                game_over_ = true;
            }
            enemy.move();
            window_.draw(enemy.sprite);
        }


        //Explosions logic
        for (auto explosion: explosions_) {
            explosion.delta_time += clock_.getElapsedTime().asSeconds();
            if (explosion.delta_time < 1) {
                window_.draw(explosion.sprite);
            }
        }
        clock_.restart();
        window_.display();
    }
    return false;
}

void game::deleteOutOfBounds(std::pmr::vector<Entity> &entities) const {
    int i = 0;
    while (i < entities.size()) {
        if (auto &projectile = entities[i];
            projectile.sprite.getPosition().x < 0
            || projectile.sprite.getPosition().x > static_cast<float>(window_.getSize().x)
            || projectile.sprite.getPosition().y < 0
            || projectile.sprite.getPosition().y > static_cast<float>(window_.getSize().y)) {
            //found on the internet
            //destroys the last element to avoid shifting every element with vector.erase()
            entities[i] = entities.back();
            entities.pop_back();
        } else {
            ++i;
        }
    }
}
