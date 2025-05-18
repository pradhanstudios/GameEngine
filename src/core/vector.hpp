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

struct Vector3 {
    float x, y, z;
    Vector3(float x, float y, float z) : x(x), y(y), z(z) {}
    Vector3() : x(0), y(0), z(0) {}

    Vector3 operator+(const Vector3& other) const {
        return Vector3(x + other.x, y + other.y, z + other.z);
    }

    Vector3 operator-(const Vector3& other) const {
        return Vector3(x - other.x, y - other.y, z - other.z);
    }

    Vector3 operator*(float scalar) const {
        return Vector3(x * scalar, y * scalar, z * scalar);
    }

    Vector3 operator/(float scalar) const {
        return Vector3(x / scalar, y / scalar, z / scalar);
    }

    Vector3& operator+=(const Vector3& other) {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }

    Vector3& operator-=(const Vector3& other) {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        return *this;
    }

    Vector3& operator*=(float scalar) {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        return *this;
    }

    Vector3& operator/=(float scalar) {
        x /= scalar;
        y /= scalar;
        z /= scalar;
        return *this;
    }

    bool operator==(const Vector3& other) const {
        return x == other.x && y == other.y && z == other.z;
    } 

    bool operator!=(const Vector3& other) const {
        return !operator==(other);
    }

    float distranceTo(const Vector3& other) const {
        return sqrtf((x - other.x) * (x - other.x) + (y - other.y) * (y - other.y) + (z - other.z) * (z - other.z));
    }

    std::string toString() const {
        return "Vector3(" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ")";
    }
};

#endif // VECTOR_HPP
