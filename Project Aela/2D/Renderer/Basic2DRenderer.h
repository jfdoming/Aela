/*
* Class: Project Aela's 2D Renderer
* Author: Robert Ciborowski
* Date: 02/12/2017
* Description: A class used by Aela's Renderer to render 2D objects.
*/

// These are the includes.
#include <vector>
#include <string>
// #include <glm/glm.hpp>

// These are Project Aela headers.
#include "../../Utilities/Rect/Rect.h"
#include "../../2D/Texture/GLImage.h"
#include "../Simple 2D Framebuffer/Simple2DFramebuffer.h"
#include "../Text/TextFont.h"
#include "../../Utilities/Colour/ColourRGBA.h"
#include "../../Window/Window.h"

// This is the Basic2DRenderer class.
namespace Aela {
	class Basic2DRenderer {
		public:
			Basic2DRenderer() {}

			~Basic2DRenderer() {}

			// This is the setup function.
			virtual void setup() = 0;

			// These functions are accessible to Project Aela's main renderer in order to render 2D components.
			virtual void renderImageToSimple2DFramebuffer(Image* image, Simple2DFramebuffer* framebuffer, Rect<int>* output, Rect<int>* cropping, Rect<unsigned int>* windowDimensions, ColourRGBA* tint) = 0;
			virtual void renderImageToSimple2DFramebuffer(Image* image, Simple2DFramebuffer* framebuffer, Rect<int>* output, Rect<int>* cropping, Rect<unsigned int>* windowDimensions, ColourRGBA* tint, unsigned int customShader) = 0;
			virtual void renderImageToFramebuffer(Image* image, unsigned int framebuffer, Rect<int>* output, Rect<int>* cropping, Rect<unsigned int>* windowDimensions, ColourRGBA* tint) = 0;
			virtual void renderImageToFramebuffer(Image* image, unsigned int framebuffer, Rect<int>* output, Rect<int>* cropping, Rect<unsigned int>* windowDimensions, ColourRGBA* tint, unsigned int customShader) = 0;
			virtual void renderTextToSimple2DFramebuffer(std::string text, TextFont* textFont, Simple2DFramebuffer* framebuffer, Rect<int>* output, Rect<unsigned int>* windowDimensions, ColourRGBA* colour,
				unsigned int pointsPerPixel) = 0;
			virtual void renderMultisampledBufferToBuffer(unsigned int multisampledBuffer, unsigned int secondaryBuffer, Rect<unsigned int>* windowDimensions) = 0;
			virtual void renderRectangle(Rect<int>* output, Simple2DFramebuffer* framebuffer, Rect<unsigned int>* windowDimensions, ColourRGBA* colour) = 0;
			virtual void renderRectangle(unsigned int xPosition, unsigned int yPosition, int width, int height, Simple2DFramebuffer* framebuffer, Rect<unsigned int>* windowDimensions, ColourRGBA* colour) = 0;
			virtual void renderTriangle(glm::vec2 pointA, glm::vec2 pointB, glm::vec2 pointC, Simple2DFramebuffer* framebuffer, Rect<unsigned int>* windowDimensions, ColourRGBA* colour) = 0;
			virtual void renderTriangle(unsigned int pointAX, unsigned int pointAY, unsigned int pointBX, unsigned int pointBY, unsigned int pointCX, unsigned int pointCY,
				Simple2DFramebuffer* framebuffer, Rect<unsigned int>* windowDimensions, ColourRGBA* colour) = 0;

			// These are some useful, self-explanatory functions.
			virtual void drawTestQuad() = 0;
			virtual bool checkFrameBuffer() = 0;

			// Although this function is called in setup(), it should be called to change the MSAA amount.
			virtual void setupSimple2DFramebuffer(Simple2DFramebuffer* framebuffer, unsigned int multisampling, Rect<int>* dimensions, Rect<int>* output) = 0;

			virtual void clearSimple2DFramebuffer(Simple2DFramebuffer* framebuffer) = 0;

			// These are some getters and setters.
			void setWindow(Window* setWindow);
			Window* getWindow();

		protected:
			// These are handles to shaders.
			unsigned int bufferTextureToBufferProgramID, textToBufferProgramID, imageToBufferProgramID;
			// These are handles to variables inside of the image shader. Note: shaders that use the same uniforms and buffers as the
			// 2DTextureBufferToBuffer shader (bufferTextureToBufferProgramID) should set the same locations of uniforms as they are in
			// the 2DTextureBufferToBuffer shader.
			unsigned int imageTextureID, imageVertexBufferID, imageUVBufferID, imageTopLeftCoordID, imageWindowDimensionsID, imageTintID;
			// These are handles to variables inside of the text shader as well as the actual texture used for text.
			unsigned int characterTextureID, characterTexture, characterQuadVertexBufferID, characterTopLeftCoordID,
				characterDimensionsID, characterWindowDimensionsID, characterColourID;

			// These are some Project Aela objects that the 3D renderer uses.
			Window* window = nullptr;

			// These are used by the renderer so that "window->getDimensions()->getWidth()"
			// (and "...getHeight()") does not have to called all the time.
			int windowWidth = 0, windowHeight = 0;
	};
}