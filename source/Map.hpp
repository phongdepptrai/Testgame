#pragma once

#include"Game.hpp"
#include<vector>
class Map{
public:
    Map(int ms, int ts);
    ~Map();

    void LoadMap( int sizeX, int sizeY);
    void addTile(int srcX, int srcY, int xpos, int ypos, SDL_Texture* text);

    void setTextures(SDL_Texture* tex, int i){ textures.at(i) = tex;};
    SDL_Texture* getTextures(int i){ return textures.at(i);};
private:  
    const char* mapFilePath;
    int mapScale, tileSize;
    int scaledSize;
    
    std::vector<SDL_Texture*> textures = std::vector<SDL_Texture*>(25);

    std::vector<std::pair<const char*, const char*>> fileMap;
    // Background Bush Sprite Paths
    std::vector<std::pair<const char*, const char*> > bbPaths = {
        {"assets\\MapTexture\\landscape\\boundary_bush.png", "load bb(): No texture"},
        {"assets\\MapTexture\\landscape\\boundary_bush_left.png" , "load bb_l(): No texture"},
        {"assets\\MapTexture\\landscape\\boundary_bush_right.png" , "load bb_r(): No texture"},
        {"assets\\MapTexture\\landscape\\boundary_bush_up.png", "load bb_u(): No texture"},
        {"assets\\MapTexture\\landscape\\boundary_bush_down.png", "load bb_d(): No texture"},
        {"assets\\MapTexture\\landscape\\boundary_bush_upleft.png" , "load bb_ul(): No texture"},
        {"assets\\MapTexture\\landscape\\boundary_bush_downleft.png" , "load bb_dl(): No texture"},
        {"assets\\MapTexture\\landscape\\boundary_bush_upright.png" , "load bb_ur(): No texture"},
        {"assets\\MapTexture\\landscape\\boundary_bush_downright.png" , "load bb_dr(): No texture"},
    };

    // Dirt and Dirt/Grass Transitional Sprite Paths
    std::vector<std::pair<const char*, const char*> > dirtPaths = {
        {"assets\\MapTexture\\landscape\\dirt.png", "load dirt(): No textures"},
        {"assets\\MapTexture\\landscape\\dirt_transleft.png", "load dirttl(): No textures"},
        {"assets\\MapTexture\\landscape\\dirt_transright.png", "load dirttr(): No textures"},
        {"assets\\MapTexture\\landscape\\dirt_transup.png", "load dirttu(): No textures"},
        {"assets\\MapTexture\\landscape\\dirt_transdown.png", "load dirttd(): No textures"},
        {"assets\\MapTexture\\landscape\\dirt_transdownright.png", "load dirttdr(): No textures"},
        {"assets\\MapTexture\\landscape\\dirt_transdownleft.png", "load dirttdl(): No textures"},
        {"assets\\MapTexture\\landscape\\dirt_transupright.png", "load dirttur(): No textures"},
        {"assets\\MapTexture\\landscape\\dirt_transupleft.png", "load dirttul(): No textures"},

    };
};

namespace world_map {
    enum collisions {
       NO_COLLISION = 0,
       BLOCK_COLLISION = 1,
       GROUND_COLLISION = 2,
       WATER_COLLISION = 3,
       BUSH_COLLISION = 4,
       JUICE_COLLISION = 5,
       GRASS_COLLISION = 6,
       BOUNDARY_BUSH_COLLISION = 7,
       BOUNDARY_BUSH_LEFT = 8,
       BOUNDARY_BUSH_RIGHT = 9,
       BOUNDARY_BUSH_UP = 10,
       BOUNDARY_BUSH_DOWN = 11,
       BOUNDARY_BUSH_DOWNRIGHT = 12,
       BOUNDARY_BUSH_DOWNLEFT = 13,
       BOUNDARY_BUSH_UPRIGHT = 14,
       BOUNDARY_BUSH_UPLEFT = 15,

       DIRT_COLLISION = 16,
       DIRT_TRANS_LEFT = 17,
       DIRT_TRANS_RIGHT = 18,
       DIRT_TRANS_UP = 19,
       DIRT_TRANS_DOWN = 20,
 
       DIRT_TRANS_UPLEFT = 21,
       DIRT_TRANS_DOWNLEFT = 22,
       DIRT_TRANS_UPRIGHT = 23,
       DIRT_TRANS_DOWNRIGHT = 24
    };
}