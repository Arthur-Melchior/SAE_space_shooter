//
// Created by Kalle on 29.10.2025.
//

#ifndef SPACE_SHOOTER_RESOURCES_MANAGER_H
#define SPACE_SHOOTER_RESOURCES_MANAGER_H
#include <string>
#include <vector>

#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/Texture.hpp"

template<typename T>
struct Resource {
    std::string name;
    T resource;

    Resource() = default;

    Resource(std::string n, const T &res)
        : name(std::move(n)), resource(res) {
    }
};

class ResourceManager {
public:
    std::vector<Resource<sf::Texture> > loaded_textures;
    std::vector<Resource<sf::Sprite> > loaded_sprites;

    void load_sprites(const std::string &);

    sf::Sprite *find_sprite(const std::string &);
};

#endif //SPACE_SHOOTER_RESOURCES_MANAGER_H
