/*
* Class: Project Aela's 2D GLRenderer
* Author: Robert Ciborowski
* Date: February 2017
* Description: A class used by Aela's GLRenderer to render 2D objects.
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
#include "Basic2DRenderer.h"
#include "../../Utilities/Rect/Rect.h"
#include "../../2D/Texture/GLImage.h"
#include "../Simple 2D Framebuffer/Simple2DFramebuffer.h"
#include "../Text/TextFont.h"
#include "../../Utilities/Colour/ColourRGBA.h"
#include "../../Window/Window.h"

// This is the Basic2DGLRenderer class.
namespace Aela {
	class Basic2DGLRenderer : public Basic2DRenderer {
		public:
			Basic2DGLRenderer() {

			}

			~Basic2DGLRenderer() {

			}

			// This is the setup function.
			void setup();

			// These functions are accessible to Project Aela's main renderer in order to render 2D components.
			void renderImageToSimple2DFramebuffer(Image* image, Simple2DFramebuffer* framebuffer, Rect<int>* output, Rect<int>* cropping, Rect<unsigned int>* windowDimensions, ColourRGBA* tint);
			void renderImageToSimple2DFramebuffer(Image* image, Simple2DFramebuffer* framebuffer, Rect<int>* output, Rect<int>* cropping, Rect<unsigned int>* windowDimensions, ColourRGBA* tint, unsigned int customShader);
			void renderImageToFramebuffer(Image* image, unsigned int framebuffer, Rect<int>* output, Rect<int>* cropping, Rect<unsigned int>* windowDimensions, ColourRGBA* tint);
			void renderImageToFramebuffer(Image* image, unsigned int framebuffer, Rect<int>* output, Rect<int>* cropping, Rect<unsigned int>* windowDimensions, ColourRGBA* tint, unsigned int customShader);
			void renderTextToSimple2DFramebuffer(std::string text, TextFont* textFont, Simple2DFramebuffer* framebuffer, Rect<int>* output, Rect<unsigned int>* windowDimensions, ColourRGBA* colour,
				unsigned int pointsPerPixel);
			void renderMultisampledBufferToBuffer(unsigned int multisampledBuffer, unsigned int secondaryBuffer, Rect<unsigned int>* windowDimensions);
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

		private:
			// This function is used to render a single character.
			void renderCharacterBuffer(Simple2DFramebuffer* framebuffer, Rect<int>* output, Rect<unsigned int>* windowDimensions, std::vector<unsigned char>* buffer, unsigned int width, unsigned int rows, ColourRGBA* colour);

			// These functions are used in the setup of the 2D renderer.
			void load2DShaders();
			void getGLSLVariableHandles();
	};
}