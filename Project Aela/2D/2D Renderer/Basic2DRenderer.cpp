/*
* Class: Project Aela's 2D Renderer
* Author: Robert Ciborowski
* Date: February 2017
* Description: A class used by Aela's Renderer to render 2D objects.
*/

#include "Basic2DRenderer.h"
#include "../../Old Garbage/texture.hpp"
#include "../../Error Handler/ErrorHandler.h"
#include "../../Old Garbage/shader.hpp"

// This is the setup function, which sets up OpenGL-related variables.
void Basic2DRenderer::setup() {
	// This loads shaders.
	load2DShaders();

	// This gets handles to GLSL variables.
	getGLSLVariableHandles();

	// This generates textures for characters, textures buffers, etc.
	generateTexturesAndFrameBuffer();
	
	// This checks the framebuffer.
	checkFrameBuffer();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Basic2DRenderer::load2DShaders() {
	// These functions load shaders.
	bufferTextureToBufferProgramID = loadShaders("res/shaders/2D/2DTextureBufferToBuffer.vert", "res/shaders/2D/2DTextureBufferToBuffer.frag");
	textToBufferProgramID = loadShaders("res/shaders/2D/TextToBuffer.vert", "res/shaders/2D/TextToBuffer.frag");
	imageToBufferProgramID = loadShaders("res/shaders/2D/2DTextureToBuffer.vert", "res/shaders/2D/2DTextureToBuffer.frag");
}

void Basic2DRenderer::getGLSLVariableHandles() {
	// These functions get handles to GLSL variables from GLSL shaders.
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
}

void Basic2DRenderer::generateTexturesAndFrameBuffer() {
	// This sets up the 2D framebuffer. This is included in the same function as the
	// texture generation code to avoid only one of the two (buffer and texture) being generated.
	// This is especially important since some textures are tied to the framebuffer.
	// Note for later: use glBlitFramebuffer() for copying a rectangle of an image.
	glGenFramebuffers(1, &frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

	// This loads the character texture.
	glGenTextures(1, &characterTexture);
	glBindTexture(GL_TEXTURE_2D, characterTexture);

	// This loads the texture that is tied to the framebuffer.
	glGenTextures(1, frameBufferTexture.getTexture());
	glBindTexture(GL_TEXTURE_2D, *(frameBufferTexture.getTexture()));
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1280, 720, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	frameBufferTexture.setDimensions(0, 0, 1280, 720);
	frameBufferTexture.setOutput(0, 0, 1280, 720);
	glDrawBuffer(GL_COLOR_ATTACHMENT0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, *(frameBufferTexture.getTexture()), 0);
}

// This function renders a texture directly to a specified framebuffer.
void Basic2DRenderer::renderTextureToBuffer(Texture* texture, Rect<unsigned int>* windowDimensions, GLuint frameBuffer) {
	renderTextureToBuffer(texture, windowDimensions, frameBuffer, bufferTextureToBufferProgramID);
}

// This function renders a texture directly to a framebuffer, using a custom shader.
// Note: Custom shaders can be used for post-process effects.
void Basic2DRenderer::renderTextureToBuffer(Texture* texture, Rect<unsigned int>* windowDimensions, GLuint frameBuffer, GLuint customShader) {
	// Due to the way that OpenGL seems to flip the texture, the shader needs to flip it back
	// and show the texture's opposite side. Or the renderer could just send normal data.
	// GL_CULL_FACE needs to be disabled if one uses the first option.
	glDisable(GL_CULL_FACE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

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

	// The following large chunk of code passes all necessary variables to the shader.
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

// This function renders a texture directly to the 2D renderer's buffer.
void Basic2DRenderer::renderTextureTo2DBuffer(Texture* texture, Rect<unsigned int>* windowDimensions) {
	renderTextureToBuffer(texture, windowDimensions, frameBuffer, imageToBufferProgramID);
}

// This function renders text directly to the 2D renderer's buffer.
void Basic2DRenderer::renderTextTo2DBuffer(std::string text, TextFont* textFont, Rect<int>* output, Rect<unsigned int>* windowDimensions, ColourRGBA* colour, unsigned int pointsPerPixel) {
	// This sets up some variables for later use.
	Rect<int> characterPositioning = *output;
	FT_GlyphSlot glyph = (*(textFont->getFace()))->glyph;
	char* p;

	// This renders the characters, one at a time.
	for (unsigned int i = 0; i < text.length(); i++) {
		p = &((char) (text.at(i)));
		// This loads the character.
		if (FT_Load_Char(*(textFont->getFace()), *p, FT_LOAD_RENDER)) {
			continue;
		}
		// This positions and renders the character.
		characterPositioning.setY(output->getY() - (glyph->metrics.horiBearingY / pointsPerPixel));
		characterPositioning.setWidth(glyph->bitmap.width);
		characterPositioning.setHeight(glyph->bitmap.rows);
		renderCharacter(p, &characterPositioning, windowDimensions, glyph, colour);
		characterPositioning.setX(characterPositioning.getX() + (glyph->metrics.horiAdvance / pointsPerPixel));
	}
}

// This clears the 2D framebuffer.
void Basic2DRenderer::clearFrameBuffer() {
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	glClear(GL_COLOR_BUFFER_BIT);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

// This renders a single character using the character shader.
void Basic2DRenderer::renderCharacter(char* character, Rect<int>* output, Rect<unsigned int>* windowDimensions, FT_GlyphSlot glyph, ColourRGBA* colour) {
	glBindTexture(GL_TEXTURE_2D, characterTexture);
	// Note: 1 byte alignment is required when uploading texture data.
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	// Apparently, linear filtering looks best for text.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, glyph->bitmap.width, glyph->bitmap.rows, 0, GL_ALPHA, GL_UNSIGNED_BYTE, glyph->bitmap.buffer);

	// Due to the way that OpenGL seems to flip the texture, the shader needs to flip it back
	// and show the texture's opposite side. Or the renderer could just send normal data.
	// GL_CULL_FACE needs to be disabled if one uses the first option.
	glDisable(GL_CULL_FACE);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	// This sets up positioning data.
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

	// This calls some OpenGL functions necessary before rendering.
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	glViewport(0, 0, windowWidth, windowHeight);
	glUseProgram(textToBufferProgramID);

	// This sets up some variables necessary for the GLSL shader.
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

	// This renders the character and disables buffers afterwards.
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

// This function is used to draw a quad (for testing purposes).
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

// This function is used to draw a rectangle.
void Basic2DRenderer::renderRectangle(Rect<int>* output, Rect<unsigned int>* windowDimensions,  ColourRGBA* colour) {
	glUseProgram(0);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	glViewport(0, 0, windowDimensions->getWidth(), windowDimensions->getHeight());
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	int windowWidth = windowDimensions->getWidth(), windowHeight = windowDimensions->getHeight();
	glColor4f(colour->getR(), colour->getG(), colour->getB(), colour->getA());
	glBegin(GL_QUADS);
	glVertex2f(-1 + 2 * ((float) output->getX() / windowWidth), -(-1 + 2 * ((float) output->getY() / windowHeight)));
	glVertex2f(-1 + 2 * ((float) output->getX() / windowWidth), -(-1 + 2 * (((float) output->getY() + output->getHeight()) / windowHeight)));
	glVertex2f(-1 + 2 * (((float) output->getX() + output->getWidth()) / windowWidth), -(-1 + 2 * (((float) output->getY() + output->getHeight()) / windowHeight)));
	glVertex2f(-1 + 2 * (((float) output->getX() + output->getWidth()) / windowWidth), -(-1 + 2 * ((float) output->getY() / windowHeight)));
	glEnd();
}

void Basic2DRenderer::renderRectangle(unsigned int xPosition, unsigned int yPosition, int width, int height, Rect<unsigned int>* windowDimensions, ColourRGBA* colour) {
	Rect<int> rect(xPosition, yPosition, width, height);
	renderRectangle(&rect, windowDimensions, colour);
}

void Basic2DRenderer::renderTriangle(glm::vec2 pointA, glm::vec2 pointB, glm::vec2 pointC, Rect<unsigned int>* windowDimensions, ColourRGBA* colour) {
	glUseProgram(0);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	glViewport(0, 0, windowDimensions->getWidth(), windowDimensions->getHeight());
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	int windowWidth = windowDimensions->getWidth(), windowHeight = windowDimensions->getHeight();
	glColor4f(colour->getR(), colour->getG(), colour->getB(), colour->getA());
	glBegin(GL_TRIANGLES);
	glVertex2f(-1 + 2 * ((float) pointA.x / windowWidth), -(-1 + 2 * ((float) pointA.y / windowHeight)));
	glVertex2f(-1 + 2 * ((float) pointB.x / windowWidth), -(-1 + 2 * ((float) pointB.y / windowHeight)));
	glVertex2f(-1 + 2 * ((float) pointC.x / windowWidth), -(-1 + 2 * ((float) pointC.y / windowHeight)));
	glEnd();
}

void Basic2DRenderer::renderTriangle(unsigned int pointAX, unsigned int pointAY, unsigned int pointBX, unsigned int pointBY, unsigned int pointCX, unsigned int pointCY, Rect<unsigned int>* windowDimensions, ColourRGBA* colour) {
	glm::vec2 pointA(pointAX, pointAY);
	glm::vec2 pointB(pointBX, pointBY);
	glm::vec2 pointC(pointCX, pointCY);
	renderTriangle(pointA, pointB, pointC, windowDimensions, colour);
}

// These are some getters.
GLuint* Basic2DRenderer::getFrameBufffer() {
	return &frameBuffer;
}

Texture* Basic2DRenderer::getFrameBufferTexture() {
	return &frameBufferTexture;
}

// This checks to see if the framebuffer is set up properly.
bool Basic2DRenderer::checkFrameBuffer() {
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		AelaErrorHandling::windowError("Aela 2D Rendering",
			"There was a problem setting up the framebuffer.\nIt's probably OpenGL's fault.\nOr maybe your graphics processor is a potato.");
		return false;
	} else {
		return true;
	}
}