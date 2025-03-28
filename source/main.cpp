#include <iostream>
#include "Game.hpp"

Game* game = nullptr;
int main(int argc, char* argv[]){
    const int FPS = 60;
    const int frameDelay = 1000 / FPS;

    Uint32 frameStart;
    int frameTime;
    SDL_Window* window = nullptr;
    game = new Game();
    bool fullscreen = false;
    window = SDL_CreateWindow("Ohio Survival",SDL_WINDOWPOS_CENTERED , SDL_WINDOWPOS_CENTERED, 1280, 640, fullscreen ? SDL_WINDOW_FULLSCREEN : 0);
    game->init("Game Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 640, false, window);
    // menu->init("assets/Font/font.ttf", "assets/MapTexture/background.jpg",window);
    // while(menu->menu_isrunning){
    //     frameStart = SDL_GetTicks();
    //     // std::cout<<"lil"<<std::endl;
 

    //     SDL_RenderClear(game->renderer);
    //     // menu->update(game);
    //     SDL_Color white = {12, 60, 88, 1};
    //     SDL_Surface* textSurface = TTF_RenderText_Solid(menu->takeFont(), "Test Text", white);
    //     SDL_Texture* texture = SDL_CreateTextureFromSurface(game->renderer, textSurface);
    //     if(texture == nullptr){
    //         std::cout<<"texture is null"<<std::endl;
    //     }
    //     SDL_Rect destRect = {100, 100, textSurface->w, textSurface->h};
    //     SDL_RenderCopy(game->renderer, texture, nullptr, &destRect);
    //     SDL_DestroyTexture(texture);
    //     SDL_FreeSurface(textSurface);
    //     // std::cout<<"test"<<std::endl;
    //     SDL_RenderPresent(game->renderer);

    //     frameTime = SDL_GetTicks() - frameStart;
    //     if(frameDelay > frameTime){
    //         SDL_Delay(frameDelay - frameTime);
    //     }
    // }

    while (game->running()){
        frameStart = SDL_GetTicks();

        game->handleEvents();
        game->update();
        game->render();
        
        frameTime = SDL_GetTicks() - frameStart;
        
        if(frameDelay > frameTime){
            SDL_Delay(frameDelay - frameTime);
        }
    }
    game->clean(window);

    return 0;
}