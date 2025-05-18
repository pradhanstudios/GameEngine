#include "texture.hpp"

Texture loadBMPTexture(const char* filename) {
    Texture texture;
	FILE* file = fopen(filename, "rb");

	if (!file) {
		printf("Could not open file %s", filename);
		return texture;
	}

	unsigned char header[54];

	if (fread(header, 1, 54, file) != 54) { // not 54 bytes
		printf("Not a BMP file or formatted incorrectly");
		return texture;
	}

	// check if it is actually a BMP file
	if (header[0] != 'B' || header[1] != 'M') {
		printf("Not a BMP file or formatted incorrectly");
		return texture;
	}

	// Collect information from the headers
	// About *( int* ) &, we are converting the value into a char *, and then converting it to int *, and then getting the value
	texture.width = *( int* ) &(header[0x12]);	
	texture.height = *( int* ) &(header[0x16]);

    int rowSize = ((texture.width * 3) + 3) & (~3);
    int unpaddedRowSize = texture.width * 3; // Size without padding

    // Allocate memory for image data without padding
    unsigned char* imageData = new unsigned char[texture.width * texture.height * 3];
    unsigned char* tempRow = new unsigned char[rowSize]; // Temporary buffer for reading

    // Read and process each row
    for (int i = 0; i < texture.height; i++) {
        // BMPs are stored bottom-to-top, so flip the Y coordinate
        int targetRow = texture.height - 1 - i;
        
        // Read the padded row into the temp buffer
        fread(tempRow, 1, rowSize, file);
        
        // Copy just the pixel data (without padding) to the image data
        memcpy(imageData + targetRow * unpaddedRowSize, tempRow, unpaddedRowSize);
    }

    // Clean up the temp buffer
    delete[] tempRow;
    fclose(file);
	// Generate Texture ID
	glGenTextures(1, &texture.textureID);
	// Bind the Texture ID
	glBindTexture(GL_TEXTURE_2D, texture.textureID);
	// Give the image data to openGL and link it to the Texture ID 
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture.width, texture.height, 0, GL_BGR, GL_UNSIGNED_BYTE, imageData);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	delete[] imageData;

	if (glGetError() != GL_NO_ERROR) {
		printf("Error loading texture");
		glDeleteTextures(1, &texture.textureID);
		return texture;
	}

	return texture;
}
