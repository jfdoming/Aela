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
#include "../Billboards/Billboards.h"
#include "../../Window/Window.h"
#include "../3D Camera/Camera3D.h"
#include "Basic3DModelRenderer.h"
#include "Basic3DShadowRenderer.h"
#include "Basic3DSkyboxRenderer.h"
#include "../3D Light/Light3D.h"

class Basic3DRenderer {
	public:
		Basic3DRenderer() {
		}

		~Basic3DRenderer() {
			// This cleans all VBOs and shaders.
			glDeleteProgram(modelProgramID);
			glDeleteProgram(depthProgramID);
			glDeleteBuffers(1, &quad_vertexbuffer);
		}

		// These are some functions related to rendering.
		void renderShadow(Model3D* model);
		void renderModel(Model3D* model);
		void clearColourFrameBuffer();
		void renderTextureIn3DSpace(GLuint* texture, bool cullTexture, glm::vec3 position, glm::vec3 lookAt, bool inverseRotation);
		void renderBillboard(Billboard* billboard);
		void bindLights(std::vector<Light3D>* lights);
		void clearShadowMaps();
		void renderLights();
		void renderSkybox(Skybox* skybox);

		// These are some functions related to setup.
		void setup();
		void setWindow(Window* setWindow);
		void setCamera(Camera3D* camera);
		void generateShadowMap(Light3D* light);
		// These are some getters.
		Window* getWindow();
		GLuint* getColourFrameBuffer();
		Texture* getColourFrameBufferTexture();

	private:
		// These are the smaller renderers that the Basic3DRenderer uses.
		Basic3DShadowRenderer shadowRenderer;
		Basic3DModelRenderer modelRenderer;
		Basic3DSkyboxRenderer skyboxRenderer;

		// These are a bunch of handles to GLSL variables that get passed to the shadow and
		// model renderer during rendering.
		GLuint depthProgramID, modelProgramID, billboardProgramID, skyboxProgramID;
		GLuint modelTextureID, modelMVPMatrixID, depthMatrixID, modelViewMatrixID, modelMatrixID, shadowMapID, shadowMatrixID, shadowModelMatrixID;
		GLuint billboardTextureID, billboardMVPMatrixID;
		GLuint skyboxTextureID, skyboxViewMatrixID, skyboxProjectionMatrixID;
		GLuint numberOfLightsID, lightPositionsID, lightDirectionsID, lightColoursID, lightPowersID, lightShadowPositionsID;
		GLuint quad_vertexbuffer;

		// These properties are used for the framebuffers.
		GLuint colourFrameBuffer, depthRenderBuffer;
		Texture colourFrameBufferTexture;

		// These are some Project Aela objects that the 3D renderer uses.
		Window* window;
		Camera3D* camera;

		// This is temporarily here. It stores the lights of the program.
		std::vector<Light3D>* lights;

		// These are used by the renderer so that "window->getWindowDimensions()->getWidth()"
		// (and "...getHeight()") does not have to called all the time.
		int windowWidth, windowHeight;

		const unsigned int DEPTH_TEXTURE_WIDTH = 2048, DEPTH_TEXTURE_HEIGHT = 2048;

		// These are some setup related functions.
		void setupShaders();
		void getIDs();
		void setupFrameBuffers();
};