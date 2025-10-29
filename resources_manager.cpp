//
// Created by Kalle on 29.10.2025.
//

#include "resources_manager.h"


void ResourceManager::load_sprites(const std::string &assets_folder) {
    for (const auto &file: std::filesystem::recursive_directory_iterator(assets_folder)) {
        if (file.path().extension() == ".png") {
            sf::Texture texture(file.path().string());
            loaded_textures.emplace_back(Resource(file.path().stem().string(), texture));
            sf::Texture &loaded_texture = loaded_textures.back().resource;
            sf::Sprite sprite(loaded_texture);
            loaded_sprites.emplace_back(Resource(file.path().stem().string(), sprite));
        }
    }
}

sf::Sprite *ResourceManager::find_sprite(const std::string &name) {
    for (auto& loaded_sprite: loaded_sprites) {
        if (loaded_sprite.name == name) {
            return &loaded_sprite.resource;
        }
    }
    return nullptr;
}
