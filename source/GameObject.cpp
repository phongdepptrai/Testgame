#include"GameObject.hpp"
#include"TextureManager.hpp"

GameObject::GameObject(const char* texturesheet, int x, int y){
    objTexture = TextureManager::LoadTexture(texturesheet);

    int texW, texH;
    SDL_QueryTexture(objTexture, NULL, NULL, &texW, &texH);
    srcR.w = texW;
    srcR.h = texH;
    srcR.x = 0;
    srcR.y = 0;

    xpos = x;
    ypos = y;
    
    destR.w = texW / 2;
    destR.h = texH / 2;
}

void GameObject::Update(){

    destR.x = xpos++;
    destR.y = ypos++;

}

void GameObject::Render(){
    SDL_RenderCopy(Game::renderer, objTexture, &srcR, &destR);
}