#pragma once

#include "SDL2/SDL.h"

class ColliderComponent;
class Collision{
public:
    static bool CheckCollision(const SDL_Rect& a,const SDL_Rect& b);
    static bool CheckCollision(const ColliderComponent& colA, const ColliderComponent& colB);
};