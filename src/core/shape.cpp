#include "shape.hpp"

CollisionInfo Rectangle::getCollisionWith(Circle* other) {
    CollisionInfo info;
    vec2 circleCenterWorld = other->position;
    vec2 rectCenterWorld = getCenter();
    glm::mat4 rectWorldTransform = glm::translate(glm::mat4(1.f), vec2ToVec3(rectCenterWorld)) * glm::rotate(glm::mat4(1.f), rotation, glm::vec3(0.f, 0.f, 1.f));

    // Create an inverse transform
    glm::mat4 inverseTransform = glm::inverse(rectWorldTransform);

    // Make circle positions a vec4 to make matrix multiplication work
    glm::vec4 circleCenterLocalvec4 = inverseTransform * vec2ToVec4(circleCenterWorld);
    // Convert back to vec2
    vec2 circleCenterLocal = vec4ToVec2(circleCenterLocalvec4);
    // find the closes point to the circle centered on (0, 0)
    float halfWidth = width * 0.5f;
    float halfHeight = height * 0.5f;
    vec2 closestPointLocal = vec2(
        std::max(-halfWidth, std::min(circleCenterLocal.x, halfWidth)),
        std::max(-halfHeight, std::min(circleCenterLocal.y, halfHeight))
    );

    glm::vec4 closestPointWorld4 = rectWorldTransform * vec2ToVec4(closestPointLocal);
    vec2 closestPointWorld = vec4ToVec2(closestPointWorld4);

    // Find the distance and see if it is more than the radius of the circle
    float distance = glm::distance(circleCenterWorld, closestPointWorld);
    if (distance <= other->radius) {
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

std::array<vec2, 4> Rectangle::getTransformedVertices() {
    std::array<vec2, 4> transformedVertices;
    float halfWidth = width * 0.5f;
    float halfHeight = height * 0.5f;

    vec2 localCorners[4] = {
        vec2(-halfWidth,  halfHeight), // bottom left 
        vec2( halfWidth,  halfHeight), // bottom right 
        vec2( halfWidth, -halfHeight), // top right 
        vec2(-halfWidth, -halfHeight)  // top left
    };

    // Build transformation matrix for world space
    glm::mat4 transform = glm::mat4(1.0f);
    transform = glm::translate(transform, vec2ToVec3(getCenter())); // Translate to center
    transform = glm::rotate(transform, rotation, glm::vec3(0.0f, 0.0f, 1.0f)); // Apply rotation

    // Transform each local corner to world space
    for (int i = 0; i < 4; ++i) {
        glm::vec4 worldPos = transform * vec2ToVec4(localCorners[i]);
        transformedVertices[i] = vec4ToVec2(worldPos);
    }

    return transformedVertices;
}

Projection Rectangle::projectVertices(std::array<vec2, 4> vertices, vec2& axis) {
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

CollisionInfo Rectangle::getCollisionWith(Rectangle* other) {
    CollisionInfo info; 
    // Initialize to no collision
    info.collision = false;

    // 1. Get world-space vertices for both rectangles
    std::array<vec2, 4> verticesA = this->getTransformedVertices();
    std::array<vec2, 4> verticesB = other->getTransformedVertices();

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
    vec2 centerAToB = other->getCenter() - getCenter();

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

// Define the collision methods after both classes are fully declared
inline CollisionInfo Rectangle::getCollision(Object& other) {
    if (Rectangle* rect = dynamic_cast<Rectangle*>(&other)) {
        return getCollisionWith(rect);
    } 

    else if (Circle* circle = dynamic_cast<Circle*>(&other)) {
        return getCollisionWith(circle);
    }

    return CollisionInfo();
}

CollisionInfo Circle::getCollisionWith(Circle* other) {
    CollisionInfo info;
    float distance = glm::distance(position, other->position);
    if (distance <= (radius + other->radius)) {
        info.collision = true;
        info.normal = glm::normalize(position - other->position);
        if (glm::length(info.normal) == 0.f) {
            info.normal = vec2(0.f, 1.f);
        }

        float overlap = (radius + other->radius) - distance;
        info.mtv = info.normal * overlap;
    }

    return info;
}

inline CollisionInfo Circle::getCollision(Object& other) {
    if (Circle* circle = dynamic_cast<Circle*>(&other)) {
        return getCollisionWith(circle);
    } 

    else if (Rectangle* rect = dynamic_cast<Rectangle*>(&other)) {
        return rect->getCollisionWith(this);
    }

    return CollisionInfo();
}
