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

SDL_Rect Game::camera = {0, 0, 800, 640};

auto& Player(manager.addEntity());
auto& wall(manager.addEntity());

SDL_Event Game::event;
bool Game::isRunning = false;

Game::Game() {
}

Game::~Game() {
}

void Game::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen) {
    int flags = 0;
    if (fullscreen) {
        flags = SDL_WINDOW_FULLSCREEN;
    }

    if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
        std::cout << "SDL initialized" << std::endl;

        window = SDL_CreateWindow(title, xpos, ypos, width, height, fullscreen ? SDL_WINDOW_FULLSCREEN : 0);
        if (window) {
            std::cout << "Window created" << std::endl;
        }
        renderer = SDL_CreateRenderer(window, -1, 0);
        if (renderer) {
            std::cout << "Renderer created" << std::endl;
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        }
        isRunning = true;
    } else {
        isRunning = false;
        std::cout << "SDL failed to initialize" << std::endl;
    }

    map = new Map("assets/MapTexture/Terrain_ss.png", 3, 32);
    map->LoadMap("assets/MapTexture/map.map", 25, 20);

    Player.addComponent<TransformComponent>(400.0f, 320.0f, 96, 96, 2);
    Player.addComponent<SpriteComponent>("assets/PlayerTexture/Samurai.png", true);
    Player.addComponent<Controller>();
    Player.addComponent<ColliderComponent>("player");
    Player.addGroup(groupPlayers);
}

auto& tiles(manager.getGroup(Game::groupMap));
auto& players(manager.getGroup(Game::groupPlayers));
auto& enemies(manager.getGroup(Game::groupEnemies));
auto& colliders(manager.getGroup(Game::groupColliders));

void Game::handleEvents() {
    SDL_PollEvent(&event);
    switch (event.type) {
        case SDL_QUIT:
            isRunning = false;
            break;
        default:
            break;
    }
}

void Game::update() {
    SDL_Rect playerCol = Player.getComponent<ColliderComponent>().collider;
    Vector2D playerPos = Player.getComponent<TransformComponent>().position;

    for (auto& c : colliders) {
        SDL_Rect cCol = c->getComponent<ColliderComponent>().collider;
        if (Collision::CheckCollision(playerCol, cCol)) {
            if (c->hasComponent<ColliderComponent>()) {
                Player.getComponent<TransformComponent>().position = playerPos;
            }
        }
    }

    manager.refresh();
    manager.update();
    camera.x = Player.getComponent<TransformComponent>().position.x - 400;
    camera.y = Player.getComponent<TransformComponent>().position.y - 320;

    if (camera.x < 0)
        camera.x = 0;
    if (camera.y < 0)
        camera.y = 0;
    if (camera.x > camera.w)
        camera.x = camera.w;
    if (camera.y > camera.h)
        camera.y = camera.h;
}

void Game::render() {
    SDL_RenderClear(renderer);

    for (auto& t : tiles) {
        t->draw();
    }
    for (auto& c : colliders) {
        c->draw();
    }
    for (auto& p : players) {
        p->draw();
    }
    for (auto& e : enemies) {
        e->draw();
    }
    SDL_RenderPresent(renderer);
}

void Game::clean() {
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    std::cout << "Game cleaned" << std::endl;
}


