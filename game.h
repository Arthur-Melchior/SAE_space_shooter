//
// Created by Kalle on 06.11.2025.
//

#ifndef SPACE_SHOOTER_GAME_H
#define SPACE_SHOOTER_GAME_H
#include "background.h"
#include "player.h"
#include "resources_manager.h"
#include "state_manager.h"
#include "SFML/Graphics/RenderWindow.hpp"

class game {
public:
    void setup();
    bool loop();
private:
    sf::RenderWindow window_;
    std::optional<Background> background_;
    std::optional<Player> player_;
    StateManager state_manager_ = {};
    ResourceManager resource_manager_;
    std::pmr::vector<Entity> bullets_;
    std::pmr::vector<Entity> enemies_;
    std::pmr::vector<Entity> explosions_;
    sf::Clock clock_;
    bool game_over_ = false;
    bool show_bounding_box_ = false;
    void deleteOutOfBounds(std::pmr::vector<Entity>& entities) const;
};

#endif //SPACE_SHOOTER_GAME_H
