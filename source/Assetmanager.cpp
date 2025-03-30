#include "Assetmanager.hpp"
#include "ECS/Components.hpp"


AssetManager::AssetManager(Manager * man): manager(man){
}

AssetManager::~AssetManager(){
}

void AssetManager::addTexture(std::string textureID, const char* path){
    SDL_Texture* tex = TextureManager::LoadTexture(path);
    if(tex == nullptr){
        std::cout<<textureID<<"'s texture failed to load"<<std::endl;
    }
    textures.emplace(textureID, tex);
}

SDL_Texture* AssetManager::getTexture(std::string textureID){
    return textures[textureID];
}

void AssetManager::createProjectile(Vector2D pos, Vector2D velocity, int range, int speed, int damage, std::string textureID){
    auto& projectile(manager->addEntity());
    projectile.addComponent<TransformComponent>(pos.x, pos.y, 256, 256, 1, speed);
    projectile.addComponent<ProjectileComponent>(range, speed, velocity, damage);
    projectile.addComponent<SpriteComponent>(textureID, true);
    projectile.addComponent<ColliderComponent>(textureID);
    projectile.addGroup(Game::groupProjectiles);
}

void AssetManager::addFont(std::string id, std::string path, int fontSize){
    fonts.emplace(id, TTF_OpenFont(path.c_str(), fontSize));
}

TTF_Font* AssetManager::getFont(std::string id){
    return fonts[id];
}

// Add a sound effect
void AssetManager::addSound(std::string id, std::string path) {
    Mix_Chunk* sound = Mix_LoadWAV(path.c_str());
    if (sound == nullptr) {
        SDL_Log("Failed to load sound: %s, SDL_mixer Error: %s", path.c_str(), Mix_GetError());
    } else {
        sounds.emplace(id, sound);
    }
}

// Retrieve a sound effect
Mix_Chunk* AssetManager::getSound(std::string id) {
    return sounds[id];
}

// Add music
void AssetManager::addMusic(std::string id, std::string path) {
    Mix_Music* music = Mix_LoadMUS(path.c_str());
    if (music == nullptr) {
        SDL_Log("Failed to load music: %s, SDL_mixer Error: %s", path.c_str(), Mix_GetError());
    } else {
        musicTracks.emplace(id, music);
    }
}

// Retrieve music
Mix_Music* AssetManager::getMusic(std::string id) {
    return musicTracks[id];
}

