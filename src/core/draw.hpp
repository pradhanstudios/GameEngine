#ifndef DRAW_HPP
#define DRAW_HPP
#include "constants.hpp"
#include "vector.hpp"
#include "texture.hpp"

class Object;
class Rectangle;
class Circle;

void _drawShape(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection, GLuint VAO, GLsizei vertexCount, GLuint shader, GLenum primitiveType, GLenum textureUnit, bool useElements, GLuint textureID, vec3 color, bool useColor);
void drawRectangle(Rectangle* rect);
void drawRectangle(vec2 centerPosition, float width, float height, float rotation, Texture* texture, GLuint shader, vec3 color=color::white, bool useColor=color::noUse);
void drawCircle(Circle* circle);
void drawCircle(vec2 position, float radius, float rotation, Texture* texture, GLuint shader, vec3 color=color::white, bool useColor=color::noUse);

#endif // DRAW_HPP
