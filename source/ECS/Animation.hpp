#pragma once 

class Animation{
public:
    int index = 0;
    int frames = 0;
    int speed = 100;
    Animation() = default;
    Animation(int i, int f, int s){
        index = i;
        frames = f;
        speed = s;
    }
};