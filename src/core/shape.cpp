#include "shape.hpp"

void _drawShape(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection, GLuint VAO, GLsizei vertexCount, GLuint shader, GLenum primitiveType, GLenum textureUnit, bool useElements, GLuint textureID, Vector3 color, bool useColor) {
    glUseProgram(shader);
    GLint modelLoc = glGetUniformLocation(shader, "model");
    GLint viewLoc = glGetUniformLocation(shader, "view");
    GLint projLoc = glGetUniformLocation(shader, "projection");

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    // TEXTURE OR COLOR
    if (!useColor && textureID) {
        GLuint useTextureLocation = glGetUniformLocation(shader, "useTexture");
        glUniform1i(useTextureLocation, 1);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureID);
        GLuint textureLocation = glGetUniformLocation(shader, "aTexture");
        glUniform1i(textureLocation, textureUnit - GL_TEXTURE0);
    }

    else {
        GLuint colorLocation = glGetUniformLocation(shader, "color");
        glUniform4f(colorLocation, color.x, color.y, color.z, 1.0);

        GLuint useTextureLocation = glGetUniformLocation(shader, "useTexture");
        glUniform1i(useTextureLocation, 0);
    }
    
    // BLENDING
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // DRAW
    glBindVertexArray(VAO);

    if (useElements) {
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }

    else {
        glDrawArrays(primitiveType, 0, vertexCount);
    }

    // CLEANUP
    glBindTexture(GL_TEXTURE_2D, 0);
    glUseProgram(0);
}

void drawRectangleManual(Vector position, Texture* texture, int width, int height, GLuint shader, Vector3 color, bool useColor, float rotationAngleRad) {
    // SETUP (move to initialization of rectangle later)
    float halfWidth = width / 2.f;
    float halfHeight = height / 2.f;

    Vector centerPosition = Vector(position.x + halfWidth, position.y + halfHeight);

    unsigned int indices[] = {0, 1, 2, 0, 2, 3};

    float vertices[] = {
        -0.5, -0.5, 0.f, 0.f, // bottom left
        0.5, -0.5, 1.f, 0.f, // bottom right
        0.5, 0.5, 1.f, 1.f, // top right
        -0.5, 0.5, 0.f, 1.f, // top left
    };

    GLuint VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*) (2 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // unbinding since we're using them later 
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // SET UP GLM STUFF
    glm::mat4 model(1.f);

    model = glm::translate(model, glm::vec3(centerPosition.x, centerPosition.y, 0.f));
    model = glm::rotate(model, rotationAngleRad, glm::vec3(0.f, 0.f, 1.f));
    model = glm::scale(model, glm::vec3(width, height, 1.0f));

    glm::mat4 projection = glm::ortho(0.f, float(RESOLUTION_WIDTH), float(RESOLUTION_HEIGHT), 0.f, -1.f, 1.f);
    glm::mat4 view(1.f);

    // DRAW
    _drawShape(model, view, projection, VAO, 6, shader, GL_TRIANGLES, GL_TEXTURE0, true, (texture ? texture->textureID : 0), color, useColor);

    glBindVertexArray(0);
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void drawCircleManual(Vector position, Texture* texture, int radius, float rotation, GLuint shader, Vector3 color, bool useColor) {
    // SETUP VERTICES
    // (x, y, dummy u, dummy v)
    float vertices[(TRIANGLE_COUNT_CIRCLE + 2) * 4];
    int vertexIDX = 0;
    // Start at (0, 0) and work from there
    vertices[vertexIDX++] = 0.f; // x 
    vertices[vertexIDX++] = 0.f; // y 
    vertices[vertexIDX++] = 0.f; // dummy u 
    vertices[vertexIDX++] = 0.f; // dummy v 

    for (int i = 0; i <= TRIANGLE_COUNT_CIRCLE; i++) {
        float angle = i * tau / TRIANGLE_COUNT_CIRCLE;
        vertices[vertexIDX++] = radius * cos(angle); // x 
        vertices[vertexIDX++] = radius * sin(angle); // y 
        vertices[vertexIDX++] = 0.f; // dummy u 
        vertices[vertexIDX++] = 0.f; // dummy v 
    }    
    // ACTUAL SETUP (do at initialization of shape)
    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Position attribute (layout location 0 in shader)
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // Texture coordinate attribute (layout location 1 in shader - even if dummy for stencil)
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0); // Unbind VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0); // Unbind VBO
    
    // STENCIL
    glEnable(GL_STENCIL_TEST);
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    
    // make a model for the stencil
    glm::mat4 model(1.f);
    model = glm::translate(model, glm::vec3(position.x, position.y, 0.f));

    glm::mat4 projection = glm::ortho(0.f, float(RESOLUTION_WIDTH), float(RESOLUTION_HEIGHT), 0.f, -1.f, 1.f);
    glm::mat4 view(1.f);

    _drawShape(model, view, projection, VAO, (TRIANGLE_COUNT_CIRCLE + 2), shader, GL_TRIANGLE_FAN, GL_TEXTURE0, false, (texture ? texture->textureID : 0), color, useColor); 

    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glStencilFunc(GL_EQUAL, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

    drawRectangleManual(Vector(position.x - radius, position.y - radius), texture, radius * 2, radius * 2, shader, color, useColor, rotation);

    glDisable(GL_STENCIL_TEST);
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

CollisionInfo Rectangle::getCollisionWith(Circle* other) {
    CollisionInfo info;
    float halfWidth = width * 0.5f;
    float halfHeight = height * 0.5f;
    glm::vec2 circleCenterWorld = glm::vec2(other->position.x, other->position.y);
    glm::vec2 rectCenterWorld = glm::vec2(position.x + halfWidth, position.y + halfHeight);
    glm::mat4 rectWorldTransform = glm::translate(glm::mat4(1.f), glm::vec3(rectCenterWorld.x, rectCenterWorld.y, 0.f)) *
                                    glm::rotate(glm::mat4(1.f), rotation, glm::vec3(0.f, 0.f, 1.f));

    // Create an inverse transform
    glm::mat4 inverseTransform = glm::inverse(rectWorldTransform);
    // glm::mat4 inverseTransform = glm::mat4(1.f);
    // inverseTransform = glm::rotate(inverseTransform, -rotation, glm::vec3(0.f, 0.f, 1.f));
    // inverseTransform = glm::translate(inverseTransform, glm::vec3(-rectCenterWorld.x, -rectCenterWorld.y, 0.f));

    // Make circle positions a vec4 to make matrix multiplication work
    glm::vec4 circleCenterLocalvec4 = inverseTransform * glm::vec4(circleCenterWorld.x, circleCenterWorld.y, 0.f, 1.f);
    // Convert back to vec2
    glm::vec2 circleCenterLocal = glm::vec2(circleCenterLocalvec4.x, circleCenterLocalvec4.y);
    // find the closes point to the circle centered on (0, 0)
    glm::vec2 closestPointLocal = glm::vec2(
        std::max(-halfWidth, std::min(circleCenterLocal.x, halfWidth)),
        std::max(-halfHeight, std::min(circleCenterLocal.y, halfHeight))
    );

    glm::vec4 closestPointWorld4 = rectWorldTransform * glm::vec4(closestPointLocal.x, closestPointLocal.y, 0.0f, 1.0f);
    glm::vec2 closestPointWorld = glm::vec2(closestPointWorld4.x, closestPointWorld4.y);

    // Find the distance and see if it is more than the radius of the circle
    float distance = glm::distance(circleCenterWorld, closestPointWorld);
    if (distance <= other->radius) {
        info.collision = true;
        info.normal = glm::normalize(circleCenterWorld - closestPointWorld);
        if (glm::length(info.normal) == 0.f) {
            info.normal = glm::vec2(0.f, 1.f);
        }

        float overlap = other->radius - distance;
        info.mtv = info.normal * overlap;
    }

    return info;
}

std::array<glm::vec2, 4> Rectangle::getTransformedVertices() {
    std::array<glm::vec2, 4> transformedVertices;
    float halfWidth = width * 0.5f;
    float halfHeight = height * 0.5f;

    glm::vec2 localCorners[4] = {
        glm::vec2(-halfWidth,  halfHeight), // bottom left 
        glm::vec2( halfWidth,  halfHeight), // bottom right 
        glm::vec2( halfWidth, -halfHeight), // top right 
        glm::vec2(-halfWidth, -halfHeight)  // top left
    };

    // Build transformation matrix for world space
    glm::mat4 transform = glm::mat4(1.0f);
    transform = glm::translate(transform, glm::vec3(position.x + halfWidth, position.y + halfHeight, 0.0f)); // Translate to center
    transform = glm::rotate(transform, rotation, glm::vec3(0.0f, 0.0f, 1.0f)); // Apply rotation

    // Transform each local corner to world space
    for (int i = 0; i < 4; ++i) {
        glm::vec4 worldPos = transform * glm::vec4(localCorners[i].x, localCorners[i].y, 0.0f, 1.0f);
        transformedVertices[i] = glm::vec2(worldPos.x, worldPos.y);
    }

    return transformedVertices;
}

Projection Rectangle::projectVertices(std::array<glm::vec2, 4> vertices, glm::vec2& axis) {
    // Ensure axis is normalized for correct dot products
    glm::vec2 normalizedAxis = glm::normalize(axis);

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
    CollisionInfo info; // Initialize to no collision
    info.collision = false;

    // 1. Get world-space vertices for both rectangles
    std::array<glm::vec2, 4> verticesA = this->getTransformedVertices();
    std::array<glm::vec2, 4> verticesB = other->getTransformedVertices();
    // printf("Rectangle A Vertices:\n");
    // for (int i = 0; i < 4; ++i) {
    //     printf("  V%d: (%f, %f)\n", i, verticesA[i].x, verticesA[i].y);
    // }
    // printf("Rectangle B Vertices:\n");
    // for (int i = 0; i < 4; ++i) {
    //     printf("  V%d: (%f, %f)\n", i, verticesB[i].x, verticesB[i].y);
    // }

    // 2. Define the candidate axes (normals of the faces)
    glm::vec2 axes[4];

    // Axes from Rectangle A (two unique normal directions)
    glm::vec2 edgeA1 = verticesA[1] - verticesA[0]; // Example edge
    glm::vec2 axisA1 = glm::normalize(glm::vec2(-edgeA1.y, edgeA1.x)); // Perpendicular to edgeA1
    axes[0] = axisA1;

    glm::vec2 edgeA2 = verticesA[2] - verticesA[1]; // Another adjacent edge
    glm::vec2 axisA2 = glm::normalize(glm::vec2(-edgeA2.y, edgeA2.x)); // Perpendicular to edgeA2
    axes[1] = axisA2;

    // Axes from Rectangle B (two unique normal directions)
    glm::vec2 edgeB1 = verticesB[1] - verticesB[0];
    glm::vec2 axisB1 = glm::normalize(glm::vec2(-edgeB1.y, edgeB1.x));
    axes[2] = axisB1;

    glm::vec2 edgeB2 = verticesB[2] - verticesB[1];
    glm::vec2 axisB2 = glm::normalize(glm::vec2(-edgeB2.y, edgeB2.x));
    axes[3] = axisB2;

    float minOverlap = std::numeric_limits<float>::max(); // Stores the smallest overlap found
    glm::vec2 mtvAxis; // Stores the axis corresponding to the minOverlap

    // 3. Loop through all candidate axes and check for separation
    for (glm::vec2 axis : axes) {
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

    // Get centers of both rectangles (for determining relative direction)
    float halfWidth = width / 2.f;
    float halfHeight = height / 2.f;
    float otherHalfWidth = other->width / 2.f;
    float otherHalfHeight = other->height / 2.f;
    glm::vec2 centerA = glm::vec2(position.x + halfWidth, position.y + halfHeight); // Assuming position is center
    glm::vec2 centerB = glm::vec2(other->position.x + otherHalfWidth, other->position.y + otherHalfHeight);

    // Vector from A's center to B's center
    glm::vec2 centerAToB = centerB - centerA;

    // If the mtvAxis points in the same general direction as centerAToB,
    // then the MTV needs to push A *against* that direction.
    // If dot product is negative, it means mtvAxis and centerAToB are in opposite directions.
    if (glm::dot(centerAToB, mtvAxis) < 0) {
        info.mtv = mtvAxis * minOverlap; // MTV pushes A away from B
        info.normal = mtvAxis;
    } else {
        info.mtv = -mtvAxis * minOverlap; // MTV pushes A away from B
        info.normal = -mtvAxis;
    }

    return info;
}

// Define the collision methods after both classes are fully declared
inline CollisionInfo Rectangle::getCollision(Object& other) {
    if (Rectangle* rect = dynamic_cast<Rectangle*>(&other)) {
        return getCollisionWith(rect);
        // return !(position.x > rect->position.x + rect->width ||
        //          position.x + width < rect->position.x ||
        //          position.y > rect->position.y + rect->height ||
        //          position.y + height < rect->position.y);
    } 

    else if (Circle* circle = dynamic_cast<Circle*>(&other)) {
        // float closestX = std::max(position.x, std::min(circle->position.x, position.x + width));
        // float closestY = std::max(position.y, std::min(circle->position.y, position.y + height));
        // return circle->position.distanceTo(Vector(closestX, closestY)) <= circle->radius;
        return getCollisionWith(circle);
    }

    return CollisionInfo();
}

CollisionInfo Circle::getCollisionWith(Circle* other) {
    CollisionInfo info;
    float distance = position.distanceTo(other->position);
    if (distance <= (radius + other->radius)) {
        info.collision = true;
        glm::vec2 position_glm = glm::vec2(position.x, position.y);
        glm::vec2 otherpos_glm = glm::vec2(other->position.x, other->position.y);
        info.normal = glm::normalize(position_glm - otherpos_glm);
        if (glm::length(info.normal) == 0.f) {
            info.normal = glm::vec2(0.f, 1.f);
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
        // float closestX = std::max(rect->position.x, std::min(position.x, rect->position.x + rect->width));
        // float closestY = std::max(rect->position.y, std::min(position.y, rect->position.y + rect->height));
        // return position.distanceTo(Vector(closestX, closestY)) <= radius;
        return rect->getCollisionWith(this);
    }

    return CollisionInfo();
}
