#include "../include/game.h"

int main() {
    Game game;
    game.setup();
    game.mainMenu();
    return game.loop();
}
