#include "collision.hpp"

struct Projection {
    float min, max;
};

std::array<vec2, 4> getTransformedVertices(Rectangle* rect) {
    std::array<vec2, 4> transformedVertices;
    float halfWidth = rect->width * 0.5f;
    float halfHeight = rect->height * 0.5f;

    vec2 localCorners[4] = {
        vec2(-halfWidth,  halfHeight), // bottom left 
        vec2( halfWidth,  halfHeight), // bottom right 
        vec2( halfWidth, -halfHeight), // top right 
        vec2(-halfWidth, -halfHeight)  // top left
    };

    // Build transformation matrix for world space
    glm::mat4 transform = glm::mat4(1.0f);
    transform = glm::translate(transform, vec2ToVec3(rect->getCenter())); // Translate to center
    transform = glm::rotate(transform, rotation, glm::vec3(0.0f, 0.0f, 1.0f)); // Apply rotation

    // Transform each local corner to world space
    for (int i = 0; i < 4; ++i) {
        glm::vec4 worldPos = transform * vec2ToVec4(localCorners[i]);
        transformedVertices[i] = vec4ToVec2(worldPos);
    }

    return transformedVertices;
}

Projection projectVertices(std::array<vec2, 4> vertices, vec2 axis) {
    vec2 normalizedAxis = glm::normalize(axis);

    float min = glm::dot(vertices[0], normalizedAxis);
    float max = min;

    for (size_t i = 1; i < 4; ++i) {
        float p = glm::dot(vertices[i], normalizedAxis);
        if (p < min) {
            min = p;
        } else if (p > max) {
            max = p;
        }
    }

    return {min, max};
}

CollisionInfo colliding(Rectangle* rect1, Rectangle* rect2) {
    CollisionInfo info; 
    // Initialize to no collision
    info.collision = false;

    // 1. Get world-space vertices for both rectangles
    std::array<vec2, 4> verticesA = getTransformedVertices(rect1);
    std::array<vec2, 4> verticesB = getTransformedVertices(rect2);

    // 2. Define the candidate axes (normals of the faces)
    vec2 axes[4];

    // Axes from Rectangle A
    vec2 edgeA1 = verticesA[1] - verticesA[0];
    vec2 axisA1 = glm::normalize(vec2(-edgeA1.y, edgeA1.x)); // Perpendicular to edgeA1
    axes[0] = axisA1;

    vec2 edgeA2 = verticesA[2] - verticesA[1]; // Another adjacent edge
    vec2 axisA2 = glm::normalize(vec2(-edgeA2.y, edgeA2.x)); // Perpendicular to edgeA2
    axes[1] = axisA2;

    // Axes from Rectangle B (two unique normal directions)
    vec2 edgeB1 = verticesB[1] - verticesB[0];
    vec2 axisB1 = glm::normalize(vec2(-edgeB1.y, edgeB1.x));
    axes[2] = axisB1;

    vec2 edgeB2 = verticesB[2] - verticesB[1];
    vec2 axisB2 = glm::normalize(vec2(-edgeB2.y, edgeB2.x));
    axes[3] = axisB2;

    float minOverlap = std::numeric_limits<float>::max();
    vec2 mtvAxis; // Stores the axis corresponding to the minOverlap

    // 3. Loop through all candidate axes and check for separation
    for (vec2 axis : axes) {
        // Project both rectangles onto the current axis
        Projection pA = projectVertices(verticesA, axis);
        Projection pB = projectVertices(verticesB, axis);

        // Check for overlap on this axis
        if (pA.max < pB.min || pB.max < pA.min) {
            info.collision = false;
            return info;
        }

        float overlap = std::min(pA.max, pB.max) - std::max(pA.min, pB.min);

        // Keep track of the minimum overlap found and its corresponding axis
        if (overlap < minOverlap) {
            minOverlap = overlap;
            mtvAxis = axis;
        }
    }

    info.collision = true;

    // vec2 from A's center to B's center
    vec2 centerAToB = rect2->getCenter() - rect1->getCenter();

    // If the mtvAxis points in the same general direction as centerAToB,
    // then the MTV needs to push A against that direction.
    // If dot product is negative, it means mtvAxis and centerAToB are in opposite directions.
    if (glm::dot(centerAToB, mtvAxis) < 0) {
        info.mtv = mtvAxis * minOverlap;
        info.normal = mtvAxis;
    } else {
        info.mtv = -mtvAxis * minOverlap;
        info.normal = -mtvAxis;
    }

    return info;
}

CollisionInfo colliding(Rectangle* rect, Circle* circle) {
    CollisionInfo info;
    vec2 circleCenterWorld = circle->position;
    vec2 rectCenterWorld = rect->getCenter();
    glm::mat4 rectWorldTransform = glm::translate(glm::mat4(1.f), vec2ToVec3(rectCenterWorld)) * glm::rotate(glm::mat4(1.f), rotation, vec001);

    // Create an inverse transform
    glm::mat4 inverseTransform = glm::inverse(rectWorldTransform);

    // Make circle positions a vec4 to make matrix multiplication work
    glm::vec4 circleCenterLocalvec4 = inverseTransform * vec2ToVec4(circleCenterWorld);
    // Convert back to vec2
    vec2 circleCenterLocal = vec4ToVec2(circleCenterLocalvec4);
    // find the closes point to the circle centered on (0, 0)
    float halfWidth = rect->width * 0.5f;
    float halfHeight = rect->height * 0.5f;
    vec2 closestPointLocal = vec2(
        std::max(-halfWidth, std::min(circleCenterLocal.x, halfWidth)),
        std::max(-halfHeight, std::min(circleCenterLocal.y, halfHeight))
    );

    glm::vec4 closestPointWorld4 = rectWorldTransform * vec2ToVec4(closestPointLocal);
    vec2 closestPointWorld = vec4ToVec2(closestPointWorld4);

    // Find the distance and see if it is more than the radius of the circle
    float distance = glm::distance(circleCenterWorld, closestPointWorld);
    if (distance <= circle->radius) {
        info.collision = true;
        info.normal = glm::normalize(circleCenterWorld - closestPointWorld);

        if (glm::length(info.normal) == 0.f) {
            info.normal = vec2(0.f, 1.f);
        }

        float overlap = other->radius - distance;
        info.mtv = info.normal * overlap;
    }

    return info;
}

CollisionInfo colliding(Circle* circle1, Circle* circle2) {
    CollisionInfo info;
    float distance = glm::distance(circle1->position, circle2->position);
    if (distance <= (circle1->radius + circle2->radius)) {
        info.collision = true;
        info.normal = glm::normalize(circle1->position - circle2->position);
        if (glm::length(info.normal) == 0.f) {
            info.normal = vec2(0.f, 1.f);
        }

        float overlap = (circle1->radius + circle2->radius) - distance;
        info.mtv = info.normal * overlap;
    }

    return info;
}

CollisionInfo colliding(Circle* circle, Rectangle* rect) {
    return colliding(rect, circle);
}

virtual void Collision::applyCollisions(vec2& position, vec2& velocity, vec2& acceleration) {
        if (!info.collision) {
            return;
        } 
        
        position += info.mtv;
        float velocityDotNormal = glm::dot(velocity, info.normal);

        if (velocityDotNormal < 0.f) {
            vec2 normalVelocityComponent = info.normal * velocityDotNormal;
            vec2 tangentialVelocityComponent = velocity - normalVelocityComponent;
            const float bounce = 0.f;
            normalVelocityComponent = -normalVelocityComponent * bounce; 
            tangentialVelocityComponent *= (1.f - friction);
            velocity = normalVelocityComponent + tangentialVelocityComponent;
        }

        // float accelerationDotNormal = glm::dot(acceleration, info.normal);

        // Later implement more advanced accleration system
        if (glm::dot(info.normal, vec2(0.0f, 1.0f)) > 0.7f) { // Normal is pointing mostly upwards (like a floor)
            if (acceleration.y < 0) { 
                acceleration.y = 0; 
            }

            // If the object's vertical velocity is very small after collision,
            if (glm::abs(velocity.y) < 0.1f) { // Threshold for "stopped vertically"
                velocity.y = 0;
            }
        }

        if (glm::dot(info.normal, vec2(0.0f, -1.0f)) > 0.7f) {
            if (acceleration.y > 0) { // If acceleration is pulling upwards (e.g. anti-gravity)
                acceleration.y = 0;
            }

            if (glm::abs(velocity.y) < 0.1f) {
                velocity.y = 0;
            }
        }
    }
