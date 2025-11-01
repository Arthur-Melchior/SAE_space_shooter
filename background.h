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

enum class road_direction {
    bottom,
    top,
    left,
    right,
};

struct Road {
    sf::Sprite sprite;
    road_direction road_start;
    road_direction road_end;
};

class Background{
public:
    sf::Clock clock;

    std::pmr::string assets_path;
    sf::Sprite grass_sprite;
    std::pmr::vector<Road> roads;
    std::vector<std::vector<sf::Sprite> > background_tiles;

    float tile_size;
    float tiles_per_window;
    float background_speed;

    Background(sf::RenderWindow &window, std::pmr::vector<Road> roads, sf::Clock clock, std::pmr::string assets_path,
               sf::Sprite grass_sprite, float tile_size,
               float tiles_per_window, float background_speed);

    void draw(sf::RenderWindow &window);

    void draw_road(sf::RenderWindow &window);

private:
    float distance_per_tile;
    float tile_scale;
    float road_index;
    float time_elapsed;
};

#endif //SPACE_SHOOTER_BACKGROUND_H
