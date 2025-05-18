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

void Font::renderSentence(const char* sentence, int fontSize, Vector position, GLuint shader, Vector3 color) {
    if (!shader) {
        printf("Font::renderSentence(); Please use a shader\n");
        return;
    }

    glUseProgram(shader);

    // set color
    GLuint colorLocation = glGetUniformLocation(shader, "textColor");
    glUniform3f(colorLocation, color.x, color.y, color.z);

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    for (int i = 0; *(sentence + i) != '\0'; i++) {
        char c = *(sentence + i);
        Character character = getCharacter(c);
        // printf("Character: %c, Texture id: %u\n", c, texture.textureID);

        float scale = fontSize / 48.f;
        Vector characterAdjustedSize = getCharacterSize(character, fontSize);
        Vector characterAdjustedPosition = Vector(position.x + character.bearingX * scale, (position.y + fontSize) - character.bearingY * scale);
        // printf("Character '%c' width %i height %i BearingX %i BearingY %i\n", c, character.width, character.height, character.bearingX, character.bearingY);


        _drawRectangleV(characterAdjustedPosition, character, characterAdjustedSize, shader);

        // glActiveTexture(GL_TEXTURE0);
        // glBindTexture(GL_TEXTURE_2D, texture.textureID);
        //
        // GLuint textureLocation = glGetUniformLocation(shader, "aTexture");
        // glUniform1i(textureLocation, 0);
        //
        // glBegin(GL_QUADS);
        //     glTexCoord2f(0.0f, 0.0f);
        //     glVertex2f(position.x, position.y);
        //     glTexCoord2f(1.0f, 0.0f);
        //     glVertex2f(position.x + characterSize.x, position.y);
        //     glTexCoord2f(1.0f, 1.0f);
        //     glVertex2f(position.x + characterSize.x, position.y + characterSize.y);
        //     glTexCoord2f(0.0f, 1.0f);
        //     glVertex2f(position.x, position.y + characterSize.y);
        // glEnd();

        position.x += character.advance * scale;
    }

    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);

}
