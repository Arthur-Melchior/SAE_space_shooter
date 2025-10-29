//
// Created by Kalle on 23.10.2025.
//

#include "background.h"

#include <cmath>
#include <filesystem>
#include <format>

#include "SFML/Graphics/Texture.hpp"

Background::Background(sf::RenderWindow &window,
                       std::pmr::vector<Road> roads,
                       sf::Clock clock,
                       std::pmr::string assets_path,
                       sf::Sprite grass_sprite,
                       float tile_size,
                       float tiles_per_window,
                       float background_speed) : clock(clock),
                                                 assets_path(assets_path),
                                                 grass_sprite(grass_sprite),
                                                 roads(roads),
                                                 tile_size(tile_size),
                                                 tiles_per_window(tiles_per_window),
                                                 background_speed(background_speed),
                                                 distance_per_tile(window.getSize().x / tiles_per_window),
                                                 tile_scale(distance_per_tile / tile_size),
                                                 road_index(std::round(tiles_per_window / 2)),
                                                 time_elapsed(0) {
    for (auto &road: roads) {
        road.sprite.setScale({
            tile_scale,
            tile_scale
        });
    }

    grass_sprite.setScale({
        tile_scale,
        tile_scale
    });

    for (float x = 0; x < tiles_per_window; ++x) {
        background_tiles.emplace_back();
        for (float y = 0; y < tiles_per_window + 1; ++y) {
            background_tiles[x].emplace_back(grass_sprite);
            background_tiles[x][y].setPosition({
                x * distance_per_tile,
                y * distance_per_tile
            });
        }
    }
}

void Background::draw(sf::RenderWindow &window) {
    for (int x = 0; x < background_tiles.size(); ++x) {
        for (int y = 0; y < background_tiles.size(); ++y) {
            auto &tile = background_tiles[x][y];
            if (tile.getPosition().y > window.getSize().y) {
                tile.setPosition({x * distance_per_tile, -tile.getGlobalBounds().size.y});
            }

            tile.move({0, background_speed});
            window.draw(tile);
        }
    }
}

void Background::draw_road(sf::RenderWindow &window) {
    roads[0].sprite.setScale({tile_scale,tile_scale});
    roads[0].sprite.setPosition({10,10});
    window.draw(roads[0].sprite);
}
