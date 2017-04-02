/*
* Name: Project Aela's Basic 3D Renderer
* Author: Robert Ciborowski
* Date: October 2016
* Description: A class as well as its smaller classes used by Aela's Renderer to
*              render 3D objects.
*/

#pragma once
#include <glm/gtc/matrix_transform.hpp>

// This makes GLEW Static to avoid errors.
#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif

// This includes GLEW.
#include <GL/glew.h>

#include "../../Error Handler/ErrorHandler.h"
#include "../../Old Garbage/shader.hpp"
#ifndef AELA_MODEL
#define AELA_MODEL
#include "../Models/Models.h"
#endif
#include "../Billboards/Billboards.h"
#include "../../Window/Window.h"
#include "../3D Camera/Camera3D.h"
#include "Basic3DModelRenderer.h"
#include "Basic3DShadowRenderer.h"

class Basic3DRenderer {
	public:
		Basic3DRenderer() {
			depthFrameBuffer = 0;
		}

		~Basic3DRenderer() {
			// This cleans all VBOs and shaders.
			glDeleteProgram(programID);
			glDeleteProgram(depthProgramID);
			glDeleteProgram(quad_programID);
			glDeleteFramebuffers(1, &depthFrameBuffer);
			glDeleteTextures(1, &depthTexture);
			glDeleteBuffers(1, &quad_vertexbuffer);
			glDeleteVertexArrays(1, &vertexArrayID);
		}

		// These are some functions related to rendering.
		void renderShadows(Model3D* model);
		void renderModels(Model3D* model);
		void clearColourFrameBuffer();
		void renderTextureIn3DSpace(GLuint* texture, bool cullTexture, glm::vec3 position, glm::vec3 lookAt, bool inverseRotation);
		void renderBillboard(Billboard* billboard);

		// These are some functions related to setup.
		void setup();
		void setWindow(Window* setWindow);
		void setCamera(Camera3D* camera);
		void resetDepthTexture();
		// These are some getters.
		Window* getWindow();
		GLuint* getColourFrameBuffer();
		Texture* getColourFrameBufferTexture();

	private:
		// These are the smaller renderers that the Basic3DRenderer uses.
		Basic3DShadowRenderer modelShader;
		Basic3DModelRenderer textureRenderer;

		// These are a bunch of handles to GLSL variables that get passed to the shadow and
		// model renderer during rendering.
		GLuint vertexArrayID, depthProgramID, depthMatrixID, quad_programID, texID, programID, billboardProgramID;
		GLuint textureID, matrixID, viewMatrixID, modelMatrixID, depthBiasID, shadowMapID, lightInvDirID;
		GLuint depthTexture;
		GLuint quad_vertexbuffer;

		// These properties are used for the framebuffers.
		GLuint colourFrameBuffer, depthFrameBuffer, depthRenderBuffer;
		Texture colourFrameBufferTexture;

		// These are some Project Aela objects that the 3D renderer uses.
		Window* window;
		Camera3D* camera;

		// These are used by the renderer so that "window->getWindowDimensions()->getWidth()"
		// (and "...getHeight()") does not have to called all the time.
		int windowWidth, windowHeight;

		// These are some setup related functions.
		void setupVertexArrayID();
		void setupShaders();
		void getIDs();
		void setupFrameBuffers();
};