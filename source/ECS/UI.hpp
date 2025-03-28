#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <string>

// #include "../AssetManager.hpp"
#include "../Game.hpp"
#include "EntityComponentSystem.hpp"

class UI : public Component {
public:
    UI(int x, int y, std::string text, std::string font, SDL_Color& color): 
        labelText(text), labelFont(font), labelColor(color) {
            position.x = x;
            position.y = y;

            setLabelText(labelText, labelFont);
        };
    ~UI(){
    };

    void setLabelText(std::string text, std::string font){
        SDL_Surface* sur = TTF_RenderText_Blended(Game::assets->getFont(font), text.c_str(), labelColor);
        labelTexture = SDL_CreateTextureFromSurface(Game::renderer, sur);
        SDL_FreeSurface(sur);

        SDL_QueryTexture(labelTexture, nullptr, nullptr, &position.w, &position.h);
    }
    void setPos(int x, int y){
        position.x = x;
        position.y = y;
    }
    void setColor(SDL_Color color){ 
        labelColor = color; 
        // setLabelText(labelText, labelFont, labelColor); 
    };
    void setText(std::string text){ 
        labelText = text; 
        // setLabelText(labelText, labelFont, labelColor);
    };
    void destroyTexture(){
        SDL_DestroyTexture(labelTexture);
    }
    void update() override{

    };
    void draw() override{
        setLabelText(labelText, labelFont);
        
        SDL_RenderCopy(Game::renderer, labelTexture, nullptr, &position);
        SDL_DestroyTexture(labelTexture);
    };

private:
    SDL_Rect position;
    std::string labelText, labelFont;
    SDL_Color labelColor;
    SDL_Texture* labelTexture;
};