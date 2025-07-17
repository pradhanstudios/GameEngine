#ifndef SHADER_HPP
#define SHADER_HPP
#include "constants.hpp"
namespace Shader {
char* _read(const char* filename);
GLuint init(const char* vertexShader, const char* fragShader);

extern GLuint Text;
extern GLuint Shape;
}
#endif // SHADER_HPP
