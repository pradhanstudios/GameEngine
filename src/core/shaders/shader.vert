#version 330 core
layout (location = 0) in vec2 position;
layout (location = 1) in vec2 texCoord;
out vec2 outTexCoord;

uniform vec2 scale;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    vec4 scaledPosition = vec4(position.x * scale.x, position.y * scale.y, 0.0, 1.0);
    gl_Position = projection * view * model * scaledPosition;
    outTexCoord = texCoord;
}
