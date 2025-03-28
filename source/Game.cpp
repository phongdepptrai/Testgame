#include "Game.hpp"
#include "TextureManager.hpp"

#include "Map.hpp"


#include "ECS/Components.hpp"

#include "Vector.hpp"

#include "Collision.hpp"
#include "AssetManager.hpp"

#include<sstream>

Map* map;
SDL_Renderer* Game::renderer = nullptr;
Manager manager;

SDL_Rect Game::camera = {0, 0, 2000, 2000};
AssetManager* Game::assets = new AssetManager(&manager);

auto& Player(manager.addEntity());


auto& Label(manager.addEntity());

auto& startButton(manager.addEntity());
auto& settingsButton(manager.addEntity());
auto& exitButton(manager.addEntity());



SDL_Event Game::event;
bool Game::isRunning = false;

bool Game::start = false;
bool Game::menu = true;
bool Game::exit = false;
bool Game::settingsEnabled = false;


Game::Game() {
}

Game::~Game() {
}

void Game::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen, SDL_Window* window) {
    int flags = 0;
    if (fullscreen) {
        flags = SDL_WINDOW_FULLSCREEN;
    }
    if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
        std::cout << "SDL initialized" << std::endl;
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

    if (TTF_Init() == -1) {
        std::cout << "TTF failed to initialize" << std::endl;
    }
    initTextures();
    // initObject();
    
}

void Game::initTextures(){
    assets->addTexture( "player", "assets/PlayerTexture/Samurai.png");
    assets->addFont("font", "assets/Font/font.ttf", 16);
    SDL_Color color = {23,243,254,205};
    SDL_Color color2 = {233,23,254,205};
    Label.addComponent<UI>(10, 10, "Score: 0", "font",white);

    startButton.addComponent<UI>(255, 205, "Play Game", "font", white);
    settingsButton.addComponent<UI>(255, 255, "Settings", "font", mint);
    exitButton.addComponent<UI>(255, 305, "Exit", "font",white);
}

void Game::initObject(){
   
    map = new Map(4, 20);
    map->LoadMap(100, 100);
    

    Player.addComponent<TransformComponent>(1000.0f, 1200.0f, 96, 96, 3);
    Player.addComponent<SpriteComponent>("player", true);
    Player.addComponent<Controller>();
    Player.addComponent<ColliderComponent>("player");
    Player.addGroup(groupPlayers);

    assets->addTexture("projectile", "assets/Projectiles/attack/attack.png");
    // assets->addTexture("projectile", "assets/MapTexture/terrain_ss.png");

    // assets->creatProjectile(Vector2D(1100,1000),Vector2D(2,0),100,1,"projectile" );
    // assets->creatProjectile(Vector2D(1000,1200),Vector2D(2,1),200,3,"projectile");
    // assets->creatProjectile(Vector2D(1000,1200),Vector2D(2,-1),200,3,"projectile");
    // assets->creatProjectile(Vector2D(1000,1200),Vector2D(3,1),200,3,"projectile");

}

auto& tiles(manager.getGroup(Game::groupMap));
auto& players(manager.getGroup(Game::groupPlayers));
auto& enemies(manager.getGroup(Game::groupEnemies));
auto& colliders(manager.getGroup(Game::groupColliders));
auto& projectiles(manager.getGroup(Game::groupProjectiles));

void Game::handleEvents() {
    SDL_PollEvent(&event);
    switch (event.type) {
        case SDL_QUIT:
            isRunning = false;
            break;
        default:
            break;
    }
    if(menu){
        if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT){  
            if (start){ 
                std::cout<<"start"<<std::endl;
                initObject();
                menu = false;
            }
            else if (exit){ 
                std::cout<<"exit"<<std::endl;
                // game->clean(window);
                // menu = false;
            }
            else if (settingsEnabled){
                std::cout<<"settings"<<std::endl;
                    //Setting();
                // menu = false;
            }
        }
    }
}

void Game::update() {
    if(!menu){
        SDL_Rect playerCol = Player.getComponent<ColliderComponent>().collider;
        Vector2D playerPos = Player.getComponent<TransformComponent>().position;

        std::stringstream ss;
        ss << "Score: " << Player.getComponent<TransformComponent>().position.x;
        Label.getComponent<UI>().setText(ss.str());
        manager.refresh();
        manager.update();


        for (auto& c : colliders) {
            SDL_Rect cCol = c->getComponent<ColliderComponent>().collider;
            if (Collision::CheckCollision(cCol, playerCol)) {
                std::cout<<"Collision with: " << c->getComponent<ColliderComponent>().tag << std::endl;
                Player.getComponent<TransformComponent>().position = playerPos;
            }
        }
        for (auto& p : projectiles) {
            SDL_Rect pCol = p->getComponent<ColliderComponent>().collider;
            if (Collision::CheckCollision(pCol, playerCol)) {
                std::cout<<"Collision with: " << p->getComponent<ColliderComponent>().tag << std::endl;
                p->destroy();   
            }
        }
        camera.x = Player.getComponent<TransformComponent>().position.x - 320;
        camera.y = Player.getComponent<TransformComponent>().position.y - 200;

        // if (camera.x < 0)
        //     camera.x = 0;
        // if (camera.y < 0)
        //     camera.y = 0;
        // if (camera.x > camera.w)
        //     camera.x = camera.w;
        // if (camera.y > camera.h)
        //     camera.y = camera.h;
    }
    else if(menu){
        manager.refresh();
        manager.update();
        //check mouse position
        SDL_GetMouseState(&mouseX,&mouseY);
        if (mouseX>=250&&mouseX<=500&&mouseY>=200&&mouseY<250){
            start = true;
            startButton.getComponent<UI>().setColor(mint);
            startButton.getComponent<UI>().setPos(255, 205);
        }
        else {
            start = false;
            startButton.getComponent<UI>().setColor(white);
            startButton.getComponent<UI>().setPos(250, 200);
        }
        if (mouseX>=250&&mouseX<=500&&mouseY>=250&&mouseY<300){ 
            settingsEnabled=true;
            settingsButton.getComponent<UI>().setColor(mint);
            settingsButton.getComponent<UI>().setPos(255, 255);
        }
        else {
            settingsEnabled=false;
            settingsButton.getComponent<UI>().setColor(white);
            settingsButton.getComponent<UI>().setPos(250, 250);
        }


        if (mouseX>=250&&mouseX<=500&&mouseY>=300&&mouseY<=350){ 
            exit=true;
            exitButton.getComponent<UI>().setColor(mint);
            exitButton.getComponent<UI>().setPos(255, 305);
        }
        else {
            exit=false;
            exitButton.getComponent<UI>().setColor(white);
            exitButton.getComponent<UI>().setPos(250, 300);
        }
    }
};

void Game::render() {
    SDL_RenderClear(renderer);
    if(!menu){
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
        for (auto& p : projectiles) {
            p->draw();
        }
        Label.draw();
    }
    else if(menu){
        Label.draw();
        startButton.draw();
        settingsButton.draw();
        exitButton.draw();
    }
    SDL_RenderPresent(renderer);
}

void Game::clean(SDL_Window* window) {
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    std::cout << "Game cleaned" << std::endl;
}


