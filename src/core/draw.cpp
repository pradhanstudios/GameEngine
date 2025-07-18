#include "draw.hpp"
#include "object.hpp"
#include "shape.hpp"

void bindToShader(GLuint shader, const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection, const vao::Vao& vao, float scaleX, float scaleY, GLenum textureUnit, GLuint textureID, vec3 color, bool useColor) {
    GLint scaleLoc = glGetUniformLocation(shader, "scale");
    GLint modelLoc = glGetUniformLocation(shader, "model");
    GLint viewLoc = glGetUniformLocation(shader, "view");
    GLint projLoc = glGetUniformLocation(shader, "projection");

    glUniform2f(scaleLoc, scaleX, scaleY);
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    // TEXTURE OR COLOR
    if (!useColor) {
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

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    vao.bind();
}

void drawRectangle(Rectangle* rect) {
    bool useColor = rect->texture ? color::noUse : color::use;
    drawRectangle(rect->getCenter(), rect->width, rect->height, rect->rotation, rect->texture, shader::shape, color::white, useColor);
}

void drawRectangle(vec2 centerPosition, float width, float height, float rotation, Texture* texture, GLuint shader, vec3 color, bool useColor) {
    // Setup model 
    glm::mat4 model(1.f);

    model = glm::translate(model, vec2ToVec3(centerPosition));
    model = glm::rotate(model, rotation, vec3(0.f, 0.f, 1.f));

    glm::mat4 projection = glm::ortho(0.f, float(display::width), float(display::height), 0.f, -1.f, 1.f);
    glm::mat4 view(1.f);

    // Setup drawing
    glUseProgram(shader);
    GLuint textureToBind = (texture ? texture->textureID : 0);
    bindToShader(shader, model, view, projection, vao::rectangle, width, height, GL_TEXTURE0, textureToBind, color, useColor);

    // draw
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // cleanup
    glBindTexture(GL_TEXTURE_2D, 0);
    vao::rectangle.unbind();
    glUseProgram(0);
}

void drawCircle(Circle* circle) {
    bool useColor = circle->texture ? color::noUse : color::use;
    drawCircle(circle->position, circle->radius, circle->rotation, circle->texture, shader::shape, color::white, useColor);
}

void drawCircle(vec2 position, float radius, float rotation, Texture* texture, GLuint shader, vec3 color, bool useColor) {
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
    glUseProgram(shader);
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
    glUseProgram(shader);
}
