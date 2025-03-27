#include "Game.hpp"
#include "TextureManager.hpp"

#include "Map.hpp"

#include "ECS/EntityComponentSystem.hpp"
#include "ECS/Components.hpp"

#include "Vector.hpp"

#include "Collision.hpp"

Map* map;
SDL_Renderer* Game::renderer = nullptr;
Manager manager;

auto& Player(manager.addEntity());
auto& wall(manager.addEntity());

SDL_Event Game::event;

std::vector<ColliderComponent*> Game::colliders;


enum GroupLabels : std::size_t{
    groupMap,
    groupPlayers,
    groupEnemies,
    groupColliders
};

Game::Game(){
}
Game::~Game(){
}

void Game::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen){
    int flags = 0;
    if(fullscreen){
        flags = SDL_WINDOW_FULLSCREEN;
    }

    if(SDL_Init(SDL_INIT_EVERYTHING) == 0){
        std::cout << "SDL initialized" << std::endl;

        window = SDL_CreateWindow(title, xpos, ypos, width, height, fullscreen ? SDL_WINDOW_FULLSCREEN : 0);
        if(window){
            std::cout << "Window created" << std::endl;
        }
        renderer = SDL_CreateRenderer(window, -1, 0);
        if(renderer){
            std::cout << "Renderer created" << std::endl;
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        }
        //if SDL successfully creates a renderer, the game will run
        isRunning = true;
    }
    else{
        // if SDL fails to initialize, the game will not run
        isRunning = false;
        std::cout << "SDL failed to initialize" << std::endl;
    }
    

    
    map = new Map();
    
    // Map::LoadMap("assets/MapTexture/TestMap.txt", 16, 16);
    
    //wall
    //wall components
    wall.addComponent<TransformComponent>(300.0f, 300.0f, 300, 20, 1);
    wall.addComponent<SpriteComponent>("assets/bird.png");
    wall.addComponent<ColliderComponent>("wall");
    //wall group
    wall.addGroup(groupMap);

    //player
    // player components
    Player.addComponent<TransformComponent>(0.0f, 0.0f, 96, 96, 1);
    Player.addComponent<SpriteComponent>("assets/PlayerTexture/Sprites/IDLE.png",10,100);
    Player.addComponent<Controller>();
    Player.addComponent<ColliderComponent>("player");
    //player group
    Player.addGroup(groupPlayers);
}

void Game::handleEvents(){
    
    SDL_PollEvent(&event);
    switch (event.type){
        case SDL_QUIT:
            isRunning = false;
            break;

        default:
            break;
    }
}

void Game::update(){
    manager.refresh();
    manager.update();
    
    for(auto cc : colliders){
        Collision::CheckCollision(Player.getComponent<ColliderComponent>(), *cc);
    }
}

auto& tiles(manager.getGroup(groupMap));
auto& players(manager.getGroup(groupPlayers));
auto& enemies(manager.getGroup(groupEnemies));
auto& colliders(manager.getGroup(groupColliders));

void Game::render(){
    SDL_RenderClear(renderer);

    for (auto& t : tiles){
        t->draw();
    }
    for (auto& p : players){
        p->draw();
    }
    for (auto& e : enemies){
        e->draw();
    }
    SDL_RenderPresent(renderer);
}

void Game::clean(){
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    std::cout<<"Game cleaned"<<std::endl;
}

void Game::addTile(int id, int x, int y){
    auto& tile(manager.addEntity());
    tile.addComponent<TileComponent>(x, y, 32, 32, id);
    tile.addComponent<ColliderComponent>("terrain");
    tile.addGroup(groupMap);
}

