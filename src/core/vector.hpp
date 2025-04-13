#ifndef VECTOR_HPP
#define VECTOR_HPP

#include "constants.hpp"

struct Vector {
    float x, y;
    Vector(float x, float y) : x(x), y(y) {}
    Vector() : x(0), y(0) {}
    Vector operator+(const Vector& other) const {
        return Vector(x + other.x, y + other.y);
    }

    Vector operator-(const Vector& other) const {
        return Vector(x - other.x, y - other.y);
    }

    Vector operator*(float scalar) const {
        return Vector(x * scalar, y * scalar);
    }

    Vector operator/(float scalar) const {
        return Vector(x / scalar, y / scalar);
    }

    bool operator!=(const Vector& other) const {
        return !(*this == other);
    }

    Vector operator-() const {
        return Vector(-x, -y);
    }

    bool operator==(const Vector& other) const {
        return x == other.x && y == other.y;
    }

    Vector& operator+=(const Vector& other) {
        x += other.x;
        y += other.y;
        return *this;
    }

    Vector& operator-=(const Vector& other) {
        x -= other.x;
        y -= other.y;
        return *this;
    }

    Vector& operator*=(float scalar) {
        x *= scalar;
        y *= scalar;
        return *this;
    }

    Vector& operator/=(float scalar) {
        x /= scalar;
        y /= scalar;
        return *this;
    }

    float distanceTo(const Vector& other) const {
        return sqrtf((x - other.x) * (x - other.x) + (y - other.y) * (y - other.y));
    }
    
    operator std::string() const {
        return "Vector(" + std::to_string(x) + ", " + std::to_string(y) + ")";
    }
};

#endif // VECTOR_HPP