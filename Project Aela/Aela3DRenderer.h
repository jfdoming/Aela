#include <vector>
#include <string>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// This makes GLEW Static to avoid errors.
#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif

// This includes GLEW.
#include <GL/glew.h>

#include "AelaError.h"
#include "shader.hpp"
#include "AelaModels.h"
#include "AelaWindow.h"
#include "AelaError.h"
#include "controls.hpp"
// wtfffrff

class Aela3DBasicShadowRenderer {
	public:
		Aela3DBasicShadowRenderer() {

		}

		void renderShadows(AelaModel * model, GLuint depthProgramID, GLuint depthMatrixID);
};

class Aela3DBasicTextureRenderer {
	public:
		Aela3DBasicTextureRenderer() {

		}

		void Aela3DBasicTextureRenderer::renderTextures(AelaModel * model, GLuint depthMatrixID, GLuint programID,
			GLuint matrixID, GLuint modelMatrixID, GLuint viewMatrixID, GLuint depthBiasID, GLuint lightInvDirID, GLuint textureID, GLuint depthTexture, GLuint shadowMapID);
};

class Aela3DBasicRenderer {
	public:
		Aela3DBasicRenderer() {
			FramebufferName = 0;

		}

		void renderShadows(AelaModel * model);
		void renderTextures(AelaModel * model);
		void setupBasicRendering();
		void setWindow(AelaWindow * setWindow);
		AelaWindow * getWindow();
		GLuint * getFramebuffer();
		int windowWidth, windowHeight;

	private:
		Aela3DBasicShadowRenderer shadowRenderer;
		Aela3DBasicTextureRenderer textureRenderer;

		GLuint VertexArrayID, depthProgramID, depthMatrixID, quad_programID, texID, programID;
		GLuint TextureID, MatrixID, ViewMatrixID, ModelMatrixID, DepthBiasID, ShadowMapID, lightInvDirID;
		GLuint depthTexture;
		GLuint FramebufferName;
		GLuint quad_vertexbuffer;

		AelaWindow * window;

		const GLfloat g_quad_vertex_buffer_data[18] = {
			-1.0f, -1.0f, 0.0f,
			1.0f, -1.0f, 0.0f,
			-1.0f,  1.0f, 0.0f,
			-1.0f,  1.0f, 0.0f,
			1.0f, -1.0f, 0.0f,
			1.0f,  1.0f, 0.0f,
		};

		void setupVertexArrayID();
		void setupShaders();
		void getIDs();
		void setupDepthTexture();
		void setupQuadVertexBuffer();
		void getFrameBufferName();
};

enum class Aela3DRendererFlag {
	RENDER_SHADOWS, RENDER_TEXTURES
};

class Aela3DRenderer {
	public:
		Aela3DRenderer(AelaWindow * windowToSet) {
			// TEMPORARY! This won't exist once models are moved elsewhere.
			models.resize(2);
			models[0].loadTexture("textures/uvmap.DDS");
			models[1].loadTexture("textures/beretta.DDS");

			// This laods the models from OBJ files.
			models[0].loadModel("models/room_thickwalls.obj");
			models[1].loadModel("models/beretta.obj");
			setWindow(windowToSet);
			setupRendering();

		}

		void render();

	private:
		std::vector<Aela3DRendererFlag> flags;
		Aela3DBasicRenderer basicRenderer;


		// TEMPORARY!
		std::vector<AelaModel> models;

		void addFlag(Aela3DRendererFlag flag);
		void renderShadows();
		void renderTextures();

		void setupRendering();
		void setupGLFeatures();
		bool setupGLEW();

		void setWindow(AelaWindow * setWindow);
		
};