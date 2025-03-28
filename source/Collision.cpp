#include "Collision.hpp"
#include "ECS/Collider.hpp"

bool Collision::CheckCollision(const SDL_Rect& a,const SDL_Rect& b) {
    return a.x < b.x + b.w && a.x + a.w > b.x && a.y < b.y + b.h && a.h + a.y > b.y;
};

bool Collision::CheckCollision(const ColliderComponent& colA, const ColliderComponent& colB) {

    if( CheckCollision(colA.collider, colB.collider)){
        // std::cout<<colA.tag << " hit: " << colB.tag << std::endl;
        return true;
    }
    return false;
};
