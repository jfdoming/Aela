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
#include "../Particles/ParticleEmitter.h"
#include "../../Window/Window.h"
#include "../Camera/Camera3D.h"
#include "Basic3DModelRenderer.h"
#include "Basic3DShadowRenderer.h"
#include "Basic3DSkyboxRenderer.h"
#include "../Light/Light3D.h"

namespace Aela {
	class Basic3DRenderer {
		public:
			Basic3DRenderer() {
			}

			~Basic3DRenderer() {
				// This cleans all VBOs and shaders.
				glDeleteProgram(modelProgramID);
				glDeleteProgram(depthProgramID);
			}

			// These are some functions related to rendering.
			void renderShadow(Entity3D* entity);
			void render3DEntity(Entity3D* entity, bool multisampling);
			void clearColourFrameBuffer(bool multisampling);
			void renderTextureIn3DSpace(GLuint* texture, bool cullTexture, glm::vec3* position, glm::vec3* scaling, glm::vec3* lookAt, bool inverseRotation, bool multisampling);
			void renderBillboard(Billboard* billboard, bool multisampling);
			void clearShadowMaps();
			void sendLightDataToShader();
			void renderSkybox(Skybox* skybox, bool multisampling);
			void renderParticles(ParticleEmitter* particleEmitter, Camera3D* camera, bool multisampling);

			// These are some functions related to setup.
			void setup(unsigned int multisampling);
			void bindLights(std::vector<Light3D>* lights);
			void generateShadowMap(Light3D* light);

			// These are some getters and setters.
			Window* getWindow();
			void setWindow(Window* setWindow);
			void setCamera(Camera3D* camera);
			GLuint* getColourFrameBuffer();
			Texture* getColourFrameBufferTexture();
			GLuint* getMultisampledColourFrameBuffer();
			Texture* getMultisampledColourFrameBufferTexture();

			// Although this function is called in setup(), it should also be called to change the MSAA amount.
			void setupFrameBuffers(unsigned int multisampling);

		private:
			// These are the smaller renderers that the Basic3DRenderer uses.
			Basic3DShadowRenderer shadowRenderer;
			Basic3DModelRenderer modelRenderer;
			Basic3DSkyboxRenderer skyboxRenderer;

			// These are a bunch of handles to GLSL variables that get passed to the shadow and
			// model renderer during rendering.
			GLuint depthProgramID, modelProgramID, billboardProgramID, skyboxProgramID;
			GLuint modelTextureID, modelMVPMatrixID, depthMatrixID, modelViewMatrixID, modelMatrixID, cameraPositionID, shadowMapID, shadowMatrixID, shadowModelMatrixID;
			GLuint billboardTextureID, billboardMVPMatrixID;
			GLuint skyboxTextureID, skyboxViewMatrixID, skyboxProjectionMatrixID;
			GLuint numberOfLightsID, lightPositionsID, lightDirectionsID, lightColoursID, lightPowersID, lightShadowPositionsID;

			// These properties are used for the framebuffers.
			GLuint multisampledColourFrameBuffer, colourFrameBuffer, depthRenderBuffer;
			Texture multisampledColourFrameBufferTexture, colourFrameBufferTexture;

			// These are some Project Aela objects that the 3D renderer uses.
			Window* window;
			Camera3D* camera;

			// This stores the lights to render.
			std::vector<Light3D>* lights;

			// These are used by the renderer so that "window->getWindowDimensions()->getWidth()"
			// (and "...getHeight()") does not have to called all the time.
			int windowWidth, windowHeight;

			// These variables store the resolution of the depth textures.
			const unsigned int DEPTH_TEXTURE_WIDTH = 2048, DEPTH_TEXTURE_HEIGHT = 2048;

			// These are some setup related functions.
			void setupShaders();
			void getIDs();

			void renderParticle(Particle particle, glm::vec3* positionOffset, glm::vec3* rotationOffset, glm::vec3* scalingOffset,
				bool multisampling);
	};
};