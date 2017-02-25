#include <vector>
#include <string>
#include <glm/glm.hpp>
// #include <glm/gtc/matrix_transform.hpp>

// This makes GLEW Static to avoid errors.
#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif

// This includes GLEW.
#include <GL/glew.h>

#include "Window.h"
#include "ErrorHandler.h"
#include "Rect.h"
#include "shader.hpp"
#include "Texture.h"

// This space is left empty for implementation.

class Basic2DRenderer {
	public:
		Basic2DRenderer() {

		}

		void setup();
		void renderTexture(Texture* texture, Rect<int>* windowDimensions);

	private:
		GLuint programID, textureID, positionID, quad_VertexArrayID, quad_vertexbuffer, textureDimensionsID, windowDimensionsID, boundingBoxID;
};
