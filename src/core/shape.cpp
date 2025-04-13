#include "shape.hpp"

// Define the collision methods after both classes are fully declared
inline bool Rectangle::isColliding(Object& other) {
    if (Rectangle* rect = dynamic_cast<Rectangle*>(&other)) {
        return !(position.x > rect->position.x + rect->width ||
                 position.x + width < rect->position.x ||
                 position.y > rect->position.y + rect->height ||
                 position.y + height < rect->position.y);
    } else if (Circle* circle = dynamic_cast<Circle*>(&other)) {
        float closestX = std::max(position.x, std::min(circle->position.x, position.x + width));
        float closestY = std::max(position.y, std::min(circle->position.y, position.y + height));
        return circle->position.distanceTo(Vector(closestX, closestY)) <= circle->radius;
    }

    return false;
}

inline bool Circle::isColliding(Object& other) {
    if (Circle* circle = dynamic_cast<Circle*>(&other)) {
        float distance = position.distanceTo(circle->position);
        return distance <= (radius + circle->radius);
    } else if (Rectangle* rect = dynamic_cast<Rectangle*>(&other)) {
        float closestX = std::max(rect->position.x, std::min(position.x, rect->position.x + rect->width));
        float closestY = std::max(rect->position.y, std::min(position.y, rect->position.y + rect->height));
        return position.distanceTo(Vector(closestX, closestY)) <= radius;
    }

    return false;
}