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

        Texture texture = Texture(0, face->glyph->bitmap.width, face->glyph->bitmap.rows);
        glGenTextures(1, &texture.textureID);
        glBindTexture(GL_TEXTURE_2D, texture.textureID);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            texture.width,
            texture.height,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        setCharacter(c, texture);
    }

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    FT_Done_Face(face);
    FT_Done_FreeType(ft);
}

void Font::renderSentence(const char* sentence, int fontSize, Vector position, GLuint shader) {
    if (!shader) {
        printf("Font::renderSentence(); Please use a shader");
        return;
    }

    glUseProgram(shader);

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    for (int i = 0; *(sentence + i) != '\0'; i++) {
        char c = *(sentence + i);
        Texture texture = getCharacter(c);
        // printf("Character: %c, Texture id: %u\n", c, texture.textureID);

        Vector characterSize = getCharacterSize(texture, fontSize);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture.textureID);
    
        GLuint textureLocation = glGetUniformLocation(shader, "aTexture");
        glUniform1i(textureLocation, 0);

        glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 0.0f);
            glVertex2f(position.x, position.y);
            glTexCoord2f(1.0f, 0.0f);
            glVertex2f(position.x + characterSize.x, position.y);
            glTexCoord2f(1.0f, 1.0f);
            glVertex2f(position.x + characterSize.x, position.y + characterSize.y);
            glTexCoord2f(0.0f, 1.0f);
            glVertex2f(position.x, position.y + characterSize.y);
        glEnd();

        position.x += characterSize.x;
    }

    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);

}
