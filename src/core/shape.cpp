#include "shape.hpp"

void _drawRectangleV(Vector position, Texture* texture, Vector size, GLuint shader, Vector3 color) {
    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    float vertices[] = {
        position.x, position.y, 0.f, 0.f, // bottom left
        position.x + size.x, position.y, 1.f, 0.f, // bottom right
        position.x + size.x, position.y + size.y, 1.f, 1.f, // top right
        position.x, position.y + size.y, 0.f, 1.f, // top left
    };

    // upload data
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*) (2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glUseProgram(shader);

    if (texture && texture->textureID) {
        GLuint useTextureLocation = glGetUniformLocation(shader, "useTexture");
        glUniform1i(useTextureLocation, 1);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture->textureID);
        GLuint textureLocation = glGetUniformLocation(shader, "aTexture");
        glUniform1i(textureLocation, 0);

    }

    else {
        // printf("Got here from _drawRectangleV(); position %f %f\n", position.x, position.y);
        GLuint colorLocation = glGetUniformLocation(shader, "color");
        glUniform4f(colorLocation, color.x, color.y, color.z, 1.0);

        GLuint useTextureLocation = glGetUniformLocation(shader, "useTexture");
        glUniform1i(useTextureLocation, 0);
    }
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    GLubyte indices[] = {0, 1, 2, 0, 2, 3};
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, indices);

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDisable(GL_BLEND);
    glUseProgram(0);
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
