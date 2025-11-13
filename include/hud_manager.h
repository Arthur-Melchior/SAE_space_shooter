//
// Created by Kalle on 12.11.2025.
//

#ifndef SPACE_SHOOTER_HUD_MANAGER_H
#define SPACE_SHOOTER_HUD_MANAGER_H
#include <optional>

#include "SFML/Graphics/Text.hpp"


class HudManager {
public:
    std::optional<sf::Text> main_menu_title;
    std::optional<sf::Text> main_menu_play;
    std::optional<sf::Text> score_text;
    std::optional<sf::Text> game_over_text;
    std::optional<sf::Text> win_text;
    std::optional<sf::Text> debug_enemies;
    std::optional<sf::Text> debug_bullets;
};


#endif //SPACE_SHOOTER_HUD_MANAGER_H