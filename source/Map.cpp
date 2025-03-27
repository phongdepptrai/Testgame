#include "Map.hpp"
#include "Game.hpp"
#include "ECS/EntityComponentSystem.hpp"
#include "ECS/Components.hpp"
#include <fstream>

extern Manager manager;

Map::Map(const char* mfp, int ms, int ts): mapFilePath(mfp), mapScale(ms), tileSize(ts){
    scaledSize = tileSize * mapScale;
}

Map::~Map(){
}
void Map::LoadMap(std::string path, int sizeX, int sizeY){
    char tile;
    std::fstream mapFile;
    mapFile.open(path);
    int srcX, srcY;
    for(int y = 0; y < sizeY; y++){
        for(int x = 0; x < sizeX; x++){
            mapFile.get(tile);
            srcY = atoi(&tile) * tileSize;
            mapFile.get(tile);
            srcX = atoi(&tile) * tileSize;
            addTile(srcX, srcY, x * scaledSize, y * scaledSize);
            mapFile.ignore(); //,
        }
        // mapFile.ignore(); // \n
    }

    mapFile.ignore();

    for(int y = 0; y < sizeY; y++){
        for(int x = 0; x < sizeX; x++){
            mapFile.get(tile);
            if (tile == '1'){
                auto& tcol(manager.addEntity());
                tcol.addComponent<ColliderComponent>("terrain", x * scaledSize, y * scaledSize, scaledSize);
                tcol.addGroup(Game::groupColliders);
                // mapFile.ignore();
            }
            mapFile.ignore();
        }
    }
    mapFile.close();
}
void Map::addTile(int srcX, int srcY, int xpos, int ypos){
    auto& tile1(manager.addEntity());
    tile1.addComponent<TileComponent>(srcX, srcY, xpos, ypos, tileSize, mapScale, mapFilePath);
    // tile.addComponent<ColliderComponent>("terrain");
    tile1.addGroup(Game::groupMap);
}

