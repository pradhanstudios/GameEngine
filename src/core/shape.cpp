#include "shape.hpp"

void drawRectangleManual(Vector position, Texture* texture, int width, int height, GLuint shader, Vector3 color, bool use_color) {
    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    float vertices[] = {
        position.x, position.y, 0.f, 0.f, // bottom left
        position.x + width, position.y, 1.f, 0.f, // bottom right
        position.x + width, position.y + height, 1.f, 1.f, // top right
        position.x, position.y + height, 0.f, 1.f, // top left
    };

    // upload data
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*) (2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glUseProgram(shader);

    if (!use_color && texture && texture->textureID) {
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

void drawCircleManual(Vector position, Texture* texture, int radius, GLuint shader, Vector3 color, bool use_color) {
        glClear(GL_STENCIL_BUFFER_BIT);
        glEnable(GL_STENCIL_TEST);
        glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
        glStencilFunc(GL_ALWAYS, 1, 0xFF);
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
        
        glBegin(GL_TRIANGLE_FAN);
             glVertex2f(position.x, position.y);
             for (int i = 0; i <= TRIANGLE_COUNT_CIRCLE; i++) {
                 float angle = i * tau / TRIANGLE_COUNT_CIRCLE;
                 glVertex2f(position.x + radius * cos(angle), position.y + radius * sin(angle));
             }
        glEnd();

        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
        glStencilFunc(GL_EQUAL, 1, 0xFF);
        glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
 
        drawRectangleManual(Vector(position.x - radius, position.y - radius), texture, radius * 2, radius * 2, shader, color, use_color);

        glDisable(GL_STENCIL_TEST);
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
