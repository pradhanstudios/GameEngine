#include "shaders.hpp"

GLuint textShader = 0;

char* readFile(const char* filename) {
    char* content;
    long fsize = 0;
    FILE *file = fopen(filename, "r");
    if (file) {
        fseek(file, 0, SEEK_END);
        fsize = ftell(file);
        rewind(file);

        content = ( char* ) malloc(fsize * sizeof(char));
        fread(content, 1, fsize, file);
        fclose(file);
    }

    return content;
}

GLuint initShaders(const char* vertShader, const char* fragShader) {
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    char* vertexSource = readFile(vertShader);
    char* fragmentSource = readFile(fragShader);

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
