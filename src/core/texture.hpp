#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include "constants.hpp"

struct Texture {
    GLuint textureID;
    int width, height;

    Texture() : textureID(0), width(0), height(0) {}
    Texture(GLuint textureID, int width, int height) : textureID(textureID), width(width), height(height) {}
};

Texture loadBMPTexture(const char* filename);

#endif // TEXTURE_HPP
