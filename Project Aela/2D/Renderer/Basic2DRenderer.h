/*
* Class: Project Aela's 2D Renderer
* Author: Robert Ciborowski
* Date: February 2017
* Description: A class used by Aela's Renderer to render 2D objects.
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
#include "../../2D/Texture/Texture.h"
#include "../Simple 2D Framebuffer/Simple2DFramebuffer.h"
#include "../Text/TextFont.h"
#include "../../Utilities/Colour/ColourRGBA.h"
#include "../../Window/Window.h"

// This is the Basic2DRenderer class.
namespace Aela {
	class Basic2DRenderer {
		public:
			Basic2DRenderer() {

			}

			~Basic2DRenderer() {

			}

			// This is the setup function.
			void setup();

			// These functions are accessible to Project Aela's main renderer in order to render 2D components.
			void renderTextureToSimple2DFramebuffer(Texture* texture, Simple2DFramebuffer* framebuffer, Rect<unsigned int>* windowDimensions);
			void renderTextureToSimple2DFramebuffer(Texture* texture, Simple2DFramebuffer* framebuffer, Rect<unsigned int>* windowDimensions, GLuint customShader);
			void renderTextureToFramebuffer(Texture* texture, GLuint framebuffer, Rect<unsigned int>* windowDimensions);
			void renderTextureToFramebuffer(Texture* texture, GLuint framebuffer, Rect<unsigned int>* windowDimensions, GLuint customShader);
			void renderTextToSimple2DFramebuffer(std::string text, TextFont* textFont, Simple2DFramebuffer* framebuffer, Rect<int>* output, Rect<unsigned int>* windowDimensions, ColourRGBA* colour,
				unsigned int pointsPerPixel);
			void renderMultisampledBufferToBuffer(GLuint multisampledBuffer, GLuint secondaryBuffer, Rect<unsigned int>* windowDimensions);
			void renderRectangle(Rect<int>* output, Simple2DFramebuffer* framebuffer, Rect<unsigned int>* windowDimensions, ColourRGBA* colour);
			void renderRectangle(unsigned int xPosition, unsigned int yPosition, int width, int height, Simple2DFramebuffer* framebuffer, Rect<unsigned int>* windowDimensions, ColourRGBA* colour);
			void renderTriangle(glm::vec2 pointA, glm::vec2 pointB, glm::vec2 pointC, Simple2DFramebuffer* framebuffer, Rect<unsigned int>* windowDimensions, ColourRGBA* colour);
			void renderTriangle(unsigned int pointAX, unsigned int pointAY, unsigned int pointBX, unsigned int pointBY, unsigned int pointCX, unsigned int pointCY,
				Simple2DFramebuffer* framebuffer, Rect<unsigned int>* windowDimensions, ColourRGBA* colour);

			// These are some useful, self-explanatory functions.
			void drawTestQuad();
			bool checkFrameBuffer();

			// Although this function is called in setup(), it should be called to change the MSAA amount.
			void setupSimple2DFramebuffer(Simple2DFramebuffer* framebuffer, unsigned int multisampling, Rect<int>* dimensions, Rect<int>* output);

			void clearSimple2DFramebuffer(Simple2DFramebuffer* framebuffer);

			// These are some getters and setters.
			void setWindow(Window* setWindow);
			Window* getWindow();

		private:
			// These are handles to shaders.
			GLuint bufferTextureToBufferProgramID, textToBufferProgramID, imageToBufferProgramID;
			// These are handles to variables inside of the image shader.
			GLuint imageTextureID, imageQuadVertexBufferID, imageTopLeftBufferID, imageWidthAndHeightBufferID, imageDimensionsBufferID,
				imageWindowDimensionsBufferID;
			// These are handles to variables inside of the text shader as well as the actual texture used for text.
			GLuint characterTextureID, characterTexture, characterQuadVertexBufferID, characterTopLeftBufferID, characterWidthAndHeightBufferID,
				characterDimensionsBufferID, characterWindowDimensionsBufferID, characterColourBufferID;

			// This function is used to render a single character.
			void renderCharacter(char* character, Simple2DFramebuffer* framebuffer, Rect<int>* output, Rect<unsigned int>* windowDimensions, FT_GlyphSlot glyph, ColourRGBA* colour);

			// These functions are used in the setup of the 2D renderer.
			void load2DShaders();
			void getGLSLVariableHandles();

			// These are some Project Aela objects that the 3D renderer uses.
			Window* window;

			// These are used by the renderer so that "window->getWindowDimensions()->getWidth()"
			// (and "...getHeight()") does not have to called all the time.
			int windowWidth, windowHeight;
	};
}
