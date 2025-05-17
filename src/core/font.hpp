#ifndef FONT_HPP
#define FONT_HPP
#include <ft2build.h>
#include FT_FREETYPE_H
#include "constants.hpp"
#include "vector.hpp"
#include "texture.hpp"

struct Font {
private:
    Texture characters[95];
public:
    Font(const char* path);
    void renderSentence(const char* sentence, int fontSize, Vector position, GLuint shader);

    inline Vector getCharacterSize(Texture character, int fontSize=48) {
        float scale = fontSize / 48.f;
        return Vector(character.width * scale, character.height * scale);
    }

    inline void setCharacter(char character, Texture texture) {
        characters[character - 32] = texture;
    }

    inline Texture getCharacter(char character) {
        return characters[character - 32];
    }
};

inline Font roboto = Font("assets/Roboto.ttf");

#endif // FONT_HPP
