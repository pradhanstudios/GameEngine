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

void drawCircleManual(Vector position, Texture* texture, int radius, GLuint shader, Vector3 color, bool useColor) {
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

    drawRectangleManual(Vector(position.x - radius, position.y - radius), texture, radius * 2, radius * 2, shader, color, useColor);

    glDisable(GL_STENCIL_TEST);
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}


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
