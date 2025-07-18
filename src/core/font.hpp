#ifndef FONT_HPP
#define FONT_HPP
#include <ft2build.h>
#include FT_FREETYPE_H
#include "constants.hpp"
#include "shaders.hpp"
#include "vector.hpp"
#include "texture.hpp"
#include "draw.hpp"

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
    void renderSentence(const char* sentence, int fontSize, vec2 position, vec3 color=color::white, float rotation=0.f);

    inline vec2 getCharacterSize(Character character, int fontSize=48) {
        float scale = fontSize / 48.f;
        return vec2(character.width * scale, character.height * scale);
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
