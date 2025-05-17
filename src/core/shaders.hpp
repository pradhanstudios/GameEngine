#ifndef SHADER_HPP
#define SHADER_HPP
#include "constants.hpp"

char* readFile(const char* filename);
GLuint initShaders(const char* vertexShader, const char* fragShader);

inline GLuint textShader = initShaders("src/core/shaders/shader.vert", "src/core/shaders/shader.frag");

#endif // SHADER_HPP
