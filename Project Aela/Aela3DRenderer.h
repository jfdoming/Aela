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

class Aela3DBasicShadowRenderer {
	public:
		Aela3DBasicShadowRenderer() {

		}

		void renderShadows(AelaModel * model, GLuint depthProgramID, GLuint depthMatrixID);
};

class Aela3DBasicTextureRenderer {
	public:
		Aela3DBasicTextureRenderer() {
			biasMatrix = glm::mat4(
				0.5, 0.0, 0.0, 0.0,
				0.0, 0.5, 0.0, 0.0,
				0.0, 0.0, 0.5, 0.0,
				0.5, 0.5, 0.5, 1.0
			);
		}

		void Aela3DBasicTextureRenderer::renderTextures(AelaModel * model, GLuint depthMatrixID, GLuint programID,
			GLuint matrixID, GLuint modelMatrixID, GLuint viewMatrixID, GLuint depthBiasID, GLuint lightInvDirID, GLuint textureID, GLuint depthTexture, GLuint shadowMapID);

	private:
		glm::mat4 biasMatrix;
};

class Aela3DBasicRenderer {
	public:
		Aela3DBasicRenderer() {
			framebufferName = 0;
		}

		~Aela3DBasicRenderer() {
			// This cleans all VBOs and shaders.
			glDeleteProgram(programID);
			glDeleteProgram(depthProgramID);
			glDeleteProgram(quad_programID);
			glDeleteFramebuffers(1, &framebufferName);
			glDeleteTextures(1, &depthTexture);
			glDeleteBuffers(1, &quad_vertexbuffer);
			glDeleteVertexArrays(1, &vertexArrayID);
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

		GLuint vertexArrayID, depthProgramID, depthMatrixID, quad_programID, texID, programID;
		GLuint textureID, matrixID, viewMatrixID, modelMatrixID, depthBiasID, shadowMapID, lightInvDirID;
		GLuint depthTexture;
		GLuint framebufferName;
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
			models.resize(5);
			models[0].loadTexture("textures/uvmap.DDS");
			models[1].loadTexture("textures/beretta.DDS");
			models[2].loadTexture("textures/mug.dds");
			//models[3].loadTexture("textures/laptop.dds");
			models[3].loadTexture("textures/missile.dds");
			models[4].loadTexture("textures/cat.dds");

			// This laods the models from OBJ files.
			models[0].loadModel("models/room_thickwalls.obj");
			models[1].loadModel("models/beretta.obj");
			models[2].loadModel("models/mug.obj");
			//models[3].loadModel("models/laptop.obj");
			models[3].loadModel("models/missile.obj");
			models[4].loadModel("models/cat.obj");

			// This sets model position.
			models[1].setPosition(10, 0, 0);
			models[2].setPosition(0, 10, 15);
			models[4].setPosition(0, 0, -15);

			setWindow(windowToSet);
			setupRendering();
		}

		~Aela3DRenderer() {
			models.resize(0);
		}

		void render();

	private:
		std::vector<Aela3DRendererFlag> flags;
		Aela3DBasicRenderer basicRenderer;


		// TEMPORARY!
		std::vector<AelaModel> models;

		void addFlag(Aela3DRendererFlag flag);

		void setupRendering();
		void setupGLFeatures();
		bool setupGLEW();

		void setWindow(AelaWindow * setWindow);
		
};