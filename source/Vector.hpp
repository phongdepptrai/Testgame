#pragma once

#include <iostream>
#include <vector>
#include <math.h>


class Vector2D{
public:
    float x, y;
    Vector2D();
    Vector2D(float x, float y);
    Vector2D& Add(const Vector2D& vec);
    Vector2D& Subtract(const Vector2D& vec);
    Vector2D& Multiply(const Vector2D& vec);
    Vector2D& Divide(const Vector2D& vec);

    friend Vector2D& operator+(Vector2D& v1, const Vector2D& v2);
    friend Vector2D& operator-(Vector2D& v1, const Vector2D& v2);
    friend Vector2D& operator*(Vector2D& v1, const Vector2D& v2);
    friend Vector2D& operator/(Vector2D& v1, const Vector2D& v2);

    Vector2D& operator+=(const Vector2D& vec);
    Vector2D& operator-=(const Vector2D& vec);
    Vector2D& operator*=(const Vector2D& vec);
    Vector2D& operator/=(const Vector2D& vec);

    friend std::ostream& operator<<(std::ostream& stream, const Vector2D& vec){
        stream << "(" << vec.x << "," << vec.y << ")";
        return stream;
    }

    Vector2D& operator*=(const int& i);
    Vector2D& Zero();

    Vector2D normalize() const {
        float magnitude = std::sqrt(x * x + y * y);
        std::cout << x/magnitude << " " << y/magnitude << std::endl;
        return (magnitude > 0) ? Vector2D(x / magnitude, y / magnitude) : Vector2D(0, 0);
    }
};