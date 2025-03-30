#pragma once

#include "TextureManager.hpp"
#include <string>
#include "Vector.hpp"
#include <map>
#include "ECS/EntityComponentSystem.hpp"
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>


class AssetManager {    
public:
    AssetManager(Manager * man);
    ~AssetManager();
    
    void createProjectile(Vector2D pos, Vector2D velocity,int range,int speed, int damage, std::string textureID);


    void addTexture(std::string textureID, const char* path);
    SDL_Texture* getTexture(std::string textureID);

    void addFont(std::string id, std::string path, int fontSize );
    TTF_Font* getFont(std::string id);

    void addSound(std::string id, std::string path);
    Mix_Chunk* getSound(std::string id);
    void addMusic(std::string id, std::string path);
    Mix_Music* getMusic(std::string id);

private:
    std::map<std::string, SDL_Texture*> textures;
    Manager* manager;
    std::map<std::string, TTF_Font*> fonts;
    std::map<std::string, Mix_Music*> musicTracks;
    std::map<std::string, Mix_Chunk*> sounds;
};