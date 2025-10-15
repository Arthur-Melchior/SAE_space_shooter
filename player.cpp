#include "player.h"


bool Player::can_shoot() {
    time_elapsed += clock.restart().asSeconds();
    if (time_elapsed >= reload_speed) {
        clock.reset();
        time_elapsed = 0;
        return true;
    }

    return false;
}
