#ifndef DRAW_HPP
#define DRAW_HPP
#include "constants.hpp"
#include "vao.hpp"
#include "vector.hpp"
#include "texture.hpp"
#include "shaders.hpp"

class Object;
class Rectangle;
class Circle;

void drawRectangle(Rectangle* rect);
void drawRectangle(vec2 centerPosition, float width, float height, float rotation, Texture* texture, shader::Shader& shader, vec3 color=color::white, bool useColor=color::noUse);
void drawCircle(Circle* circle);
void drawCircle(vec2 position, float radius, float rotation, Texture* texture, shader::Shader& shader, vec3 color=color::white, bool useColor=color::noUse);

#endif // DRAW_HPP
