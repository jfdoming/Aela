/*
* Class: Project Aela's 2D GLRenderer
* Author: Robert Ciborowski
* Date: February 2017
* Description: A class used by Aela's GLRenderer to render 2D objects.
*/

#include "Basic2DGLRenderer.h"
#include "../../Old Garbage/texture.hpp"
#include "../../Error Handler/ErrorHandling.h"
#include "../../Old Garbage/shader.hpp"
#include <freetype/ftglyph.h>
#include <iostream>
#include <csignal>

using namespace Aela;

Aela::Basic2DGLRenderer::Basic2DGLRenderer() = default;

// This is the setup function, which sets up OpenGL-related variables.
void Basic2DGLRenderer::setup() {
	// This loads shaders.
	load2DShaders();

	// This gets handles to GLSL variables.
	getGLSLVariableHandles();

	// This loads the character texture.
	glGenTextures(1, &characterTexture);
	glBindTexture(GL_TEXTURE_2D, characterTexture);

	// This sets up the character texture.
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	// Apparently, linear filtering looks best for text.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// This checks the framebuffer.
	checkFrameBuffer();
}

// This function loads all necessary shaders.
void Basic2DGLRenderer::load2DShaders() {
	bufferTextureToBufferProgramID = loadShaders("../../../res/shaders/2D/2DTextureBufferToBuffer.vert", "../../../res/shaders/2D/2DTextureBufferToBuffer.frag");
	textToBufferProgramID = loadShaders("../../../res/shaders/2D/TextToBuffer.vert", "../../../res/shaders/2D/TextToBuffer.frag");
	imageToBufferProgramID = loadShaders("../../../res/shaders/2D/2DTextureToBuffer.vert", "../../../res/shaders/2D/2DTextureToBuffer.frag");
}

// This function gets handles to GLSL variables from GLSL shaders.
void Basic2DGLRenderer::getGLSLVariableHandles() {
	imageTextureID = glGetUniformLocation(bufferTextureToBufferProgramID, "quadTexture");
	imageVertexBufferID = glGetAttribLocation(bufferTextureToBufferProgramID, "vertexPosition");
	imageUVBufferID = glGetAttribLocation(imageToBufferProgramID, "UVPosition");
	imageTopLeftCoordID = glGetUniformLocation(bufferTextureToBufferProgramID, "positionOfTextureOnScreen");
	imageWindowDimensionsID = glGetUniformLocation(bufferTextureToBufferProgramID, "windowDimensions");
	imageTintID = glGetUniformLocation(bufferTextureToBufferProgramID, "tintMultiplier");

	characterTextureID = glGetUniformLocation(textToBufferProgramID, "quadTexture");
	characterQuadVertexBufferID = glGetAttribLocation(textToBufferProgramID, "vertexPosition");
	characterTopLeftCoordID = glGetUniformLocation(textToBufferProgramID, "positionOfTextureOnScreen");
	characterDimensionsID = glGetUniformLocation(textToBufferProgramID, "textureDimensions");
	characterWindowDimensionsID = glGetUniformLocation(textToBufferProgramID, "windowDimensions");
	characterColourID = glGetUniformLocation(textToBufferProgramID, "textColour");
}

void Basic2DGLRenderer::setupSimple2DFramebuffer(Simple2DFramebuffer* framebuffer, unsigned int multisampling, Rect<int>* dimensions, Rect<int>* output) {
	// This sets up the 2D framebuffer. This is included in the same function as the
	// texture generation code to avoid only one of the two (buffer and texture) being generated.
	// This is especially important since some textures are tied to the framebuffer.

	GLuint* texture = framebuffer->getFramebufferImage()->getTexture();
	GLuint* multisampledTexture = framebuffer->getMultisampledFramebufferImage()->getTexture();

	// This generates the multisampled colour framebuffer, which later is blitted to the regular colour framebuffer.
	glGenFramebuffers(1, framebuffer->getMultisampledFramebuffer());
	glBindFramebuffer(GL_FRAMEBUFFER, *framebuffer->getMultisampledFramebuffer());

	glGenTextures(1, multisampledTexture);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, *multisampledTexture);
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, multisampling, GL_RGBA, windowWidth, windowHeight, GL_TRUE);
	/* Clamping to edges is important to prevent artifacts when scaling */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	/* Linear filtering usually looks best for text */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	framebuffer->getMultisampledFramebufferImage()->setDimensions(0, 0, windowWidth, windowHeight);
	glDrawBuffer(GL_COLOR_ATTACHMENT0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, *multisampledTexture, 0);

	glGenFramebuffers(1, framebuffer->getFramebuffer());
	glBindFramebuffer(GL_FRAMEBUFFER, *framebuffer->getFramebuffer());
	glGenTextures(1, texture);
	glBindTexture(GL_TEXTURE_2D, *texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, windowWidth, windowHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glDrawBuffer(GL_COLOR_ATTACHMENT0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, *texture, 0);

	framebuffer->getFramebufferImage()->setDimensions(dimensions);
	framebuffer->setMultisampling(multisampling);
}

// This function renders a texture directly to a specified framebuffer using the shader for rendering images.
void Basic2DGLRenderer::renderImageToSimple2DFramebuffer(Image* image, Simple2DFramebuffer* framebuffer, Rect<int>* output, Rect<int>* cropping, Rect<unsigned int>* windowDimensions, ColourRGBA* tint, PositioningMode2D positioningMode) {
	renderImageToSimple2DFramebuffer(image, framebuffer, output, cropping, windowDimensions, tint, positioningMode, imageToBufferProgramID);
}

// This function renders a texture directly to a framebuffer, using a custom shader.
// Note: Custom shaders can be used for post-process effects.
void Basic2DGLRenderer::renderImageToSimple2DFramebuffer(Image* image, Simple2DFramebuffer* framebuffer, Rect<int>* output, Rect<int>* cropping, Rect<unsigned int>* windowDimensions, ColourRGBA* tint, PositioningMode2D positioningMode, unsigned int customShader) {
	if (framebuffer->getMultisampling() > 0) {
		renderImageToFramebuffer(image, *framebuffer->getMultisampledFramebuffer(), output, cropping, windowDimensions, tint, positioningMode, customShader);
	} else {
		renderImageToFramebuffer(image, *framebuffer->getFramebuffer(), output, cropping, windowDimensions, tint, positioningMode, customShader);
	}
}

// This function renders a texture directly to a specified framebuffer.
void Basic2DGLRenderer::renderImageToFramebuffer(Image* image, unsigned int framebuffer, Rect<int>* output, Rect<int>* cropping, Rect<unsigned int>* windowDimensions, ColourRGBA* tint, PositioningMode2D positioningMode) {
	renderImageToFramebuffer(image, framebuffer, output, cropping, windowDimensions, tint, positioningMode, bufferTextureToBufferProgramID);
}

// This function renders a texture directly to a framebuffer, using a custom shader.
// Note: Custom shaders can be used for post-process effects.
// Note: Shaders that use the same uniforms and buffers as the 2DTextureBufferToBuffer shader (bufferTextureToBufferProgramID) should set the same
// locations of uniforms as they are in the 2DTextureBufferToBuffer shader.
void Basic2DGLRenderer::renderImageToFramebuffer(Image* image, unsigned int framebuffer, Rect<int>* output, Rect<int>* cropping,
	Rect<unsigned int>* windowDimensions, ColourRGBA* tint, PositioningMode2D positioningMode, unsigned int customShader) {
	glDisable(GL_CULL_FACE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	int textureWidth = image->getDimensions()->getWidth(), textureHeight = image->getDimensions()->getHeight();
	int windowWidth = windowDimensions->getWidth(), windowHeight = windowDimensions->getHeight();
	float vertexTopLeftX, vertexTopLeftY, vertexBottomRightX, vertexBottomRightY;

	// You know, it would be nice if Visual Studio would stop secretly taking out the spaces between my C-style casts.
	switch (positioningMode) {
		case PositioningMode2D::TOP_LEFT:
			vertexTopLeftX = -1 + ((float)output->getX() / (windowWidth / 2.0f));
			vertexTopLeftY = -1 + ((float)output->getY() / (windowHeight / 2.0f));
			vertexBottomRightX = (-1 + ((float)(output->getX() + output->getWidth()) / (windowWidth / 2.0f)));
			vertexBottomRightY = -1 + ((float)(output->getY() + output->getHeight()) / (windowHeight / 2.0f));
			break;
		case PositioningMode2D::CENTER:
			int halfWidth = output->getWidth(), halfHeight = output->getHeight();
			vertexTopLeftX = -1 + ((float) (output->getX() - halfWidth) / (windowWidth / 2.0f));
			vertexTopLeftY = -1 + ((float) (output->getY() - halfHeight) / (windowHeight / 2.0f));
			vertexBottomRightX = (-1 + ((float) (output->getX() + halfWidth) / (windowWidth / 2.0f)));
			vertexBottomRightY = -1 + ((float) (output->getY() + +halfHeight) / (windowHeight / 2.0f));
			break;
	}

	GLfloat vertexBufferData[] = {
		vertexTopLeftX,  vertexBottomRightY, 0.0f,
		vertexTopLeftX, vertexTopLeftY, 0.0f,
		vertexBottomRightX, vertexTopLeftY, 0.0f,
		vertexTopLeftX,  vertexBottomRightY, 0.0f,
		vertexBottomRightX, vertexTopLeftY, 0.0f,
		vertexBottomRightX,  vertexBottomRightY, 0.0f
	};

	float UVTopLeftX = (float)cropping->getX() / textureWidth,
		UVTopLeftY = (float)cropping->getY() / textureHeight,
		UVBottomRightX = (float)(cropping->getX() + cropping->getWidth()) / textureWidth,
		UVBottomRightY = (float)(cropping->getY() + cropping->getHeight()) / textureHeight;

	GLfloat UVBufferData[] = {
		UVTopLeftX,  UVBottomRightY,
		UVTopLeftX, UVTopLeftY,
		UVBottomRightX, UVTopLeftY,
		UVTopLeftX,  UVBottomRightY,
		UVBottomRightX, UVTopLeftY,
		UVBottomRightX,  UVBottomRightY
	};

	// These are some functions needed by OpenGL.
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glViewport(0, 0, windowWidth, windowHeight);
	glUseProgram(customShader);

	// This passes the texture to OpenGL.
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, *(image->getTexture()));
	glUniform1i(imageTextureID, 0);

	// If there is a tint, use it. Otherwise, the shader will use its own default tint value.
	if (tint != nullptr) {
		glm::vec4 tintAsVec4 = tint->getVec4();
		glUniform4fv(imageTintID, 1, &tintAsVec4.r);
	}

	GLuint vertexBuffer;
	glGenBuffers(1, &vertexBuffer);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexBufferData), vertexBufferData, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(imageVertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glVertexAttribPointer(
		imageVertexBufferID,
		3,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*) nullptr
	);

	GLuint UVBuffer;
	glGenBuffers(1, &UVBuffer);

	glBindBuffer(GL_ARRAY_BUFFER, UVBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(UVBufferData), UVBufferData, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(imageUVBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, UVBuffer);
	glVertexAttribPointer(
		imageUVBufferID,
		2,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*) nullptr
	);

	GLfloat topLeftArray[] = {
		1.0f + vertexTopLeftX, 1.0f + vertexTopLeftY
	};

	glUniform2fv(imageTopLeftCoordID, 1, &topLeftArray[0]);

	GLfloat windowDimensionsArray[] = {
		(float) windowWidth, (float) windowHeight
	};

	glUniform2fv(imageWindowDimensionsID, 1, &windowDimensionsArray[0]);

	// This draws the texture and deletes the buffers that were used.
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisableVertexAttribArray(imageVertexBufferID);
	glDisableVertexAttribArray(imageUVBufferID);
	glDeleteBuffers(1, &vertexBuffer);
	glDeleteBuffers(1, &UVBuffer);
}

// This function renders text directly to the 2D renderer's buffer.
void Basic2DGLRenderer::renderTextToSimple2DFramebuffer(std::string text, Font* font, unsigned int size, Simple2DFramebuffer* framebuffer, Rect<int>* output,
	Rect<unsigned int>* windowDimensions, ColourRGBA* colour, PositioningMode2D positioningMode, unsigned int pointsPerPixel) {
	// Some of the terms used in this function may be confusing. See https://www.freetype.org/freetype2/docs/tutorial/step2.html to look up the terms.
	// This function creates a buffer that represents the texture of the text. It does this by combining the buffers of the individual glyphs.
	// Then it renders the buffer of the entire text. This is faster than rendering each individual character using the GPU.

	font->setSize(size);
	font->prepareForRendering(textScaling);

	// This creates a bounding box for the text.
	Rect<float> characterPositioning(output->getX(), output->getY(), 0, 0);
	// characterPositioning.setDimensions(0, 0);

	unsigned int bufferWidth = 0, bufferHeight;

	// This is the y position of the origin if the top of the glyph had a y position of zero. It's basically just bearingY.
	int originY = 0;

	// Distance to top is bearingY, distance to bottom is height - bearingY.
	int distanceToTop = 0, distanceToBottom = 0;

	// These are some more glyph-related variables.
	FT_GlyphSlot glyph = font->getFace()->glyph;

	struct RenderableGlyph {
		unsigned char* buffer;
		int width, rows, originY;
		char character;
	};

	std::vector<RenderableGlyph> glyphs;
	std::vector<unsigned char> buffer;

	FT_Face face = font->getFace();

	// This goes through every glyph to perform actions upon every glyph's properties.
	for (char i : text) {
		FT_Error error = FT_Load_Char(face, static_cast<FT_ULong>(i), FT_LOAD_RENDER);
		if (error != 0) {
			continue;
		}

		bufferWidth += glyph->bitmap.width;
		characterPositioning.setWidth(characterPositioning.getWidth() + (float) glyph->bitmap.width / textScaling);

		if (glyph->metrics.horiBearingY / (signed int) pointsPerPixel > distanceToTop) {
			distanceToTop = glyph->metrics.horiBearingY / (signed int) pointsPerPixel;
		}

		if ((glyph->metrics.height - glyph->metrics.horiBearingY) / (signed int) pointsPerPixel > distanceToBottom) {
			distanceToBottom = (glyph->metrics.height - glyph->metrics.horiBearingY) / (signed int) pointsPerPixel;
		}

		if (glyph->metrics.horiBearingY / (signed int) pointsPerPixel > originY) {
			originY = glyph->metrics.horiBearingY / (signed int) pointsPerPixel;
		}

		// This sets up a Renderable glyph using the glyph's properties. These have to be stored in a struct since FT_GlyphSlot is just a
		// typedef'd pointer.
		if (glyph->bitmap.buffer != nullptr) {
			RenderableGlyph charBuffer;
			auto* data = (unsigned char*) malloc(glyph->bitmap.width * glyph->bitmap.rows);
			memcpy(data, glyph->bitmap.buffer, glyph->bitmap.width * glyph->bitmap.rows);
			charBuffer.buffer = data;
			charBuffer.width = glyph->bitmap.width;
			charBuffer.rows = glyph->bitmap.rows;
			charBuffer.originY = glyph->metrics.horiBearingY / pointsPerPixel;
			charBuffer.character = i;
			glyphs.push_back(charBuffer);
		} else if (i != ' ') {
			continue;
		}

		// This sets up a RenderableGlyph for the space in between characters. It also makes a space for the character ' '.
		RenderableGlyph advancementBuffer;
		unsigned int horiAdvance = glyph->metrics.horiAdvance / pointsPerPixel;
		unsigned int advancement = horiAdvance - glm::clamp(glyph->bitmap.width, (unsigned int) 0, horiAdvance);
		bufferWidth += advancement;
		advancementBuffer.width = advancement;
		characterPositioning.setWidth(characterPositioning.getWidth() + (float) advancement / textScaling);
		advancementBuffer.rows = 0;
		advancementBuffer.originY = 0;
		advancementBuffer.buffer = nullptr;
		advancementBuffer.character = ' ';
		glyphs.push_back(advancementBuffer);
	}

	switch (positioningMode) {
		case PositioningMode2D::TOP_LEFT:
			characterPositioning.setY(characterPositioning.getY() - (1.0f * distanceToTop / textScaling));
			break;
		case PositioningMode2D::CENTER:
			characterPositioning.setX(characterPositioning.getX() - characterPositioning.getWidth() / 2.0f);
			characterPositioning.setY(characterPositioning.getY() - (float) distanceToTop / textScaling - characterPositioning.getHeight() / 2.0f);
			break;
	}

	bufferHeight = (unsigned int) abs(distanceToTop + distanceToBottom);

	characterPositioning.setHeight((float) bufferHeight / textScaling);
	// characterPositioning.setDimensions((float) bufferWidth / textScaling, (float) bufferHeight / textScaling);

	// This creates one buffer out of each glyph's individual buffer.
	if (!glyphs.empty()) {
		for (int i = 0; i < bufferHeight; i++) {
			for (auto& iGlyph : glyphs) {
				for (int whichChar = 0; whichChar < iGlyph.width; whichChar++) {
					int whichRowToAccess = i - (originY - iGlyph.originY);
					if (iGlyph.buffer != nullptr && whichRowToAccess >= 0 && whichRowToAccess * iGlyph.width + whichChar <
																					iGlyph.width * iGlyph.rows) {
						buffer.push_back(iGlyph.buffer[whichRowToAccess * iGlyph.width + whichChar]);
					} else {
						buffer.push_back(0);
					}
				}
			}
		}
	}

	if (bufferWidth != 0) {
		// This renders the text's buffer using the GPU.
		renderCharacterBuffer(framebuffer, &characterPositioning, windowDimensions, &buffer, bufferWidth, bufferHeight, colour);
	}

	// This frees the memory of each glyph's individual buffer.
	for (auto& jGlyph : glyphs) {
		if (jGlyph.buffer != nullptr) {
			free(jGlyph.buffer);
		}
	}
}

// This function is made specifically for blitting a 2D multisampled buffer to a regular 2D buffer.
void Basic2DGLRenderer::renderMultisampledBufferToBuffer(unsigned int multisampledBuffer, unsigned int secondaryBuffer, Rect<unsigned int>* windowDimensions) {
	glBindFramebuffer(GL_READ_FRAMEBUFFER, multisampledBuffer);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, secondaryBuffer);
	glBlitFramebuffer(0, 0, windowDimensions->getWidth(), windowDimensions->getHeight(), 0, 0, windowDimensions->getWidth(), windowDimensions->getHeight(), GL_COLOR_BUFFER_BIT, GL_NEAREST);
}

// This clears the 2D framebuffer.
void Basic2DGLRenderer::clearSimple2DFramebuffer(Simple2DFramebuffer* framebuffer) {
	glClearColor(0.0, 0.0, 0.0, 0.0);
	if (framebuffer->getMultisampling() > 0) {
		glBindFramebuffer(GL_FRAMEBUFFER, *framebuffer->getMultisampledFramebuffer());
	} else {
		glBindFramebuffer(GL_FRAMEBUFFER, *framebuffer->getFramebuffer());
	}
	glClear(GL_COLOR_BUFFER_BIT);
}

void Aela::Basic2DGLRenderer::setTextScaling(unsigned int textScaling) {
	this->textScaling = textScaling;
}

// This renders a single character using the character shader.
void Basic2DGLRenderer::renderCharacterBuffer(Simple2DFramebuffer* framebuffer, Rect<float>* output, Rect<unsigned int>* windowDimensions, std::vector<unsigned char>* buffer, unsigned int width, unsigned int rows, ColourRGBA* colour) {
	if (buffer != nullptr && !buffer->empty()) {
		glBindTexture(GL_TEXTURE_2D, characterTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, width, rows, 0, GL_ALPHA, GL_UNSIGNED_BYTE, &buffer->at(0));
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);

		// This sets up positioning data.
		int windowWidth = windowDimensions->getWidth(), windowHeight = windowDimensions->getHeight();
		float topLeftX = -1 + (output->getX() / (windowWidth / 2.0f)),
			topLeftY = -1 + (output->getY() / (windowHeight / 2.0f)),
			bottomRightX = -1 + (output->getX() + output->getWidth()) / (windowWidth / 2.0f),
			bottomRightY = -1 + ((output->getY() + output->getHeight()) / (windowHeight / 2.0f));

		GLfloat vertexBufferData[] = {
			topLeftX,  bottomRightY, 0.0f,
			topLeftX, topLeftY, 0.0f,
			bottomRightX, topLeftY, 0.0f,
			topLeftX,  bottomRightY, 0.0f,
			bottomRightX, topLeftY, 0.0f,
			bottomRightX,  bottomRightY, 0.0f
		};

		float textureHeight = output->getHeight();
		float textureWidth = output->getWidth();

		// This calls some OpenGL functions necessary before rendering.
		if (framebuffer->getMultisampling() > 0) {
			glBindFramebuffer(GL_FRAMEBUFFER, *framebuffer->getMultisampledFramebuffer());
		} else {
			glBindFramebuffer(GL_FRAMEBUFFER, *framebuffer->getFramebuffer());
		}
		glViewport(0, 0, windowWidth, windowHeight);
		glUseProgram(textToBufferProgramID);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, characterTexture);
		glUniform1i(characterTextureID, 0);

		// This sets up some variables necessary for the GLSL shader.
		GLuint quadVertexBuffer;
		glGenBuffers(1, &quadVertexBuffer);

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
			(void*) nullptr
		);

		GLfloat topLeftArray[] = {
			1.0f + topLeftX, 1.0f + topLeftY
		};

		glUniform2fv(characterTopLeftCoordID, 1, &topLeftArray[0]);

		GLfloat textureDimensionsArray[] = {
				textureWidth, textureHeight
		};

		glUniform2fv(characterDimensionsID, 1, &textureDimensionsArray[0]);

		GLfloat windowDimensionsArray[] = {
			(float) windowWidth, (float) windowHeight
		};

		glUniform2fv(characterWindowDimensionsID, 1, &windowDimensionsArray[0]);

		GLfloat colourArray[] = {
			colour->getR(), colour->getG(), colour->getB(), colour->getA()
		};

		glUniform4fv(characterColourID, 1, &colourArray[0]);

		// This renders the character and disables buffers afterwards.
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glDisableVertexAttribArray(characterQuadVertexBufferID);
		glDisableVertexAttribArray(characterTopLeftCoordID);
		glDisableVertexAttribArray(characterDimensionsID);
		glDisableVertexAttribArray(characterWindowDimensionsID);
		glDisableVertexAttribArray(characterColourID);
		glDeleteBuffers(1, &quadVertexBuffer);
	}
}

// This function is used to draw a quad (for testing purposes).
void Basic2DGLRenderer::drawTestQuad() {
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
void Basic2DGLRenderer::renderRectangle(Rect<int>* output, Simple2DFramebuffer* framebuffer, Rect<unsigned int>* windowDimensions, ColourRGBA* colour, PositioningMode2D positioningMode) {
	glUseProgram(0);
	if (framebuffer->getMultisampling() > 0) {
		glBindFramebuffer(GL_FRAMEBUFFER, *framebuffer->getMultisampledFramebuffer());
	} else {
		glBindFramebuffer(GL_FRAMEBUFFER, *framebuffer->getFramebuffer());
	}
	glViewport(0, 0, windowDimensions->getWidth(), windowDimensions->getHeight());
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	int windowWidth = windowDimensions->getWidth(), windowHeight = windowDimensions->getHeight();
	glColor4f(colour->getR(), colour->getG(), colour->getB(), colour->getA());
	glBegin(GL_QUADS);

	float vertexTopLeftX, vertexTopLeftY, vertexBottomRightX, vertexBottomRightY;

	// You know, it would be nice if Visual Studio would stop secretly taking out the spaces between my C-style casts.
	switch (positioningMode) {
		case PositioningMode2D::TOP_LEFT:
			vertexTopLeftX = -1 + ((float)output->getX() / (windowWidth / 2.0f));
			vertexTopLeftY = -1 + ((float)output->getY() / (windowHeight / 2.0f));
			vertexBottomRightX = (-1 + ((float)(output->getX() + output->getWidth()) / (windowWidth / 2.0f)));
			vertexBottomRightY = -1 + ((float)(output->getY() + output->getHeight()) / (windowHeight / 2.0f));
			break;
		case PositioningMode2D::CENTER:
			int halfWidth = output->getWidth(), halfHeight = output->getHeight();
			vertexTopLeftX = -1 + ((float)(output->getX() - halfWidth) / (windowWidth / 2.0f));
			vertexTopLeftY = -1 + ((float)(output->getY() - halfHeight) / (windowHeight / 2.0f));
			vertexBottomRightX = (-1 + ((float)(output->getX() + halfWidth) / (windowWidth / 2.0f)));
			vertexBottomRightY = -1 + ((float)(output->getY() + +halfHeight) / (windowHeight / 2.0f));
			break;
	}

	glVertex2f(vertexTopLeftX, -(vertexTopLeftY));
	glVertex2f(vertexTopLeftX, -(vertexBottomRightY));
	glVertex2f(vertexBottomRightX, -(vertexBottomRightY));
	glVertex2f(vertexBottomRightX, -(vertexTopLeftY));
	glEnd();
}

void Basic2DGLRenderer::renderRectangle(unsigned int xPosition, unsigned int yPosition, int width, int height, Simple2DFramebuffer* framebuffer, Rect<unsigned int>* windowDimensions, ColourRGBA* colour, PositioningMode2D positioningMode) {
	Rect<int> rect(xPosition, yPosition, width, height);
	renderRectangle(&rect, framebuffer, windowDimensions, colour, positioningMode);
}

// This function is used to draw a triangle.
void Basic2DGLRenderer::renderTriangle(glm::vec2 pointA, glm::vec2 pointB, glm::vec2 pointC, Simple2DFramebuffer* framebuffer, Rect<unsigned int>* windowDimensions, ColourRGBA* colour) {
	glUseProgram(0);
	if (framebuffer->getMultisampling() > 0) {
		glBindFramebuffer(GL_FRAMEBUFFER, *framebuffer->getMultisampledFramebuffer());
	} else {
		glBindFramebuffer(GL_FRAMEBUFFER, *framebuffer->getFramebuffer());
	}
	glViewport(0, 0, windowDimensions->getWidth(), windowDimensions->getHeight());
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	int windowWidth = windowDimensions->getWidth(), windowHeight = windowDimensions->getHeight();
	glColor4f(colour->getR(), colour->getG(), colour->getB(), colour->getA());
	glBegin(GL_TRIANGLES);
	glVertex2f(-1 + 2 * (pointA.x / windowWidth), -(-1 + 2 * (pointA.y / windowHeight)));
	glVertex2f(-1 + 2 * (pointB.x / windowWidth), -(-1 + 2 * (pointB.y / windowHeight)));
	glVertex2f(-1 + 2 * (pointC.x / windowWidth), -(-1 + 2 * (pointC.y / windowHeight)));
	glEnd();
}

void Basic2DGLRenderer::renderTriangle(unsigned int pointAX, unsigned int pointAY, unsigned int pointBX, unsigned int pointBY, unsigned int pointCX,
	unsigned int pointCY, Simple2DFramebuffer* framebuffer, Rect<unsigned int>* windowDimensions, ColourRGBA* colour) {
	glm::vec2 pointA(pointAX, pointAY);
	glm::vec2 pointB(pointBX, pointBY);
	glm::vec2 pointC(pointCX, pointCY);
	renderTriangle(pointA, pointB, pointC, framebuffer, windowDimensions, colour);
}

// This checks to see if the currently bound framebuffer is set up properly.
bool Basic2DGLRenderer::checkFrameBuffer() {
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		AelaErrorHandling::windowError("Aela 2D Rendering",
			"There was a problem setting movingUp the framebuffer.\nIt's probably OpenGL's fault.\nOr maybe your graphics processor is a potato.");
		return false;
	} else {
		return true;
	}
}