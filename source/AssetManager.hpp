#pragma once

#include "TextureManager.hpp"
#include <string>
#include "Vector.hpp"
#include <map>
#include "ECS/EntityComponentSystem.hpp"
#include "SDL2/SDL_ttf.h"
class AssetManager {    
public:
    AssetManager(Manager * man);
    ~AssetManager();
    
    void creatProjectile(Vector2D pos, Vector2D velocity,int range, int speed, std::string textureID);


    //texture management
    void addTexture(std::string textureID, const char* path);
    SDL_Texture* getTexture(std::string textureID);

    void addFont(std::string id, std::string path, int fontSize );
    TTF_Font* getFont(std::string id);
private:
    std::map<std::string, SDL_Texture*> textures;
    Manager* manager;
    std::map<std::string, TTF_Font*> fonts;
};