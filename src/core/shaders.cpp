#include "shaders.hpp"

namespace Shader {
GLuint Text = 0;
GLuint Shape = 0;

char* _read(const char* filename) {
    char* content;
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Failed to open file '%s'\n", filename);
        return nullptr;
    }

    fseek(file, 0, SEEK_END);
    long fsize = ftell(file);
    rewind(file);

    content = ( char* ) (malloc(fsize + 1 * sizeof(char)));
    if (!content) {
        printf("Failed to allocate memory for file '%s'\n", filename); 
        return nullptr;
    }

    size_t bytesRead = fread(content, 1, fsize, file);
    fclose(file);

    content[bytesRead] = '\0';

    // printf("filename %s:\n%s\n\n", filename, content);

    return content;
}

GLuint init(const char* vertShader, const char* fragShader) {
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    char* vertexSource = _read(vertShader);
    char* fragmentSource = _read(fragShader);

    glShaderSource(vertexShader, 1, &vertexSource, nullptr);
    glShaderSource(fragmentShader, 1, &fragmentSource, nullptr);

    glCompileShader(vertexShader);
    GLint success;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        fprintf(stderr, "Vertex shader compilation failed:\n%s\n", infoLog);
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        return 0;
    }

    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        fprintf(stderr, "Fragment shader compilation failed:\n%s\n", infoLog);
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        return 0;
    }

    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);

    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(program, 512, nullptr, infoLog);
        fprintf(stderr, "Shader program linking failed:\n%s\n", infoLog);
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        glDeleteProgram(program);
        return 0;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    free(vertexSource);
    free(fragmentSource);

    return program;
}
}
