#ifndef FONT_HPP
#define FONT_HPP
#include <ft2build.h>
#include FT_FREETYPE_H
#include "constants.hpp"
#include "vector.hpp"
#include "texture.hpp"
#include "shape.hpp"

class Character : public Texture {
public:
    int bearingX, bearingY, advance;
    Character(GLuint textureID=0, int width=0, int height=0, int bearingX=0, int bearingY=0, int advance=0) : Texture(textureID, width, height), bearingX(bearingX), bearingY(bearingY), advance(advance) {}   
};

struct Font {
private:
    Character characters[95];
public:
    Font(const char* path);
    void renderSentence(const char* sentence, int fontSize, Vector position, GLuint shader);

    inline Vector getCharacterSize(Character character, int fontSize=48) {
        float scale = fontSize / 48.f;
        return Vector(character.width * scale, character.height * scale);
    }

    inline void setCharacter(char character, Character texture) {
        characters[character - 32] = texture;
    }

    inline Character getCharacter(char character) {
        return characters[character - 32];
    }
};

inline Font* roboto;

#endif // FONT_HPP
