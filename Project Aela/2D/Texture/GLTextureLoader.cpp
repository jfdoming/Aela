#include "GLTextureLoader.h"
#include <fstream>

#include <windows.h>
#include "../../Utilities/strut.h"

using namespace Aela;

GLTextureLoader::GLTextureLoader() {
}

GLTextureLoader::~GLTextureLoader() {
}

void GLTextureLoader::expose(LuaManager& mgr) {
	// only expose part of the class to Lua
	luabridge::getGlobalNamespace(mgr.getLuaState())
		.beginClass<Aela::GLTextureLoader>("TextureLoader")
		.endClass();

	// expose this object
	// Sorry Waseef, I had to comment this out in order to compile.
	// mgr.exposeObject(this, "textureLoader");
}

bool Aela::GLTextureLoader::load(ResourceMap& resources, std::string src) {
	GLTexture* res = nullptr;
	loadTexture(res, src);

	resources.put(src, res);
	return true;
}

bool GLTextureLoader::loadTexture(GLTexture*& result, std::string src) {
	// try to open the file
	std::ifstream in(src, std::ios::binary);
	if (!isValid(in)) {
		AelaErrorHandling::consoleWindowError("Aela Texture Loader", "The file " + src + " could not be found.");
		return false;
	}

	// create an OpenGL texture
	GLuint modelTextureID;
	glGenTextures(1, &modelTextureID);

	// This tells openGL that future functions will reference this texture.
	glBindTexture(GL_TEXTURE_2D, modelTextureID);

	unsigned int imageWidth, imageHeight;
	if (!loadTextureToBoundId(in, GL_TEXTURE_2D, &imageWidth, &imageHeight)) {
		in.close();
		AelaErrorHandling::consoleWindowError("Aela Texture Loader", "The file " + src + " could not be read.");
		return false;
	}

	in.close();

	result = new GLTexture(src, modelTextureID);

	// I'm removing this feature for now, since it's rather important to be consistent throughout Aela in resource conventions.
	// In addition, this currently does not save any memory since an extra property was added to every resource.
	// Please talk to me about re-adding this feature if you need it.

	/*// This checks to see if the resource can use the "~" shortcut for its path to signify that the resource is using the
	// commonly excepted path for its resource type. Doing this saves memory.
	if (src.substr(0, defaultTexturePath.size()) == defaultTexturePath) {
		result->setSource("~" + src.substr(defaultTexturePath.size(), src.size()));
	} else {
		result->setSource(src);
	}*/
	result->setDimensions(0, 0, imageWidth, imageHeight);
	return true;
}

bool GLTextureLoader::loadTextureToBoundId(std::ifstream& in, GLenum target) {
	return loadTextureToBoundId(in, target, nullptr, nullptr);
}

bool GLTextureLoader::loadTextureToBoundId(std::ifstream& in, GLenum target, unsigned int* width, unsigned int* height) {
	// using char[] for speed
	unsigned char textureHeader[AELA_RESOURCE_TEXTURE_HEADER_SIZE];

	// read the file header
	if (!in.read(reinterpret_cast<char*>(textureHeader), AELA_RESOURCE_TEXTURE_HEADER_SIZE)) {
		return false;
	}

	// make sure we are reading a texture file
	if (strncmp((char*) textureHeader, AELA_RESOURCE_TEXTURE_HEADER_START, 4) != 0) {
		return false;
	}

	unsigned int imageHeight = *(unsigned int*) &(textureHeader[12]), imageWidth = *(unsigned int*) &(textureHeader[16]),
		linearSize = *(unsigned int*) &(textureHeader[20]), mipMapAmount = *(unsigned int*) &(textureHeader[28]),
		fourCCType = *(unsigned int*) &(textureHeader[84]), bufferSize;

	unsigned int heightToReturn = imageHeight, widthToReturn = imageWidth;

	// calculate texture size, including mip-maps
	bufferSize = (mipMapAmount > 1) ? (linearSize * 2) : linearSize;

	// read the contents of the file.
	unsigned char* buffer = new unsigned char[bufferSize * sizeof(unsigned char)];
	in.read(reinterpret_cast<char*>(buffer), bufferSize);

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
			return false;
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
		*width = widthToReturn;
	}
	if (height != nullptr) {
		*height = heightToReturn;
	}

	return true;
}

bool GLTextureLoader::loadTextureUsingFILE(std::string path, GLenum target) {
	return loadTextureUsingFILE(path, target, nullptr, nullptr);
}

// This loads a texture using FILE. If you're wondering why the first parameter is the path rather than a FILE* (similar to how
// loadTexture's first parameter is an ifstream&), it's because you'd have to call ResourceLoader::open before using this function,
// this would mean having to pass a nullptr of FILE* into ResourceLoader::open, which breaks everything. Go ahead, try it, you'll see.
bool GLTextureLoader::loadTextureUsingFILE(std::string path, GLenum target, unsigned int* width, unsigned int* height) {
	FILE* in;

	// using char[] for speed
	unsigned char textureHeader[AELA_RESOURCE_TEXTURE_HEADER_SIZE];

	// This will try to open the DDS file.
	fopen_s(&in, path.c_str(), "rb");
	if (in == NULL) {
		AelaErrorHandling::windowError("Aela DDS Loader", "A DDS file was not found.");
		return false;
	}

	// read the file header
	fread(textureHeader, 1, AELA_RESOURCE_TEXTURE_HEADER_SIZE, in);

	// make sure we are reading a texture file
	if (strncmp((char*) textureHeader, AELA_RESOURCE_TEXTURE_HEADER_START, 4) != 0) {
		return false;
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
	unsigned char* buffer = new unsigned char[bufferSize * sizeof(unsigned char)];
	fread(buffer, 1, bufferSize, in);

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
		return false;
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
	return true;
}
