#include <stdio.h>
// #include <stdlib.h>
#include <string>
#include "AelaError.h"
#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif

#include <GL/glew.h>


GLuint loadBMP_custom(const char * imagepath) {

	printf("Reading image %s\n", imagepath);

	// Data read from the header of the BMP file
	unsigned char header[54];
	unsigned int dataPos;
	unsigned int imageSize;
	unsigned int width, height;
	// Actual RGB data
	unsigned char * data;

	// Open the file
	FILE * file = fopen(imagepath, "rb");
	if (!file) { printf("%s could not be opened. Are you in the right directory ? Don't forget to read the FAQ !\n", imagepath); getchar(); return 0; }

	// Read the header, i.e. the 54 first bytes

	// If less than 54 bytes are read, problem
	if (fread(header, 1, 54, file) != 54) {
		printf("Not a correct BMP file\n");
		return 0;
	}
	// A BMP files always begins with "BM"
	if (header[0] != 'B' || header[1] != 'M') {
		printf("Not a correct BMP file\n");
		return 0;
	}
	// Make sure this is a 24bpp file
	if (*(int*)&(header[0x1E]) != 0) { printf("Not a correct BMP file\n");    return 0; }
	if (*(int*)&(header[0x1C]) != 24) { printf("Not a correct BMP file\n");    return 0; }

	// Read the information about the image
	dataPos = *(int*)&(header[0x0A]);
	imageSize = *(int*)&(header[0x22]);
	width = *(int*)&(header[0x12]);
	height = *(int*)&(header[0x16]);

	// Some BMP files are misformatted, guess missing information
	if (imageSize == 0)    imageSize = width*height * 3; // 3 : one byte for each Red, Green and Blue component
	if (dataPos == 0)      dataPos = 54; // The BMP header is done that way

										 // Create a buffer
	data = new unsigned char[imageSize];

	// Read the actual data from the file into the buffer
	fread(data, 1, imageSize, file);

	// Everything is in memory now, the file wan be closed
	fclose(file);

	// Create one OpenGL texture
	GLuint textureID;
	glGenTextures(1, &textureID);

	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, textureID);

	// Give the image to OpenGL
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

	// OpenGL has now copied the data. Free our own version
	delete[] data;

	// Poor filtering, or ...
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); 

	// ... nice trilinear filtering.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);

	// Return the ID of the texture we just created
	return textureID;
}


#define FOURCC_DXT1 0x31545844 // Equivalent to "DXT1" in ASCII
#define FOURCC_DXT3 0x33545844 // Equivalent to "DXT3" in ASCII
#define FOURCC_DXT5 0x35545844 // Equivalent to "DXT5" in ASCII

GLuint loadDDS(std::string filePath) {
	unsigned char fileHeader[124];
	FILE * imageFile;

	// This will try to open the DDS file.
	imageFile = fopen(filePath.c_str(), "rb");
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