//
// Created by Kalle on 06.11.2025.
//

#include "../include/game.h"

#include "../cmake-build-debug/_deps/sfml-src/src/SFML/Window/InputImpl.hpp"
#include "../include/background.h"
#include "../include/player.h"
#include "../include/resources_manager.h"
#include "../include/state_manager.h"
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Graphics/Text.hpp"


void Game::setup() {
    window_ = sf::RenderWindow(sf::VideoMode({800, 600}), "space shooter");
    window_.setFramerateLimit(60);

    const std::pmr::string assets_folder = "./_assets";
    resource_manager_ = ResourceManager(assets_folder);
    state_manager_ = StateManager();
    enemy_spawner_.emplace(window_);
    hud_manager_ = HudManager();

    clock_ = sf::Clock();
    player_ = Player(*resource_manager_.find_sprite("ship_0000"),
                     *resource_manager_.find_sprite("tile_0000"),
                     clock_,
                     BulletType::simple,
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

    hud_manager_.main_menu_play.emplace(resource_manager_.font);
    hud_manager_.main_menu_title.emplace(resource_manager_.font);
    hud_manager_.score_text.emplace(resource_manager_.font);
    hud_manager_.game_over_text.emplace(resource_manager_.font);
    hud_manager_.win_text.emplace(resource_manager_.font);
    hud_manager_.debug_enemies.emplace(resource_manager_.font);
    hud_manager_.debug_bullets.emplace(resource_manager_.font);

    hud_manager_.main_menu_title->setString("SAE SPACE SHOOTER");
    hud_manager_.main_menu_title->setPosition({300, 200});
    hud_manager_.main_menu_play->setString("PRESS ENTER TO PLAY");
    hud_manager_.main_menu_play->setPosition({300, 500});
    hud_manager_.game_over_text->setString("game over");
    hud_manager_.game_over_text->setPosition({300, 300});
    hud_manager_.win_text->setString("you won");
    hud_manager_.win_text->setPosition({300, 300});
    hud_manager_.debug_enemies->setPosition({0, 200});
    hud_manager_.debug_bullets->setPosition({0, 400});

    show_debug_info_ = false;
    game_over_ = false;
}

void Game::mainMenu() {
    bool play = false;

    while (window_.isOpen() && !play) {
        while (const std::optional event = window_.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window_.close();
            if (event->is<sf::Event::KeyPressed>()) {
                if (sf::priv::InputImpl::isKeyPressed(sf::Keyboard::Key::Enter)) {
                    play = true;
                }
            }
        }
        window_.clear(sf::Color::Black);
        window_.draw(hud_manager_.main_menu_title.value());
        window_.draw(hud_manager_.main_menu_play.value());
        window_.display();
    }
}

bool Game::loop() {
    resource_manager_.main_song.play();
    float enemy_spawn_timer = 0;
    float power_up_spawn_timer = 0;

    while (window_.isOpen()) {
        //Event logic
        while (const std::optional event = window_.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window_.close();
            if (event->is<sf::Event::KeyPressed>()) {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::X)) {
                    show_debug_info_ = !show_debug_info_;
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter) && (game_over_ || game_won_)) {
                    game_over_ = false;
                    game_won_ = false;
                    state_manager_.score = 0;
                    enemies_.clear();
                    resource_manager_.main_song.play();
                }
            }
        }

        window_.clear(sf::Color::Black);

        //Display background_
        background_->draw(window_);

        //Player logic
        if (!game_over_) {
            player_->move(window_);
            window_.draw(player_->ship_sprite);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) {
                player_->shoot(bullets_);
            }
        }

        //Power up logic
        deleteOutOfBounds(power_ups);

        power_up_spawn_timer += clock_.getElapsedTime().asSeconds();
        if (power_up_spawn_timer > 5) {
            power_ups.emplace_back(enemy_spawner_->spawnEnemy(*resource_manager_.find_sprite("tile_0070")));
            power_up_spawn_timer = 0;
        }

        for (int i = 0; i < power_ups.size(); ++i) {
            auto &power_up = power_ups[i];
            if (power_up.sprite.getGlobalBounds().findIntersection(player_->ship_sprite.getGlobalBounds())) {
                player_->current_bullet_type = static_cast<BulletType>(
                    static_cast<int>(player_->current_bullet_type) + 1);
                if (static_cast<int>(player_->current_bullet_type) > 5) {
                    player_->current_bullet_type = triple_cone;
                }
                power_ups.erase(power_ups.begin() + i);
                continue;
            }
            power_up.move();
            window_.draw(power_up.sprite);
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
                    auto explosion = Entity(*resource_manager_.find_sprite("explosion"),
                                            HorizontalDirection::None,
                                            VerticalDirection::None,
                                            0, 0,
                                            0);
                    explosion.sprite.setScale({0.1, 0.1});
                    explosion.sprite.setPosition({
                        enemy.sprite.getPosition().x - 30,
                        enemy.sprite.getPosition().y
                    });
                    explosions_.emplace_back(explosion);
                    enemies_[i] = enemies_.back();
                    enemies_.pop_back();
                }
            }

            //Check for collision with player
            if (player_->ship_sprite.getGlobalBounds().findIntersection(bullet.sprite.getGlobalBounds())) {
                game_over_ = true;
            }

            bullet.move();
            window_.draw(bullet.sprite);
        }

        //HUD logic
        hud_manager_.score_text->setString(std::format("score : {}", state_manager_.score));
        window_.draw(hud_manager_.score_text.value());
        if (game_won_) {
            window_.draw(hud_manager_.win_text.value());
            window_.draw(hud_manager_.main_menu_play.value());
        }
        if (game_over_) {
            window_.draw(hud_manager_.game_over_text.value());
            window_.draw(hud_manager_.main_menu_play.value());
        }

        //Enemies logic
        deleteOutOfBounds(enemies_);

        if (!game_won_) {
            enemy_spawn_timer += clock_.getElapsedTime().asSeconds();
            if (enemy_spawn_timer > 1) {
                enemies_.emplace_back(enemy_spawner_->spawnEnemy(*resource_manager_.find_sprite("ship_0012")));
                enemy_spawn_timer = 0;
            }

            for (auto &enemy: enemies_) {
                //check for collision with player_
                if (enemy.sprite.getGlobalBounds().findIntersection(player_->ship_sprite.getGlobalBounds())) {
                    game_over_ = true;
                }
                enemy.move();
                auto *bullet_sprite = resource_manager_.find_sprite("tile_0003");
                bullet_sprite->setRotation(sf::degrees(180));
                enemy.shoot(clock_.getElapsedTime().asSeconds(), 1.5, bullets_, *bullet_sprite);
                window_.draw(enemy.sprite);
            }
        }

        //Explosions logic
        for (int i = 0; i < explosions_.size(); ++i) {
            auto &explosion = explosions_[i];
            explosion.delta_time += clock_.getElapsedTime().asSeconds();
            if (explosion.delta_time > 0.5) {
                explosions_.erase(explosions_.begin() + i);
                continue;
            }
            window_.draw(explosion.sprite);
        }

        //Debug logic
        if (show_debug_info_) {
            for (const auto &enemy: enemies_) {
                auto sprite = enemy.sprite;
                const sf::FloatRect bounds = sprite.getGlobalBounds();

                sf::RectangleShape boundingBox;
                boundingBox.setPosition(bounds.position);
                boundingBox.setSize(bounds.size);
                boundingBox.setFillColor(sf::Color::Transparent);
                boundingBox.setOutlineColor(sf::Color::Red);
                boundingBox.setOutlineThickness(1.f);

                window_.draw(boundingBox);
            }
            for (const auto &bullet: bullets_) {
                auto sprite = bullet.sprite;
                const sf::FloatRect bounds = sprite.getGlobalBounds();

                sf::RectangleShape boundingBox;
                boundingBox.setPosition(bounds.position);
                boundingBox.setSize(bounds.size);
                boundingBox.setFillColor(sf::Color::Transparent);
                boundingBox.setOutlineColor(sf::Color::Red);
                boundingBox.setOutlineThickness(1.f);

                window_.draw(boundingBox);
            }

            hud_manager_.debug_enemies->setString(std::format("Enemies : {}", enemies_.size()));
            hud_manager_.debug_bullets->setString(std::format("Bullets : {}", bullets_.size()));

            window_.draw(hud_manager_.debug_enemies.value());
            window_.draw(hud_manager_.debug_bullets.value());
        }

        //Final logic
        if (game_over_) {
            resource_manager_.main_song.pause();
            state_manager_.score = 0;
        }
        if (state_manager_.score >= 50) {
            game_won_ = true;
            enemies_.clear();
        }
        clock_.restart();
        window_.display();
    }

    return false;
}

void Game::deleteOutOfBounds(std::pmr::vector<Entity> &entities) const {
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
