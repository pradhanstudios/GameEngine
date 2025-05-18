#ifndef SHADER_HPP
#define SHADER_HPP
#include "constants.hpp"

char* readFile(const char* filename);
GLuint initShaders(const char* vertexShader, const char* fragShader);

extern GLuint textShader;
extern GLuint imageShader;

#endif // SHADER_HPP
