/*
* Class: Project Aela's 2D Renderer
* Author: Ekkon Games
* Date: February 2017
* Description: A class used by Aela's Renderer to render textures as 2D objects.
*/

// These are the includes.
#include <vector>
#include <string>
#include <glm/glm.hpp>

// This makes GLEW Static to avoid errors.
#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif

// This includes GLEW.
#include <GL/glew.h>

// These are Project Aela headers.
#include "Window.h"
#include "ErrorHandler.h"
#include "Rect.h"
#include "shader.hpp"
#include "Texture.h"
#include "TextFont.h"
#include "ColourRGBA.h"

// This is the Basic2DRenderer class.
class Basic2DRenderer {
	public:
		Basic2DRenderer() {

		}

		~Basic2DRenderer() {

		}

		// This is the setup function.
		void setup();

		// These functions are accessible to Project Aela's main renderer in order to render 2D components.
		void renderTextureToScreen(Texture* texture, Rect<unsigned int>* windowDimensions);
		void renderTextureToBuffer(Texture* texture, Rect<unsigned int>* windowDimensions);
		void renderTextToBuffer(std::string text, TextFont* textFont, Rect<int>* output, Rect<unsigned int>* windowDimensions, ColourRGBA* colour,
			unsigned int pointsPerPixel);

		// These are some useful self-explanatory functions.
		void clearFrameBuffer();
		void drawTestQuad();
		bool checkFrameBuffer();

		GLuint* getFrameBufffer();
		Texture* getFrameBufferTexture();

	private:
		// These are handles to shaders.
		GLuint bufferToScreenProgramID, textToBufferProgramID, imageToBufferProgramID;
		// These are handles to variables inside of the image shader.
		GLuint imageTextureID, imageQuadVertexBufferID, imageTopLeftBufferID, imageWidthAndHeightBufferID, imageDimensionsBufferID,
			imageWindowDimensionsBufferID;
		// These are handles to variables inside of the text shader as well as the actual texture used for text.
		GLuint characterTextureID, characterTexture, characterQuadVertexBufferID, characterTopLeftBufferID, characterWidthAndHeightBufferID,
			characterDimensionsBufferID, characterWindowDimensionsBufferID, characterColourBufferID;

		// These properties are related to the framebuffer.
		GLuint frameBuffer;
		Texture frameBufferTexture;

		// This function is used internally to render a single character.
		void renderCharacter(char* character, Rect<int>* output, Rect<unsigned int>* windowDimensions, FT_GlyphSlot glyph, ColourRGBA* colour);
};
