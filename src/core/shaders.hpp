#ifndef SHADER_HPP
#define SHADER_HPP
#include "constants.hpp"
namespace shader {
char* _read(const char* filename);
GLuint init(const char* vertexShader, const char* fragShader);

extern GLuint text;
extern GLuint shape;
}
#endif // SHADER_HPP
