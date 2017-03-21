/*
* Class: Project Aela's 2D Renderer
* Author: Ekkon Games
* Date: February 2017
* Description: A class used by Aela's Renderer to render textures as 2D objects.
*/

#include "Basic2DRenderer.h"
#include "texture.hpp"

// This is the setup function, which sets up OpenGL-related variables.
void Basic2DRenderer::setup() {
	bufferTextureToBufferProgramID = loadShaders("res/shaders/2D_Buffer_To_Buffer.vertexshader", "res/shaders/2D_Buffer_To_Buffer.fragmentshader");
	textToBufferProgramID = loadShaders("res/shaders/Text_To_Buffer.vertexshader", "res/shaders/Text_To_Buffer.fragmentshader");
	imageToBufferProgramID = loadShaders("res/shaders/2D_Texture_To_Buffer.vertexshader", "res/shaders/2D_Texture_To_Buffer.fragmentshader");

	imageTextureID = glGetUniformLocation(bufferTextureToBufferProgramID, "quadTexture");
	imageQuadVertexBufferID = glGetAttribLocation(bufferTextureToBufferProgramID, "vertexPosition");
	imageTopLeftBufferID = glGetAttribLocation(bufferTextureToBufferProgramID, "positionOfTextureOnScreen");
	imageWidthAndHeightBufferID = glGetAttribLocation(bufferTextureToBufferProgramID, "boundingBoxDimensions");
	imageDimensionsBufferID = glGetAttribLocation(bufferTextureToBufferProgramID, "textureDimensions");
	imageWindowDimensionsBufferID = glGetAttribLocation(bufferTextureToBufferProgramID, "windowDimensions");

	characterTextureID = glGetUniformLocation(textToBufferProgramID, "quadTexture");
	characterQuadVertexBufferID = glGetAttribLocation(textToBufferProgramID, "vertexPosition");
	characterTopLeftBufferID = glGetAttribLocation(textToBufferProgramID, "positionOfTextureOnScreen");
	characterWidthAndHeightBufferID = glGetAttribLocation(textToBufferProgramID, "boundingBoxDimensions");
	characterDimensionsBufferID = glGetAttribLocation(textToBufferProgramID, "textureDimensions");
	characterWindowDimensionsBufferID = glGetAttribLocation(textToBufferProgramID, "windowDimensions");
	characterColourBufferID = glGetAttribLocation(textToBufferProgramID, "textColour");

	glGenTextures(1, &characterTexture);
	glBindTexture(GL_TEXTURE_2D, characterTexture);

	// Note for later: use glBlitFramebuffer() for copying a rectangle of an image.
	glGenFramebuffers(1, &frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

	glGenTextures(1, frameBufferTexture.getTexture());
	glBindTexture(GL_TEXTURE_2D, *(frameBufferTexture.getTexture()));
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1280, 720, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	/* Clamping to edges is important to prevent artifacts when scaling */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	/* Linear filtering usually looks best for text */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	frameBufferTexture.setDimensions(0, 0, 1280, 720);
	frameBufferTexture.setOutput(0, 0, 1280, 720);
	glDrawBuffer(GL_COLOR_ATTACHMENT0);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, *(frameBufferTexture.getTexture()), 0);
	checkFrameBuffer();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

// This function renders a texture directly to a framebuffer.
void Basic2DRenderer::renderTextureToBuffer(Texture* texture, Rect<unsigned int>* windowDimensions, GLuint frameBuffer) {
	renderTextureToBuffer(texture, windowDimensions, frameBuffer, bufferTextureToBufferProgramID);
}

// This function renders a texture directly to a framebuffer.
void Basic2DRenderer::renderTextureToBuffer(Texture* texture, Rect<unsigned int>* windowDimensions, GLuint frameBuffer, GLuint customShader) {
	// Due to the way that OpenGL seems to flip the texture, the shader needs to flip it back
	// and show the texture's opposite side. Or the renderer could just send normal data.
	// GL_CULL_FACE needs to be disabled if one uses the first option.
	glDisable(GL_CULL_FACE);

	// This calculates some positioning and dimensional data.
	Rect<int>* output = texture->getOutput();

	int windowWidth = windowDimensions->getWidth(), windowHeight = windowDimensions->getHeight();
	float topLeftX = (float) (-1 + ((float) output->getX() / (windowWidth / 2))),
		topLeftY = (float) (-1 + ((float) output->getY() / (windowHeight / 2))),
		bottomRightX = (-1 + ((float) (output->getX() + output->getWidth()) / (windowWidth / 2))),
		bottomRightY = (float) (-1 + ((float) (output->getY() + output->getHeight()) / (windowHeight / 2)));

	GLfloat vertexBufferData[] = {
		topLeftX,  bottomRightY, 0.0f,
		topLeftX, topLeftY, 0.0f,
		bottomRightX, topLeftY, 0.0f,
		topLeftX,  bottomRightY, 0.0f,
		bottomRightX, topLeftY, 0.0f,
		bottomRightX,  bottomRightY, 0.0f
	};

	int textureWidth = texture->getDimensions()->getWidth(), textureHeight = texture->getDimensions()->getHeight();

	// These are some functions needed by OpenGL.
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	glViewport(0, 0, windowWidth, windowHeight);
	glUseProgram(customShader);

	// This passes the texture to OpenGL.
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, *(texture->getTexture()));
	glUniform1i(imageTextureID, 0);

	// This passes all necessary variables to the shader.
	GLuint quadVertexBuffer;
	glGenBuffers(1, &quadVertexBuffer);

	glBindBuffer(GL_ARRAY_BUFFER, quadVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexBufferData), vertexBufferData, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(imageQuadVertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, quadVertexBuffer);
	glVertexAttribPointer(
		imageQuadVertexBufferID,
		3,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*) 0
	);

	GLfloat topLeftArray[] = { 
		1.0f + topLeftX, 1.0f + topLeftY,
		1.0f + topLeftX, 1.0f + topLeftY,
		1.0f + topLeftX, 1.0f + topLeftY,
		1.0f + topLeftX, 1.0f + topLeftY,
		1.0f + topLeftX, 1.0f + topLeftY,
		1.0f + topLeftX, 1.0f + topLeftY
	};

	GLuint topLeftBuffer;
	glGenBuffers(1, &topLeftBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, topLeftBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(topLeftArray), topLeftArray, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(imageTopLeftBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, topLeftBuffer);
	glVertexAttribPointer(
		imageTopLeftBufferID,
		2,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*) 0
	);

	GLfloat widthAndHeightArray[] = {
		(float) output->getWidth(), (float) output->getHeight(),
		(float) output->getWidth(), (float) output->getHeight(),
		(float) output->getWidth(), (float) output->getHeight(),
		(float) output->getWidth(), (float) output->getHeight(),
		(float) output->getWidth(), (float) output->getHeight(),
		(float) output->getWidth(), (float) output->getHeight()
	};

	GLuint widthAndHeightBuffer;
	glGenBuffers(1, &widthAndHeightBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, widthAndHeightBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(widthAndHeightArray), widthAndHeightArray, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(imageWidthAndHeightBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, widthAndHeightBuffer);
	glVertexAttribPointer(
		imageWidthAndHeightBufferID,
		2,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*) 0
	);

	GLfloat textureDimensionsArray[] = {
		(float) textureWidth, (float) textureHeight,
		(float) textureWidth, (float) textureHeight,
		(float) textureWidth, (float) textureHeight,
		(float) textureWidth, (float) textureHeight,
		(float) textureWidth, (float) textureHeight,
		(float) textureWidth, (float) textureHeight
	};

	GLuint textureDimensionsBuffer;
	glGenBuffers(1, &textureDimensionsBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, textureDimensionsBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(textureDimensionsArray), textureDimensionsArray, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(imageDimensionsBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, textureDimensionsBuffer);
	glVertexAttribPointer(
		imageDimensionsBufferID,
		2,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*) 0
	);

	GLfloat windowDimensionsArray[] = {
		(float) windowWidth, (float) windowHeight,
		(float) windowWidth, (float) windowHeight,
		(float) windowWidth, (float) windowHeight,
		(float) windowWidth, (float) windowHeight,
		(float) windowWidth, (float) windowHeight,
		(float) windowWidth, (float) windowHeight
	};

	GLuint windowDimensionsBuffer;
	glGenBuffers(1, &windowDimensionsBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, windowDimensionsBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(windowDimensionsArray), windowDimensionsArray, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(imageWindowDimensionsBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, windowDimensionsBuffer);
	glVertexAttribPointer(
		imageWindowDimensionsBufferID,
		2,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*) 0
	);

	// This draws the texture and deletes the buffers that were used.
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisableVertexAttribArray(imageQuadVertexBufferID);
	glDisableVertexAttribArray(imageTopLeftBufferID);
	glDisableVertexAttribArray(imageWidthAndHeightBufferID);
	glDisableVertexAttribArray(imageDimensionsBufferID);
	glDisableVertexAttribArray(imageWindowDimensionsBufferID);
	glDeleteBuffers(1, &quadVertexBuffer);
	glDeleteBuffers(1, &topLeftBuffer);
	glDeleteBuffers(1, &widthAndHeightBuffer);
	glDeleteBuffers(1, &textureDimensionsBuffer);
	glDeleteBuffers(1, &windowDimensionsBuffer);
}

// This function renders a texture directly to the screen.
void Basic2DRenderer::renderTextureTo2DBuffer(Texture* texture, Rect<unsigned int>* windowDimensions) {
	// Due to the way that OpenGL seems to flip the texture, the shader needs to flip it back
	// and show the texture's opposite side. Or the renderer could just send normal data.
	// GL_CULL_FACE needs to be disabled if one uses the first option.
	glDisable(GL_CULL_FACE);

	// This calculates some positioning and dimensional data.
	Rect<int>* output = texture->getOutput();

	int windowWidth = windowDimensions->getWidth(), windowHeight = windowDimensions->getHeight();
	float topLeftX = (float) (-1 + ((float) output->getX() / (windowWidth / 2))),
		topLeftY = (float) (-1 + ((float) output->getY() / (windowHeight / 2))),
		bottomRightX = (-1 + ((float) (output->getX() + output->getWidth()) / (windowWidth / 2))),
		bottomRightY = (float) (-1 + ((float) (output->getY() + output->getHeight()) / (windowHeight / 2)));

	GLfloat vertexBufferData[] = {
		topLeftX,  bottomRightY, 0.0f,
		topLeftX, topLeftY, 0.0f,
		bottomRightX, topLeftY, 0.0f,
		topLeftX,  bottomRightY, 0.0f,
		bottomRightX, topLeftY, 0.0f,
		bottomRightX,  bottomRightY, 0.0f
	};

	int textureWidth = texture->getDimensions()->getWidth(), textureHeight = texture->getDimensions()->getHeight();

	glUseProgram(imageToBufferProgramID);

	// These are some functions needed by OpenGL.
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	glViewport(0, 0, windowWidth, windowHeight);

	// This passes the texture to OpenGL.
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, *(texture->getTexture()));
	glUniform1i(imageTextureID, 0);

	// This passes all necessary variables to the shader.
	GLuint quadVertexBuffer;
	glGenBuffers(1, &quadVertexBuffer);

	glBindBuffer(GL_ARRAY_BUFFER, quadVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexBufferData), vertexBufferData, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(imageQuadVertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, quadVertexBuffer);
	glVertexAttribPointer(
		imageQuadVertexBufferID,
		3,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*) 0
	);

	GLfloat topLeftArray[] = {
		1.0f + topLeftX, 1.0f + topLeftY,
		1.0f + topLeftX, 1.0f + topLeftY,
		1.0f + topLeftX, 1.0f + topLeftY,
		1.0f + topLeftX, 1.0f + topLeftY,
		1.0f + topLeftX, 1.0f + topLeftY,
		1.0f + topLeftX, 1.0f + topLeftY
	};

	GLuint topLeftBuffer;
	glGenBuffers(1, &topLeftBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, topLeftBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(topLeftArray), topLeftArray, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(imageTopLeftBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, topLeftBuffer);
	glVertexAttribPointer(
		imageTopLeftBufferID,
		2,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*) 0
	);

	GLfloat widthAndHeightArray[] = {
		(float) output->getWidth(), (float) output->getHeight(),
		(float) output->getWidth(), (float) output->getHeight(),
		(float) output->getWidth(), (float) output->getHeight(),
		(float) output->getWidth(), (float) output->getHeight(),
		(float) output->getWidth(), (float) output->getHeight(),
		(float) output->getWidth(), (float) output->getHeight()
	};

	GLuint widthAndHeightBuffer;
	glGenBuffers(1, &widthAndHeightBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, widthAndHeightBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(widthAndHeightArray), widthAndHeightArray, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(imageWidthAndHeightBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, widthAndHeightBuffer);
	glVertexAttribPointer(
		imageWidthAndHeightBufferID,
		2,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*) 0
	);

	GLfloat textureDimensionsArray[] = {
		(float) textureWidth, (float) textureHeight,
		(float) textureWidth, (float) textureHeight,
		(float) textureWidth, (float) textureHeight,
		(float) textureWidth, (float) textureHeight,
		(float) textureWidth, (float) textureHeight,
		(float) textureWidth, (float) textureHeight
	};

	GLuint textureDimensionsBuffer;
	glGenBuffers(1, &textureDimensionsBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, textureDimensionsBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(textureDimensionsArray), textureDimensionsArray, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(imageDimensionsBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, textureDimensionsBuffer);
	glVertexAttribPointer(
		imageDimensionsBufferID,
		2,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*) 0
	);

	GLfloat windowDimensionsArray[] = {
		(float) windowWidth, (float) windowHeight,
		(float) windowWidth, (float) windowHeight,
		(float) windowWidth, (float) windowHeight,
		(float) windowWidth, (float) windowHeight,
		(float) windowWidth, (float) windowHeight,
		(float) windowWidth, (float) windowHeight
	};

	GLuint windowDimensionsBuffer;
	glGenBuffers(1, &windowDimensionsBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, windowDimensionsBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(windowDimensionsArray), windowDimensionsArray, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(imageWindowDimensionsBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, windowDimensionsBuffer);
	glVertexAttribPointer(
		imageWindowDimensionsBufferID,
		2,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*) 0
	);

	// This draws the texture and deletes the buffers that were used.
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisableVertexAttribArray(imageQuadVertexBufferID);
	glDisableVertexAttribArray(imageTopLeftBufferID);
	glDisableVertexAttribArray(imageWidthAndHeightBufferID);
	glDisableVertexAttribArray(imageDimensionsBufferID);
	glDisableVertexAttribArray(imageWindowDimensionsBufferID);
	glDeleteBuffers(1, &quadVertexBuffer);
	glDeleteBuffers(1, &topLeftBuffer);
	glDeleteBuffers(1, &widthAndHeightBuffer);
	glDeleteBuffers(1, &textureDimensionsBuffer);
	glDeleteBuffers(1, &windowDimensionsBuffer);
}

void Basic2DRenderer::renderTextTo2DBuffer(std::string text, TextFont* textFont, Rect<int>* output, Rect<unsigned int>* windowDimensions, ColourRGBA* colour, unsigned int pointsPerPixel) {
	Rect<int> characterPositioning = *output;

	FT_GlyphSlot glyph = (*(textFont->getFace()))->glyph;
	char* p;

	/* Loop through all characters */
	for (unsigned int i = 0; i < text.length(); i++) {
		p = &((char) (text.at(i)));
		/* Try to load and render the character */
		if (FT_Load_Char(*(textFont->getFace()), *p, FT_LOAD_RENDER)) {
			continue;
		}
		characterPositioning.setY(output->getY() - (glyph->metrics.horiBearingY / pointsPerPixel));
		characterPositioning.setWidth(glyph->bitmap.width);
		characterPositioning.setHeight(glyph->bitmap.rows);
		renderCharacter(p, &characterPositioning, windowDimensions, glyph, colour);
		characterPositioning.setX(characterPositioning.getX() + (glyph->metrics.horiAdvance / pointsPerPixel));
	}
}

void Basic2DRenderer::clearFrameBuffer() {
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	glClear(GL_COLOR_BUFFER_BIT);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Basic2DRenderer::renderCharacter(char* character, Rect<int>* output, Rect<unsigned int>* windowDimensions, FT_GlyphSlot glyph, ColourRGBA* colour) {
	/* Upload the "bitmap", which contains an 8-bit grayscale image, as an alpha texture */
	glBindTexture(GL_TEXTURE_2D, characterTexture);
	/* We require 1 byte alignment when uploading texture data */
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	/* Clamping to edges is important to prevent artifacts when scaling */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	/* Linear filtering usually looks best for text */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, glyph->bitmap.width, glyph->bitmap.rows, 0, GL_ALPHA, GL_UNSIGNED_BYTE, glyph->bitmap.buffer);

	// Due to the way that OpenGL seems to flip the texture, the shader needs to flip it back
	// and show the texture's opposite side. Or the renderer could just send normal data.
	// GL_CULL_FACE needs to be disabled if one uses the first option.
	glDisable(GL_CULL_FACE);

	int windowWidth = windowDimensions->getWidth(), windowHeight = windowDimensions->getHeight();
	float topLeftX = (float) (-1 + ((float) output->getX() / (windowWidth / 2))),
		topLeftY = (float) (-1 + ((float) output->getY() / (windowHeight / 2))),
		bottomRightX = (-1 + ((float) (output->getX() + output->getWidth()) / (windowWidth / 2))),
		bottomRightY = (float) (-1 + ((float) (output->getY() + output->getHeight()) / (windowHeight / 2)));

	GLfloat vertexBufferData[] = {
		topLeftX,  bottomRightY, 0.0f,
		topLeftX, topLeftY, 0.0f,
		bottomRightX, topLeftY, 0.0f,
		topLeftX,  bottomRightY, 0.0f,
		bottomRightX, topLeftY, 0.0f,
		bottomRightX,  bottomRightY, 0.0f
	};

	int textureWidth = output->getWidth(), textureHeight = output->getHeight();

	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

	glViewport(0, 0, windowWidth, windowHeight);
	glUseProgram(textToBufferProgramID);

	GLuint quadVertexBuffer;
	glGenBuffers(1, &quadVertexBuffer);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, characterTexture);
	glUniform1i(characterTextureID, 0);

	glBindBuffer(GL_ARRAY_BUFFER, quadVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexBufferData), vertexBufferData, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(characterQuadVertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, quadVertexBuffer);
	glVertexAttribPointer(
		characterQuadVertexBufferID,
		3,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*) 0
	);

	GLfloat topLeftArray[] = {
		1.0f + topLeftX, 1.0f + topLeftY,
		1.0f + topLeftX, 1.0f + topLeftY,
		1.0f + topLeftX, 1.0f + topLeftY,
		1.0f + topLeftX, 1.0f + topLeftY,
		1.0f + topLeftX, 1.0f + topLeftY,
		1.0f + topLeftX, 1.0f + topLeftY
	};

	GLuint topLeftBuffer;
	glGenBuffers(1, &topLeftBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, topLeftBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(topLeftArray), topLeftArray, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(characterTopLeftBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, topLeftBuffer);
	glVertexAttribPointer(
		characterTopLeftBufferID,
		2,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*) 0
	);

	GLfloat widthAndHeightArray[] = {
		(float) output->getWidth(), (float) output->getHeight(),
		(float) output->getWidth(), (float) output->getHeight(),
		(float) output->getWidth(), (float) output->getHeight(),
		(float) output->getWidth(), (float) output->getHeight(),
		(float) output->getWidth(), (float) output->getHeight(),
		(float) output->getWidth(), (float) output->getHeight()
	};

	GLuint widthAndHeightBuffer;
	glGenBuffers(1, &widthAndHeightBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, widthAndHeightBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(widthAndHeightArray), widthAndHeightArray, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(characterWidthAndHeightBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, widthAndHeightBuffer);
	glVertexAttribPointer(
		characterWidthAndHeightBufferID,
		2,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*) 0
	);

	GLfloat textureDimensionsArray[] = {
		(float) textureWidth, (float) textureHeight,
		(float) textureWidth, (float) textureHeight,
		(float) textureWidth, (float) textureHeight,
		(float) textureWidth, (float) textureHeight,
		(float) textureWidth, (float) textureHeight,
		(float) textureWidth, (float) textureHeight
	};

	GLuint textureDimensionsBuffer;
	glGenBuffers(1, &textureDimensionsBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, textureDimensionsBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(textureDimensionsArray), textureDimensionsArray, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(characterDimensionsBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, textureDimensionsBuffer);
	glVertexAttribPointer(
		characterDimensionsBufferID,
		2,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*) 0
	);

	GLfloat windowDimensionsArray[] = {
		(float) windowWidth, (float) windowHeight,
		(float) windowWidth, (float) windowHeight,
		(float) windowWidth, (float) windowHeight,
		(float) windowWidth, (float) windowHeight,
		(float) windowWidth, (float) windowHeight,
		(float) windowWidth, (float) windowHeight
	};

	GLuint windowDimensionsBuffer;
	glGenBuffers(1, &windowDimensionsBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, windowDimensionsBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(windowDimensionsArray), windowDimensionsArray, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(characterWindowDimensionsBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, windowDimensionsBuffer);
	glVertexAttribPointer(
		characterWindowDimensionsBufferID,
		2,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*) 0
	);

	GLfloat colourArray[] = {
		colour->getR(), colour->getG(), colour->getB(), colour->getA(),
		colour->getR(), colour->getG(), colour->getB(), colour->getA(),
		colour->getR(), colour->getG(), colour->getB(), colour->getA(),
		colour->getR(), colour->getG(), colour->getB(), colour->getA(),
		colour->getR(), colour->getG(), colour->getB(), colour->getA(),
		colour->getR(), colour->getG(), colour->getB(), colour->getA()
	};

	GLuint colourBuffer;
	glGenBuffers(1, &colourBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colourBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colourArray), colourArray, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(characterColourBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, colourBuffer);
	glVertexAttribPointer(
		characterColourBufferID,
		4,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*) 0
	);

	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisableVertexAttribArray(characterQuadVertexBufferID);
	glDisableVertexAttribArray(characterTopLeftBufferID);
	glDisableVertexAttribArray(characterWidthAndHeightBufferID);
	glDisableVertexAttribArray(characterDimensionsBufferID);
	glDisableVertexAttribArray(characterWindowDimensionsBufferID);
	glDisableVertexAttribArray(characterColourBufferID);
	glDeleteBuffers(1, &quadVertexBuffer);
	glDeleteBuffers(1, &topLeftBuffer);
	glDeleteBuffers(1, &widthAndHeightBuffer);
	glDeleteBuffers(1, &textureDimensionsBuffer);
	glDeleteBuffers(1, &windowDimensionsBuffer);
	glDeleteBuffers(1, &colourBuffer);
}

void Basic2DRenderer::drawTestQuad() {
	glUseProgram(0);
	glColor3f(1.0, 1.0, 0.5);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);
	glVertex2f(0, 0);
	glTexCoord2f(1, 0);
	glVertex2f(1, 0);
	glTexCoord2f(1, 1);
	glVertex2f(1, 1);
	glTexCoord2f(0, 1);
	glVertex2f(0, 1);
	glEnd();
}

GLuint* Basic2DRenderer::getFrameBufffer() {
	return &frameBuffer;
}

Texture* Basic2DRenderer::getFrameBufferTexture() {
	return &frameBufferTexture;
}

bool Basic2DRenderer::checkFrameBuffer() {
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		AelaErrorHandling::windowError("Aela 2D Rendering",
			"There was a problem setting up the framebuffer.\nIt's probably OpenGL's fault.\nOr maybe your graphics processor is a potato.");
		return false;
	} else {
		return true;
	}
}