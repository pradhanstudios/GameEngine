#include "vao.hpp"

namespace vao {
Vao createRectangleVao() {
    // Data
    unsigned int indices[] = {0, 1, 2, 0, 2, 3};

    float vertices[] = {
        -0.5, -0.5, 0.f, 0.f, // bottom left
        0.5, -0.5, 1.f, 0.f, // bottom right
        0.5, 0.5, 1.f, 1.f, // top right
        -0.5, 0.5, 0.f, 1.f, // top left
    };

    // Creating
    GLuint vao, vbo, ebo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    // Binding
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Sending data
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*) (2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // unbinding  
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);    

    return {vao, vbo, ebo};
}

Vao createCircleVao() {
    // Data
    float vertices[(display::_triangleCountCircle + 2) * 4];
    int vertexIDX = 0;
    // Start at (0, 0) and work from there
    vertices[vertexIDX++] = 0.f; // x 
    vertices[vertexIDX++] = 0.f; // y 
    vertices[vertexIDX++] = 0.f; // dummy u 
    vertices[vertexIDX++] = 0.f; // dummy v 

    for (int i = 0; i <= display::_triangleCountCircle; i++) {
        float angle = i * math::tau / display::_triangleCountCircle;
        vertices[vertexIDX++] = cos(angle);
        vertices[vertexIDX++] = sin(angle);
        vertices[vertexIDX++] = 0.f; 
        vertices[vertexIDX++] = 0.f;
    }    

    // Creating
    GLuint vao, vbo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    // Binding
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Sending
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Unbinding
    glBindVertexArray(0); // Unbind vao
    glBindBuffer(GL_ARRAY_BUFFER, 0); // Unbind vbo

    return {vao, vbo, 0u};
}
}
