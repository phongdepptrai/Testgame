#pragma once

#include "Components.hpp"
#include "../Vector.hpp"

class TransformComponent : public Component { 
    public:
        Vector2D position;
        Vector2D velocity;
        bool direction = false;
        Uint32 collapseTimer = 800; 
        Uint32 timer = 0;
        bool collapsed = false;

        int score = 0;
        
        int height = 32;
        int width = 32;
        int scale = 1;

        int speed = 3;

        int health = 100;
        int maxHealth = 100;
        int damage = 10;

        TransformComponent(){
            position.Zero();
        }
        TransformComponent(int sc){
            position.x = 400;
            position.y = 320;
            scale = sc;
        }
        TransformComponent(float x, float y, int w, int h, int s){
            position.x = x;
            position.y = y;
            height = h;
            width = w;
            scale = s;
        }
        TransformComponent(float x, float y, int w, int h, int s, int mspeed){
            position.x = x;
            position.y = y;
            height = h;
            width = w;
            scale = s;
            speed = mspeed;
        }
        void init() override {
            velocity.Zero();
        }
        void update() override {
            position.x += velocity.x * speed;
            position.y += velocity.y * speed;
        }
        void setMaxHealth(int HP){
            maxHealth = HP;
            health = HP;
        }
        
    
};
