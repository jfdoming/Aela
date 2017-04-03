/*
* Class: Project Aela's 2D Renderer
* Author: Robert Ciborowski
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
#include "../../Utilities/Rect/Rect.h"
#include "../../3D/Texture/Texture.h"
#include "../Text/TextFont.h"
#include "../../Utilities/Colour/ColourRGBA.h"

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
		void renderTextureToBuffer(Texture* texture, Rect<unsigned int>* windowDimensions, GLuint frameBuffer);
		void renderTextureToBuffer(Texture* texture, Rect<unsigned int>* windowDimensions, GLuint frameBuffer, GLuint customShader);
		void renderTextureTo2DBuffer(Texture* texture, Rect<unsigned int>* windowDimensions);
		void renderTextTo2DBuffer(std::string text, TextFont* textFont, Rect<int>* output, Rect<unsigned int>* windowDimensions, ColourRGBA* colour,
			unsigned int pointsPerPixel);

		// These are some useful, self-explanatory functions.
		void clearFrameBuffer();
		void drawTestQuad();
		bool checkFrameBuffer();

		// These are some getters.
		GLuint* getFrameBufffer();
		Texture* getFrameBufferTexture();

	private:
		// These are handles to shaders.
		GLuint bufferTextureToBufferProgramID, textToBufferProgramID, imageToBufferProgramID;
		// These are handles to variables inside of the image shader.
		GLuint imageTextureID, imageQuadVertexBufferID, imageTopLeftBufferID, imageWidthAndHeightBufferID, imageDimensionsBufferID,
			imageWindowDimensionsBufferID;
		// These are handles to variables inside of the text shader as well as the actual texture used for text.
		GLuint characterTextureID, characterTexture, characterQuadVertexBufferID, characterTopLeftBufferID, characterWidthAndHeightBufferID,
			characterDimensionsBufferID, characterWindowDimensionsBufferID, characterColourBufferID;

		// These properties are related to the framebuffer.
		GLuint frameBuffer;
		Texture frameBufferTexture;

		// This function is used to render a single character.
		void renderCharacter(char* character, Rect<int>* output, Rect<unsigned int>* windowDimensions, FT_GlyphSlot glyph, ColourRGBA* colour);

		// These functions are used in the setup of the 2D renderer.
		void load2DShaders();
		void getGLSLVariableHandles();
		void generateTexturesAndFrameBuffer();
};
