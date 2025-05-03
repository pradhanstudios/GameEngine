#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include "constants.hpp"

struct Texture {
    int height, width;
    GLuint textureID;

    Texture() : textureID(0), height(0), width(0) {}
};

Texture loadBMPTexture(const char* filename);

#endif // TEXTURE_HPP
