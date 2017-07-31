#include "TextureLoader.h"
#include "Texture.h"

using namespace Aela;

TextureLoader::TextureLoader() {
}

TextureLoader::~TextureLoader() {
}

void TextureLoader::expose(LuaManager& mgr) {
	// only expose part of the class to Lua
	luabridge::getGlobalNamespace(mgr.getLuaState())
		.beginClass<Aela::TextureLoader>("TextureLoader")
		.endClass();

	// expose this object
	// Sorry Waseef, I had to comment this out in order to compile.
	// mgr.exposeObject(this, "textureLoader");
}

bool Aela::TextureLoader::load(std::unordered_map<std::string, Resource*>* resources, std::string src) {
	// try to open the file
	std::ifstream in;
	if (!open(in, src)) {
		return false;
	}

	// create an OpenGL texture
	GLuint modelTextureID;
	glGenTextures(1, &modelTextureID);

	// This tells openGL that future functions will reference this texture.
	glBindTexture(GL_TEXTURE_2D, modelTextureID);

	unsigned int imageWidth, imageHeight;
	loadTexture(in, &modelTextureID, GL_TEXTURE_2D, &imageWidth, &imageHeight);

	in.close();

	Texture* res = new Texture(modelTextureID);
	res->setDimensions(0, 0, imageWidth, imageHeight);
	res->setOutput(0, 0, imageWidth, imageHeight);

	(*resources)[src] = res;
	return true;
}

void TextureLoader::loadTexture(std::ifstream& in, GLuint* texID, GLenum target) {
	loadTexture(in, texID, target, nullptr, nullptr);
}

void TextureLoader::loadTexture(std::ifstream& in, GLuint* texID, GLenum target, unsigned int* width, unsigned int* height) {
	// using char[] for speed
	char textureHeader[AELA_RESOURCE_TEXTURE_HEADER_SIZE];

	// read the file header
	in.read(textureHeader, AELA_RESOURCE_TEXTURE_HEADER_SIZE);

	// make sure we are reading a texture file
	if (strncmp((char*) textureHeader, AELA_RESOURCE_TEXTURE_HEADER_START, 4) != 0) {
		return;
	}

	unsigned int imageHeight = *(unsigned int*) &(textureHeader[12]);
	unsigned int imageWidth = *(unsigned int*) &(textureHeader[16]);
	unsigned int linearSize = *(unsigned int*) &(textureHeader[20]);
	unsigned int mipMapAmount = *(unsigned int*) &(textureHeader[28]);
	unsigned int fourCCType = *(unsigned int*) &(textureHeader[84]);
	unsigned int bufferSize;

	// calculate texture size, including mip-maps
	bufferSize = (mipMapAmount > 1) ? (linearSize * 2) : linearSize;

	// read the contents of the file.
	char* buffer = new char[bufferSize * sizeof(unsigned char)];
	in.read(buffer, bufferSize);

	// determine the texture format
	unsigned int format;
	switch ((CompressionAlgorithms) fourCCType) {
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
			return;
	}

	// set the pixel storage mode
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	unsigned int blockSize = (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;
	unsigned int offset = 0;

	// load the mipmaps
	for (unsigned int level = 0; level < mipMapAmount && (imageWidth || imageHeight); level++) {
		unsigned int size = ((imageWidth + 3) / 4) * ((imageHeight + 3) / 4) * blockSize;
		glCompressedTexImage2D(target, level, format, imageWidth, imageHeight, 0, size, buffer + offset);
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

	if (width != nullptr) {
		*width = imageWidth;
	}
	if (height != nullptr) {
		*height = imageHeight;
	}
}