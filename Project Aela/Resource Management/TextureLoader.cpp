#include "TextureLoader.h"

using namespace Aela;

bool TextureLoader::isValid(std::ifstream &in) {
	return true;
}

Resource* Aela::TextureLoader::load(std::ifstream &in) {
	// using char[] for speed
	char textureHeader[AELA_RESOURCE_TEXTURE_HEADER_SIZE];

	// read the file header
	in.get(textureHeader, AELA_RESOURCE_TEXTURE_HEADER_SIZE);

	// make sure we are reading a texture file
	if (strncmp(textureHeader, AELA_RESOURCE_TEXTURE_HEADER_START, 4) != 0) {
		return NULL;
	}

	unsigned int imageHeight = *(unsigned int*) &(textureHeader[8]);
	unsigned int imageWidth = *(unsigned int*) &(textureHeader[12]);
	unsigned int linearSize = *(unsigned int*) &(textureHeader[16]);
	unsigned int mipMapAmount = *(unsigned int*) &(textureHeader[24]);
	unsigned int fourCCType = *(unsigned int*) &(textureHeader[80]);
	char* buffer;
	unsigned int bufferSize;
	
	// calculate texture size, including mip-maps
	bufferSize = (mipMapAmount > 1) ? (linearSize * 2) : linearSize;

	// read the contents of the file.
	buffer = new char[bufferSize * sizeof(unsigned char)];
	in.get(buffer, bufferSize);

	// determine the texture format
	unsigned int format;
	switch (fourCCType) {
		case CompressionAlgorithms::FOURCC_DXT1:
			format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
			break;
		case CompressionAlgorithms::FOURCC_DXT3:
			format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
			break;
		case CompressionAlgorithms::FOURCC_DXT5:
			format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
			break;
		default:
			delete[] buffer;
			return NULL;
	}

	// create an OpenGL texture
	GLuint textureID;
	glGenTextures(1, &textureID);

	// This tells openGL that future functions will reference this texture.
	glBindTexture(GL_TEXTURE_2D, textureID);

	// set the pixel storage mode
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	unsigned int blockSize = (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;
	unsigned int offset = 0;

	// load the mipmaps
	for (unsigned int level = 0; level < mipMapAmount && (imageWidth || imageHeight); level++) {
		unsigned int size = ((imageWidth + 3) / 4) * ((imageHeight + 3) / 4) * blockSize;
		glCompressedTexImage2D(GL_TEXTURE_2D, level, format, imageWidth, imageHeight, 0, size, buffer + offset);
		offset += size;
		imageWidth /= 2;
		imageHeight /= 2;

		// ensure the texture has dimensions that are powers of two
		if (imageWidth < 1) {
			imageWidth = 1;
		}
		if (imageHeight < 1) {
			imageHeight = 1;
		}

	}
	delete[] buffer;

	TextureResource* res = new TextureResource();
	res->data = textureID;

	std::cout << "texture id: " << res->data << std::endl;

	return res;
}
