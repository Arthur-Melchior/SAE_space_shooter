//
// Created by Kalle on 29.10.2025.
//

#include "resources_manager.h"

#include <format>

ResourceManager::ResourceManager(const std::pmr::string & assets_folder) {
    load_resources(assets_folder);
}

ResourceManager::ResourceManager() {
}


void ResourceManager::load_resources(const std::pmr::string &assets_folder) {
    for (const auto &file: std::filesystem::recursive_directory_iterator(assets_folder)) {
        if (file.path().extension() == ".png") {
            const sf::Texture texture(file.path().string());
            loaded_textures.emplace_back(Resource(file.path().stem().string(), texture));
        }

        if (file.path().extension() == ".ttf") {
            font = sf::Font(file.path().string());
        }
    }
    for (auto &[name, resource]: loaded_textures) {
        const sf::Sprite sprite(resource);
        loaded_sprites.emplace_back(Resource(name, sprite));
    }
}

sf::Sprite *ResourceManager::find_sprite(const std::string &name) {
    for (auto &loaded_sprite: loaded_sprites) {
        if (loaded_sprite.name == name) {
            return &loaded_sprite.resource;
        }
    }
    return nullptr;
}
