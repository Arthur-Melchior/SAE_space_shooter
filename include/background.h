//
// Created by Kalle on 23.10.2025.
//

#ifndef SPACE_SHOOTER_BACKGROUND_H
#define SPACE_SHOOTER_BACKGROUND_H
#include <string>
#include <vector>

#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/Texture.hpp"

class Background{
public:
    sf::Clock clock;

    std::pmr::string assets_path;
    sf::Sprite grass_sprite;
    std::vector<std::vector<sf::Sprite> > background_tiles;

    float tile_size;
    float tiles_per_window;
    float background_speed;

    Background(sf::RenderWindow &window, sf::Clock clock, std::pmr::string assets_path,
               sf::Sprite grass_sprite, float tile_size,
               float tiles_per_window, float background_speed);

    void draw(sf::RenderWindow &window);

private:
    float distance_per_tile;
    float tile_scale;
    float road_index;
    float time_elapsed;
};

#endif //SPACE_SHOOTER_BACKGROUND_H
