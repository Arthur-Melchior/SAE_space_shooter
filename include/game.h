//
// Created by Kalle on 06.11.2025.
//

#ifndef SPACE_SHOOTER_GAME_H
#define SPACE_SHOOTER_GAME_H
#include "background.h"
#include "enemy_spawner.h"
#include "hud_manager.h"
#include "player.h"
#include "resources_manager.h"
#include "state_manager.h"
#include "SFML/Graphics/RenderWindow.hpp"

class Game {
public:
    void setup();
    void mainMenu();
    bool loop();
private:
    sf::RenderWindow window_;
    StateManager state_manager_ = {};
    ResourceManager resource_manager_;
    HudManager hud_manager_;
    std::pmr::vector<Entity> bullets_;
    std::pmr::vector<Entity> enemies_;
    std::pmr::vector<Entity> explosions_;
    std::pmr::vector<Entity> power_ups;
    sf::Clock clock_;
    bool game_over_ = false;
    bool game_won_ = false;
    bool show_debug_info_ = false;

    //Optional because c++ complains about constructor otherwise
    std::optional<Background> background_;
    std::optional<Player> player_;
    std::optional<EnemySpawner> enemy_spawner_;

    void deleteOutOfBounds(std::pmr::vector<Entity>& entities) const;
};

#endif //SPACE_SHOOTER_GAME_H
