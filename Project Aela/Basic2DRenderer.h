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

// This space is left empty for implementation.

class Basic2DRenderer {
	public:
		Basic2DRenderer() {

		}

		void setup();
		void renderTexture(GLuint texture, Rect<int> * windowDimensions, Rect<int> * output);

	private:
		GLuint programID, textureID, positionID;
};
