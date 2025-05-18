#version 330 core
in vec2 outTexCoord;
out vec4 fragColor;
uniform sampler2D aTexture;
uniform vec3 textColor;

void main() {
    float alpha = texture(aTexture, outTexCoord).r;
    fragColor = vec4(textColor, alpha);
}
