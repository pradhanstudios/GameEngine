#include "font.hpp" 

Font::Font(const char* path) {
    FT_Library ft;
    if (FT_Init_FreeType(&ft)) {
        printf("Could not init freetype\n");
        return;
    }

    FT_Face face;
    if (FT_New_Face(ft, path, 0, &face)) {
        printf("Failed to load font\n");
        return;
    }

    FT_Set_Pixel_Sizes(face, 0, 48);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for (unsigned char c = 32; c < 127; c++) {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            printf("Could not load character '%c'. continuing\n", c);
            continue;
        }

        Character character = Character(0, face->glyph->bitmap.width, face->glyph->bitmap.rows, face->glyph->bitmap_left, face->glyph->bitmap_top, face->glyph->advance.x >> 6);
        glGenTextures(1, &character.textureID);
        glBindTexture(GL_TEXTURE_2D, character.textureID);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            character.width,
            character.height,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        setCharacter(c, character);
    }

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    FT_Done_Face(face);
    FT_Done_FreeType(ft);
}

void Font::renderSentence(const char* sentence, int fontSize, vec2 position, vec3 color, float rotation) {
    glUseProgram(Shader::Text);

    // set color
    GLuint colorLocation = glGetUniformLocation(Shader::Text, "textColor");
    glUniform3f(colorLocation, color.x, color.y, color.z);

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    for (int i = 0; *(sentence + i) != '\0'; i++) {
        char c = *(sentence + i);
        Character character = getCharacter(c);

        float scale = fontSize / 48.f;
        vec2 characterAdjustedSize = getCharacterSize(character, fontSize);
        vec2 characterAdjustedPosition = vec2(position.x + character.bearingX * scale, (position.y + fontSize) - character.bearingY * scale);

        drawRectangle(characterAdjustedPosition + characterAdjustedSize * 0.5f, characterAdjustedSize.x, characterAdjustedSize.y, rotation, static_cast<Texture*>(&character), Shader::Text);
        position.x += character.advance * scale;
    }

    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);
    glUseProgram(0);
}
