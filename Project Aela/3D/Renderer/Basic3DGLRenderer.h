/*
* Name: Project Aela's Basic 3D GLRenderer
* Author: Robert Ciborowski
* Date: October 2016
* Description: A class as well as its smaller classes used by Aela's GLRenderer to
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

#include <map>

#include "../../Error Handler/ErrorHandler.h"
#include "../../Old Garbage/shader.hpp"
#include "../Particles/ParticleEmitter.h"
#include "../../Window/Window.h"
#include "../Camera/Camera3D.h"
#include "Basic3DGLModelRenderer.h"
#include "Basic3DGLShadowRenderer.h"
#include "Basic3DGLSkyboxRenderer.h"
#include "../Light/LightEntity.h"
#include "../../2D/Texture/GLImage.h"
#include "../Maps/Map3D.h"

namespace Aela {
	class Basic3DGLRenderer {
		public:
			Basic3DGLRenderer() {
			}

			~Basic3DGLRenderer() {
				// This cleans all VBOs and shaders.
				glDeleteProgram(modelProgramID);
				glDeleteProgram(depthProgramID);
			}

			// These are some functions related to rendering.
			void renderShadows(Map3D* map);
			void renderModelEntities(Map3D* map, bool multisampling);

			void clearColourFrameBuffer(bool multisampling);
			void renderTextureIn3DSpace(GLuint* texture, bool cullTexture, glm::vec3* position, glm::vec3* scaling, glm::vec3* lookAt, bool inverseRotation, bool multisampling);
			void renderBillboard(BillboardEntity* billboard, bool multisampling);
			void clearShadowMaps();
			void sendLightDataToShader();
			void renderSkybox(Skybox* skybox, bool multisampling);
			void renderParticles(ParticleEmitter* particleEmitter, Camera3D* camera, bool multisampling);

			// These are some functions related to setup.
			void setup(unsigned int multisampling);
			void bindLights(std::unordered_map<long long, LightEntity>* lights);
			void generateShadowMap(LightEntity* light);

			// These are some getters and setters.
			Window* getWindow();
			void setWindow(Window* setWindow);
			void setCamera(Camera3D* camera);
			GLuint* getColourFrameBuffer();
			Image* getColourFrameBufferTexture();
			GLuint* getMultisampledColourFrameBuffer();
			Image* getMultisampledColourFrameBufferTexture();

			// Although this function is called in setup(), it should also be called to change the MSAA amount.
			void setupFrameBuffers(unsigned int multisampling);

			// This will recreate all framebuffers.
			void rebuildFrameBuffers(bool multisampling);

		private:
			// These are the smaller renderers that the Basic3DGLRenderer uses.
			Basic3DGLShadowRenderer shadowRenderer;
			Basic3DGLModelRenderer modelRenderer;
			Basic3DSkyboxRenderer skyboxRenderer;

			// These are a bunch of handles to GLSL variables that get passed to the shadow and
			// model renderer during rendering.
			GLuint depthProgramID, modelProgramID, billboardProgramID, skyboxProgramID;
			GLuint modelTextureID, projectionMatrixID, depthMatrixID, viewMatrixID, modelMatrixID, rotationMatrixID, cameraPositionID, shadowMapID, shadowMatrixID, shadowModelMatrixID;
			GLuint billboardTextureID, billboardMVPMatrixID;
			GLuint skyboxTextureID, skyboxViewMatrixID, skyboxProjectionMatrixID;
			GLuint numberOfLightsID, lightPositionsID, lightDirectionsID, lightColoursID, lightPowersID, lightShadowPositionsID;

			// These properties are used for the framebuffers.
			GLuint multisampledColourFrameBuffer, colourFrameBuffer, depthRenderBuffer;
			GLImage multisampledColourFrameBufferTexture, colourFrameBufferTexture;

			// These are some Project Aela objects that the 3D renderer uses.
			Window* window = nullptr;
			Camera3D* camera = nullptr;

			// This stores the lights to render.
			std::unordered_map<long long, LightEntity>* lights;

			// These are used by the renderer so that "window->getDimensions()->getWidth()"
			// (and "...getHeight()") does not have to called all the time.
			int windowWidth, windowHeight;

			// These variables store the resolution of the depth textures.
			const unsigned int DEPTH_TEXTURE_WIDTH = 2048, DEPTH_TEXTURE_HEIGHT = 2048;

			// This stores the limit to the number of instances that may occur, based on potential hardware limitations.
			const size_t SHADOW_INSTANCE_LIMIT = 100;
			const size_t MODEL_INSTANCE_LIMIT = 100;

			// These are some setup related functions.
			void setupShaders();
			void getIDs();

			void renderParticle(Particle particle, glm::vec3* positionOffset, glm::vec3* rotationOffset, glm::vec3* scalingOffset,
				bool multisampling);
	};
};