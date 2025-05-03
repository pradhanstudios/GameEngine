#include "texture.hpp"

Texture loadBMPTexture(const char* filename) {
	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* version = glGetString(GL_VERSION);

    Texture texture;

	if (!renderer || !version) {
	    printf("OpenGL context is not active\n");
	    return texture;
	}

	printf("Renderer: %s\n", renderer);
	printf("OpenGL version: %s\n", version);
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
	unsigned int imageSize = texture.width * texture.height * 3;
	printf("Width: %u, Height: %u, Image Size: %u\n", texture.width, texture.height, imageSize);

	// Allocate memory for the image data
	unsigned char* imageData = new unsigned char[imageSize];
	// Read the data
	if (fread(imageData, 1, imageSize, file) != imageSize) {
		printf("Could not read image data");
		delete[] imageData;
		fclose(file);
		return texture;
	}

	fclose(file);
	// Generate Texture ID
	glGenTextures(1, &texture.textureID);
	// Bind the Texture ID
	glBindTexture(GL_TEXTURE_2D, texture.textureID);
	// Give the image data to openGL and link it to the Texture ID 
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture.width, texture.height, 0, GL_BGR, GL_UNSIGNED_BYTE, imageData);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
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
