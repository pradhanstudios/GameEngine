#version 330 core
in vec2 outTexCoord;
out vec4 fragColor;
uniform sampler2D aTexture;
uniform vec4 color = vec4(1.0, 1.0, 1.0, 1.0);
uniform int useTexture = 1;

void main() {
    if (useTexture == 1) {
        fragColor = texture(aTexture, outTexCoord);
    }

    else {
        fragColor = color;
    }
}
