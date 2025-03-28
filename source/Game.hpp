#pragma once

#include<SDL2/SDL.h> 
#include<SDL2/SDL_image.h>
#include<SDL2/SDL_ttf.h>
#include<iostream>
#include<vector>

class ColliderComponent;
class AssetManager;

class Game{
    public:
        Game();
        ~Game();

        void init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen, SDL_Window* window);
        void initObject();
        void initTextures();
        void initMenu();

        void handleEvents();
        void update();
        
        void render();
        void clean(SDL_Window* window);

        bool running(){
            return isRunning;
        }

        static SDL_Renderer* renderer;
        static SDL_Event event;
        static bool isRunning;
        static SDL_Rect camera;
        static AssetManager* assets;
        static bool menu;
        static bool exit;
        static bool settingsEnabled; 
        static bool start;

        int mouseX, mouseY;
        SDL_Color mint = {20, 193, 225, 255};
        SDL_Color white = {12, 60, 88, 255};
        enum GroupLabels : std::size_t{
            groupMap,
            groupPlayers,
            groupEnemies,
            groupColliders,
            groupProjectiles,
            groupUI
        };
    private:
        int cnt = 0;


};
