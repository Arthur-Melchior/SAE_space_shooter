//
// Created by Kalle on 15.10.2025.
//

#ifndef SPACE_SHOOTER_PLAYER_H
#define SPACE_SHOOTER_PLAYER_H
#include "entity.h"
#include "SFML/Audio/Sound.hpp"
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/System/Clock.hpp"
#include "SFML/System/Time.hpp"

class Player {
public:
    sf::Sprite ship_sprite;
    sf::Sprite bullet_sprite;
    sf::Clock clock;
    float reload_speed;
    float time_elapsed;
    

    void move();
    bool can_shoot();
    void shoot(std::pmr::vector<Entity> &existing_bullets);

};

#endif //SPACE_SHOOTER_PLAYER_H
