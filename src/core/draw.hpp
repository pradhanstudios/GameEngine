#ifndef DRAW_HPP
#define DRAW_HPP
#include "constants.hpp"
#include "vector.hpp"
#include "object.hpp"
#include "shape.hpp"

void draw(Object* obj);
void draw(Rectangle& rect);
void draw(Circle& circle);

void _drawShape(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection, GLuint VAO, GLsizei vertexCount, GLuint shader, GLenum primitiveType, GLenum textureUnit, bool useElements, GLuint textureID, vec3 color, bool useColor);
void drawRectangleManual(vec2 centerPosition, float width, float height, float rotation, Texture* texture, GLuint shader, vec3 color=WHITE, bool useColor=NO_USE_COLOR);
void drawCircleManual(vec2 position, float radius, float rotation, Texture* texture, GLuint shader, vec3 color=WHITE, bool useColor=NO_USE_COLOR);

#endif // DRAW_HPP
