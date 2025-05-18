#version 330 core
in vec2 outTexCoord;
out vec4 fragColor;
uniform sampler2D aTexture;
void main() {
    fragColor = texture(aTexture, outTexCoord);
}
