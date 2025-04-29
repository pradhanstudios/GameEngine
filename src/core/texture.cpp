#include "texture.hpp"

GLuint loadBMPTexture(const char* filename) {
	FILE* file = fopen(filename, "rb");
	unsigned char *imageData;
	unsigned char header[54];
	unsigned int height;
	unsigned int width;
	unsigned int imageSize; // width * height * 3 because there are three color channels
	if (fread(header, 1, 54, file) != 54) { // not 54 bytes
		printf("Not a BMP file or formatted incorrectly");
		return 0;
	}

	// check if it is actually a BMP file
	if (header[0] != 'B' || header[1] != 'M') {
		printf("Not a BMP file or formatted incorrectly");
		return 0;
	}

	// Collect information from the headers
	// About *( int* ) &, we are converting the value into a char *, and then converting it to int *, and then getting the value
	width = *( int* ) &(header[0x12]);	
	height = *( int* ) &(header[0x16]);
	imageSize = width * height * 3;

	// Allocate memory for the image data
	imageData = new unsigned char[imageSize];
	// Read the data
	fread(imageData, 1, imageSize, file);
	fclose(file);
	GLuint textureID;
	// Generate Texture ID
	glGenTextures(1, &textureID);
	// Bind the Texture ID
	glBindTexture(GL_TEXTURE_2D, textureID);
	// Give the image data to openGL and link it to the Texture ID 
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, imageData);

	delete[] imageData;

	return textureID;
}
