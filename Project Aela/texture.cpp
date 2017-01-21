#include <stdio.h>
// #include <stdlib.h>
#include <string>
#include <fstream>
#include "ErrorHandler.h"
#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif

#include <GL/glew.h>

#define FOURCC_DXT1 0x31545844 // Equivalent to "DXT1" in ASCII
#define FOURCC_DXT3 0x33545844 // Equivalent to "DXT3" in ASCII
#define FOURCC_DXT5 0x35545844 // Equivalent to "DXT5" in ASCII

GLuint loadDDS(std::string filePath) {
	// This actually reads the file.
	std::ifstream in(filePath);
	std::string lines;

	if (in.is_open()) {
		// I'm not done this part.
	} else {
		AelaErrorHandling::windowError("Aela DDS Texture Loader", "The program tried to load a non-existent DDS file.");
		return false;
	}

	unsigned char fileHeader[124];
	FILE * imageFile;

	// This will try to open the DDS file.
	fopen_s(&imageFile, filePath.c_str(), "rb");
	if (imageFile == NULL) {
		AelaErrorHandling::windowError("Aela DDS Loader", "A DDS file that the program tried to read was not found.");
		return 0;
	}

	// This will check the file to see if it begins with "DDS ".
	// If this file header does not exist, the file is not a DDS file.
	char DDSText[4];
	fread(DDSText, 1, 4, imageFile);
	if (strncmp(DDSText, "DDS ", 4) != 0) {
		fclose(imageFile);
		return 0;
	}

	// This will recieve the rest of the header.
	fread(&fileHeader, 124, 1, imageFile);

	unsigned int imageHeight = *(unsigned int*) &(fileHeader[8]);
	unsigned int imageWidth = *(unsigned int*) &(fileHeader[12]);
	unsigned int linearSize = *(unsigned int*) &(fileHeader[16]);
	unsigned int mipMapAmount = *(unsigned int*) &(fileHeader[24]);
	unsigned int fourCCType = *(unsigned int*) &(fileHeader[80]);
	unsigned char * buffer;
	unsigned int bufferSize;

	// This checks to see how large the bufferSize will be, including all mip-maps.
	bufferSize = mipMapAmount > 1 ? linearSize * 2 : linearSize;

	// This allocates memory for the buffer using malloc() and reads the rest of the file.
	buffer = (unsigned char*) malloc(bufferSize * sizeof(unsigned char));
	fread(buffer, 1, bufferSize, imageFile);

	// This closes the file.
	fclose(imageFile);

	// This finds the proper format.
	unsigned int format;
	switch (fourCCType) {
	case FOURCC_DXT1:
		format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
		break;
	case FOURCC_DXT3:
		format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
		break;
	case FOURCC_DXT5:
		format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
		break;
	default:
		free(buffer);
		return 0;
	}

	// This creates an OpenGL texture.
	GLuint textureID;
	glGenTextures(1, &textureID);

	// This tells openGL that future functions will reference this texture.
	glBindTexture(GL_TEXTURE_2D, textureID);

	// This sets the pixel atorage mode.
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	unsigned int blockSize = (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;
	unsigned int offset = 0;

	// This loads the mipmaps.
	for (unsigned int level = 0; level < mipMapAmount && (imageWidth || imageHeight); level++) {
		unsigned int size = ((imageWidth + 3) / 4) * ((imageHeight + 3) / 4) * blockSize;
		glCompressedTexImage2D(GL_TEXTURE_2D, level, format, imageWidth, imageHeight, 0, size, buffer + offset);
		offset += size;
		imageWidth /= 2;
		imageHeight /= 2;

		// If these if statements are triggered, the textures do not have power of two dimensions. Eww.
		if (imageWidth < 1) {
			imageWidth = 1;
		}
		if (imageHeight < 1) {
			imageHeight = 1;
		}

	}
	free(buffer);
	return textureID;
}

GLuint loadDDSOld(std::string filePath) {
	unsigned char fileHeader[124];
	FILE * imageFile;

	// This will try to open the DDS file.
	fopen_s(&imageFile, filePath.c_str(), "rb");
	if (imageFile == NULL) {
		AelaErrorHandling::windowError("Aela DDS Loader", "A DDS file that the program tried to read was not found.");
		return 0;
	}

	// This will check the file to see if it begins with "DDS ".
	// If this file header does not exist, the file is not a DDS file.
	char DDSText[4];
	fread(DDSText, 1, 4, imageFile);
	if (strncmp(DDSText, "DDS ", 4) != 0) {
		fclose(imageFile);
		return 0;
	}

	// This will recieve the rest of the header.
	fread(&fileHeader, 124, 1, imageFile);

	unsigned int imageHeight = *(unsigned int*) &(fileHeader[8]);
	unsigned int imageWidth = *(unsigned int*) &(fileHeader[12]);
	unsigned int linearSize = *(unsigned int*) &(fileHeader[16]);
	unsigned int mipMapAmount = *(unsigned int*) &(fileHeader[24]);
	unsigned int fourCCType = *(unsigned int*) &(fileHeader[80]);
	unsigned char * buffer;
	unsigned int bufferSize;
	
	// This checks to see how large the bufferSize will be, including all mip-maps.
	bufferSize = mipMapAmount > 1 ? linearSize * 2 : linearSize;

	// This allocates memory for the buffer using malloc() and reads the rest of the file.
	buffer = (unsigned char*) malloc(bufferSize * sizeof(unsigned char));
	fread(buffer, 1, bufferSize, imageFile);
	
	// This closes the file.
	fclose(imageFile);

	// This finds the proper format.
	unsigned int format;
	switch (fourCCType) {
	case FOURCC_DXT1:
		format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
		break;
	case FOURCC_DXT3:
		format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
		break;
	case FOURCC_DXT5:
		format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
		break;
	default:
		free(buffer);
		return 0;
	}

	// This creates an OpenGL texture.
	GLuint textureID;
	glGenTextures(1, &textureID);

	// This tells openGL that future functions will reference this texture.
	glBindTexture(GL_TEXTURE_2D, textureID);

	// This sets the pixel atorage mode.
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	unsigned int blockSize = (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;
	unsigned int offset = 0;

	// This loads the mipmaps.
	for (unsigned int level = 0; level < mipMapAmount && (imageWidth || imageHeight); level++) {
		unsigned int size = ((imageWidth + 3) / 4) * ((imageHeight + 3) / 4) * blockSize;
		glCompressedTexImage2D(GL_TEXTURE_2D, level, format, imageWidth, imageHeight, 0, size, buffer + offset);
		offset += size;
		imageWidth /= 2;
		imageHeight /= 2;

		// If these if statements are triggered, the textures do not have power of two dimensions. Eww.
		if (imageWidth < 1) {
			imageWidth = 1;
		}
		if (imageHeight < 1) {
			imageHeight = 1;
		}

	}
	free(buffer);
	return textureID;
}