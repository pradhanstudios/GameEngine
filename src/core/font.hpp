#ifndef FONT_HPP
#define FONT_HPP
#include <ft2build.h>
#include FT_FREETYPE_H
#include "constants.hpp"
#include "texture.hpp"

struct Font {
private:
    Texture characters[95];
public:
    Font(const char* path);
    inline void setCharacter(char character, Texture texture) {
        characters[character - 32] = texture;
    }

    inline Texture getCharacter(char character) {
        return characters[character - 32];
    }
};

#endif // FONT_HPP
