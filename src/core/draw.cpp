#include "draw.hpp"
#include "object.hpp"
#include "shape.hpp"

void bindToShader(shader::Shader& shader, const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection, const vao::Vao& vao, float scaleX, float scaleY, GLenum textureUnit, GLuint textureID, vec3 color, bool useColor) {
    shader.set("scale", scaleX, scaleY);
    shader.setm4("model", glm::value_ptr(model));
    shader.setm4("view", glm::value_ptr(view));
    shader.setm4("projection", glm::value_ptr(projection));

    // TEXTURE OR COLOR
    if (!useColor) {
        shader.set("useTexture", true);
        shader.set("aTexture", int(textureUnit - GL_TEXTURE0));
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureID);
    }

    else {
        shader.set("color", color.x, color.y, color.z, 1.0);
        shader.set("useTexture", false);
    }

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    vao.bind();
}

void drawRectangle(Rectangle* rect) {
    bool useColor = rect->texture ? color::noUse : color::use;
    drawRectangle(rect->getCenter(), rect->width, rect->height, rect->rotation, rect->texture, shader::shape, color::white, useColor);
}

void drawRectangle(vec2 centerPosition, float width, float height, float rotation, Texture* texture, shader::Shader& shader, vec3 color, bool useColor) {
    // Setup model 
    glm::mat4 model(1.f);

    model = glm::translate(model, vec2ToVec3(centerPosition));
    model = glm::rotate(model, rotation, vec3(0.f, 0.f, 1.f));

    glm::mat4 projection = glm::ortho(0.f, float(display::width), float(display::height), 0.f, -1.f, 1.f);
    glm::mat4 view(1.f);

    // Setup drawing
    shader.enable();
    GLuint textureToBind = (texture ? texture->textureID : 0);
    bindToShader(shader, model, view, projection, vao::rectangle, width, height, GL_TEXTURE0, textureToBind, color, useColor);

    // draw
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // cleanup
    glBindTexture(GL_TEXTURE_2D, 0);
    vao::rectangle.unbind();
    shader.disable();
}

void drawCircle(Circle* circle) {
    bool useColor = circle->texture ? color::noUse : color::use;
    drawCircle(circle->position, circle->radius, circle->rotation, circle->texture, shader::shape, color::white, useColor);
}

void drawCircle(vec2 position, float radius, float rotation, Texture* texture, shader::Shader& shader, vec3 color, bool useColor) {
    // Stencil
    glEnable(GL_STENCIL_TEST);
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    
    // Make a model for the stencil
    glm::mat4 model(1.f);
    model = glm::translate(model, vec2ToVec3(position));

    glm::mat4 projection = glm::ortho(0.f, float(display::width), float(display::height), 0.f, -1.f, 1.f);
    glm::mat4 view(1.f);

    GLuint textureToBind = (texture ? texture->textureID : 0);
    shader.enable();
    bindToShader(shader, model, view, projection, vao::circle, radius, radius, GL_TEXTURE0, textureToBind, color, useColor); 

    glDrawArrays(GL_TRIANGLE_FAN, 0, display::_triangleCountCircle + 2);

    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glStencilFunc(GL_EQUAL, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

    // This is the thing it is masking
    drawRectangle(position, radius * 2, radius * 2, rotation, texture, shader, color, useColor);

    // Cleanup
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_STENCIL_TEST);
    shader.disable();
}
