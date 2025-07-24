#ifndef SHADER_HPP
#define SHADER_HPP
#include "constants.hpp"
namespace shader {
char* _read(const char* filename);
GLuint _create(const char* vertexShader, const char* fragShader);

class Shader {
    GLuint id;
public:
    Shader() : id(0u) {}
    Shader(const char* vertexShader, const char* fragShader) {
        this->id = _create(vertexShader, fragShader);
    }

    inline bool operator==(Shader& other) {
        return id == other.getID();
    }

    inline bool operator!=(Shader& other) {
        return !operator==(other);
    }

    inline GLuint getID() {
        return id;
    }

    inline void enable() {
        glUseProgram(id);
    }

    inline void disable() {
        glUseProgram(0);
    }

    inline GLint getLocation(const char* parameter) {
        // GLint location = glGetUniformLocation(id, parameter);
        // if (location == -1) { 
        //     printf("Shader::getLocation(%s) == -1\n", parameter);
        //     assert(false);
        // }
            
        return glGetUniformLocation(id, parameter);
    }

    inline void set(const char* parameter, bool v) {
        glUniform1i(getLocation(parameter), v);
    }

    inline void set(const char* parameter, float v1, float v2) {
        glUniform2f(getLocation(parameter), v1, v2);
    }
   inline void set(const char* parameter, float v1, float v2, float v3) {
        glUniform3f(getLocation(parameter), v1, v2, v3);
    }
    inline void set(const char* parameter, float v1, float v2, float v3, float v4) {
        glUniform4f(getLocation(parameter), v1, v2, v3, v4);
    }

    inline void setm4(const char* parameter, float* matrix, GLsizei count = 1, GLboolean transpose = GL_FALSE) {
        glUniformMatrix4fv(getLocation(parameter), count, transpose, matrix);
    }
};


extern Shader text;
extern Shader shape;
}
#endif // SHADER_HPP
