#include "Assetmanager.hpp"
#include "ECS/Components.hpp"
AssetManager::AssetManager(Manager * man): manager(man){
}
AssetManager::~AssetManager(){
}
void AssetManager::addTexture(std::string textureID, const char* path){
    textures.emplace(textureID, TextureManager::LoadTexture(path));
}

SDL_Texture* AssetManager::getTexture(std::string textureID){
    return textures[textureID];
}


void AssetManager::creatProjectile(Vector2D pos, Vector2D velocity,int range, int speed, std::string textureID){
    auto& projectile(manager->addEntity());
    projectile.addComponent<TransformComponent>(pos.x, pos.y, 32, 32, 1);
    projectile.addComponent<ProjectileComponent>(range, speed, velocity);
    projectile.addComponent<SpriteComponent>(textureID, false);
    projectile.addComponent<ColliderComponent>("projectile");
    projectile.addGroup(Game::groupProjectiles);

}

void AssetManager::addFont(std::string id, std::string path, int fontSize){
    fonts.emplace(id, TTF_OpenFont(path.c_str(), fontSize));

};
TTF_Font* AssetManager::getFont(std::string id){
    return fonts[id];
}