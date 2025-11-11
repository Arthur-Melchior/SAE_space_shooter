//
// Created by Kalle on 29.10.2025.
//

#ifndef SPACE_SHOOTER_RESOURCES_MANAGER_H
#define SPACE_SHOOTER_RESOURCES_MANAGER_H
#include <string>
#include <vector>

#include "SFML/Audio.hpp"
#include "SFML/Graphics.hpp"

template<typename T>
struct Resource {
    std::string name;
    T resource;
};

class ResourceManager {
public:

    explicit ResourceManager(const std::pmr::string & assets_folder);
    ResourceManager();

    sf::Sprite *find_sprite(const std::string &);
    sf::Font font;
    sf::Music main_song;

private:
    void load_resources(const std::pmr::string &);
    std::vector<Resource<sf::Texture> > loaded_textures;
    std::vector<Resource<sf::Sprite> > loaded_sprites;
};

#endif //SPACE_SHOOTER_RESOURCES_MANAGER_H
