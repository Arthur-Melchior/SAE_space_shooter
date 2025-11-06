//
// Created by Kalle on 29.10.2025.
//

#ifndef SPACE_SHOOTER_RESOURCES_MANAGER_H
#define SPACE_SHOOTER_RESOURCES_MANAGER_H
#include <string>
#include <vector>

#include "SFML/Graphics/Font.hpp"
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/Texture.hpp"

template<typename T>
struct Resource {
    std::string name;
    T resource;
};

class ResourceManager {
public:
    ResourceManager(const std::pmr::string & assets_folder);
    sf::Sprite *find_sprite(const std::string &);
    sf::Font font;

private:
    void load_resources(const std::pmr::string &);
    std::vector<Resource<sf::Texture> > loaded_textures;
    std::vector<Resource<sf::Sprite> > loaded_sprites;
};

#endif //SPACE_SHOOTER_RESOURCES_MANAGER_H
