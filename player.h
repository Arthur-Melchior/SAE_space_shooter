//
// Created by Kalle on 15.10.2025.
//

#ifndef SPACE_SHOOTER_PLAYER_H
#define SPACE_SHOOTER_PLAYER_H
#include "entity.h"
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/System/Clock.hpp"
#include "SFML/System/Time.hpp"

class Player {
public:
    sf::Sprite sprite;
    sf::Clock clock;
    float reload_speed;
    float time_elapsed;

    bool can_shoot();

};

#endif //SPACE_SHOOTER_PLAYER_H
