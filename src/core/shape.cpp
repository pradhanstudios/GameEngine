#include "shape.hpp"

void _drawShape(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection, GLuint VAO, GLsizei vertexCount, GLuint shader, GLenum primitiveType, GLenum textureUnit, bool useElements, GLuint textureID, vec3 color, bool useColor) {
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

void drawRectangleManual(vec2 centerPosition, float width, float height, float rotation, Texture* texture, GLuint shader, vec3 color, bool useColor) {
    // SETUP (move to initialization of start of program later)
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

    model = glm::translate(model, vec2ToVec3(centerPosition));
    model = glm::rotate(model, rotation, vec3(0.f, 0.f, 1.f));
    model = glm::scale(model, glm::vec3(width, height, 1.0f));

    glm::mat4 projection = glm::ortho(0.f, float(RESOLUTION_WIDTH), float(RESOLUTION_HEIGHT), 0.f, -1.f, 1.f);
    glm::mat4 view(1.f);

    // DRAW
    GLuint textureToSend = (texture ? texture->textureID : 0);
    useColor = useColor || (textureToSend == 0);
    _drawShape(model, view, projection, VAO, 6, shader, GL_TRIANGLES, GL_TEXTURE0, true, textureToSend, color, useColor);

    glBindVertexArray(0);
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void drawCircleManual(vec2 position, float radius, float rotation, Texture* texture, GLuint shader, vec3 color, bool useColor) {
    // SETUP VERTICES
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
    model = glm::translate(model, vec2ToVec3(position));

    glm::mat4 projection = glm::ortho(0.f, float(RESOLUTION_WIDTH), float(RESOLUTION_HEIGHT), 0.f, -1.f, 1.f);
    glm::mat4 view(1.f);

    GLuint textureToSend = (texture ? texture->textureID : 0);
    useColor = useColor || (textureToSend == 0);
    // This is the mask
    _drawShape(model, view, projection, VAO, (TRIANGLE_COUNT_CIRCLE + 2), shader, GL_TRIANGLE_FAN, GL_TEXTURE0, false, textureToSend, color, useColor); 

    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glStencilFunc(GL_EQUAL, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

    // This is the thing it is masking
    drawRectangleManual(vec2(position.x - radius, position.y - radius), radius * 4, radius * 4, rotation, texture, shader, color, useColor);

    glDisable(GL_STENCIL_TEST);
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

CollisionInfo Rectangle::getCollisionWith(Circle* other) {
    CollisionInfo info;
    vec2 circleCenterWorld = other->position;
    vec2 rectCenterWorld = getCenter();
    glm::mat4 rectWorldTransform = glm::translate(glm::mat4(1.f), vec2ToVec3(rectCenterWorld)) *
                                    glm::rotate(glm::mat4(1.f), rotation, glm::vec3(0.f, 0.f, 1.f));

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
