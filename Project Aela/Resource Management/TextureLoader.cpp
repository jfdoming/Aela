#include "TextureLoader.h"

#define AELA_RESOURCE_TEXTURE_HEADER_SIZE 128

using namespace Aela;

bool TextureLoader::isValid(std::ifstream &in) {
	return true;
}

Resource* Aela::TextureLoader::load(std::ifstream &in) {
	// using char[] for speed
	char textureHeader[AELA_RESOURCE_TEXTURE_HEADER_SIZE];

	// read in the texture header
	in.get(textureHeader, AELA_RESOURCE_TEXTURE_HEADER_SIZE);

	// This will check the file to see if it begins with "DDS ".
	// If this file header does not exist, the file is not a DDS file.
	if (strncmp(textureHeader, "DDS ", 4) != 0) {
		return NULL;
	}

	unsigned int imageHeight = *(unsigned int*) &(textureHeader[8]);
	unsigned int imageWidth = *(unsigned int*) &(textureHeader[12]);
	unsigned int linearSize = *(unsigned int*) &(textureHeader[16]);
	unsigned int mipMapAmount = *(unsigned int*) &(textureHeader[24]);
	unsigned int fourCCType = *(unsigned int*) &(textureHeader[80]);
	unsigned char * buffer;
	unsigned int bufferSize;
	
	// This checks to see how large the bufferSize will be, including all mip-maps.
	bufferSize = mipMapAmount > 1 ? linearSize * 2 : linearSize;

	// This allocates memory for the buffer using malloc() and reads the rest of the file.
	buffer = new unsigned char[bufferSize * sizeof(unsigned char)];
	in.get(buffer, bufferSize)

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
			delete[] buffer;
			return 0;
	}

	// This creates an OpenGL texture.
	GLuint textureID;
	glGenTextures(1, &textureID);

	// This tells openGL that future functions will reference this texture.
	glBindTexture(GL_TEXTURE_2D, textureID);

	// This sets the pixel storage mode.
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
	delete[] buffer;

	TextureResource * res = new TextureResource("");
	res->data = textureID;

	std::cout << "texture id: " << res->data << std::endl;

	return res;
}
