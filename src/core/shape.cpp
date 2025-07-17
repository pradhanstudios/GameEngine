#include "shape.hpp"

CollisionInfo Rectangle::collision(Object* other) {
    if (Rectangle* rect = dynamic_cast<Rectangle*>(other))
        return colliding(this, rect);
    if (Circle* circle = dynamic_cast<Circle*>(other))
        return colliding(this, circle);

    return CollisionInfo();
}

CollisionInfo Circle::collision(Object* other) {
    if (Rectangle* rect = dynamic_cast<Rectangle*>(other))
        return colliding(this, rect);
    if (Circle* circle = dynamic_cast<Circle*>(other))
        return colliding(this, circle);

    return CollisionInfo();
}
