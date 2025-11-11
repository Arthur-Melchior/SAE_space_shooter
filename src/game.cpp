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

    const std::pmr::string assets_folder = "./_assets/kenney_pixel-shmup";
    resource_manager_ = ResourceManager(assets_folder);
    state_manager_ = StateManager();
    enemy_spawner_.emplace(window_);

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
        sf::Text main_text(resource_manager_.font);
        main_text.setString("SAE SPACE SHOOTER");
        main_text.setPosition({300, 200});

        sf::Text play_text(resource_manager_.font);
        play_text.setString("PRESS ENTER TO PLAY");
        play_text.setPosition({300, 500});

        window_.draw(main_text);
        window_.draw(play_text);
        window_.display();
    }
}

bool Game::loop() {
    resource_manager_.main_song.play();
    float enemy_spawn_timer = 0;

    sf::Text score(resource_manager_.font);
    sf::Text game_over_text(resource_manager_.font);
    game_over_text.setString("game over");
    game_over_text.setPosition({300, 300});

    while (window_.isOpen()) {
        //Event logic
        while (const std::optional event = window_.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window_.close();
            if (event->is<sf::Event::KeyPressed>()) {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::X)) {
                    show_bounding_box_ = !show_bounding_box_;
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter) && game_over_) {
                    game_over_ = !game_over_;
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
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::B)) {
                player_->shoot(bullets_);
            }
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
        score.setString(std::format("score : {}", state_manager_.score));
        window_.draw(score);
        if (game_over_) {
            window_.draw(game_over_text);
        }

        //Enemies logic
        deleteOutOfBounds(enemies_);

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
        if (show_bounding_box_) {
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
        }

        //Final logic
        if (game_over_) {
            resource_manager_.main_song.pause();
            state_manager_.score = 0;
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
