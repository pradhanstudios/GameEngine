#version 330 core
layout (location = 0) in vec2 position;
layout (location = 1) in vec2 texCoord;
out vec2 outTexCoord;

void main() {
    vec2 normalizedPosition;
    normalizedPosition.x = (position.x / float(1280)) * 2.0 - 1.0;
    normalizedPosition.y = -((position.y / float(720)) * 2.0 - 1.0);

    gl_Position = vec4(normalizedPosition, 0.0, 1.0);
    outTexCoord = texCoord;
}
