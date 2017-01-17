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
#include "AelaBillboards.h"
#include "AelaWindow.h"
#include "Aela3DCamera.h"
#include "AelaControls.h"

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

		void renderTextures(AelaModel * model, GLuint depthMatrixID, GLuint programID,
			GLuint matrixID, GLuint modelMatrixID, GLuint viewMatrixID, GLuint depthBiasID, GLuint lightInvDirID, GLuint textureID, GLuint depthTexture, GLuint shadowMapID);
		void renderTextureIn3DSpace(AelaWindow * window, bool cullFaces, GLuint texture, GLuint textureID, GLuint programID, GLuint viewMatrixID, GLuint matrixID, GLuint modelMatrixID, GLuint depthBiasID, GLuint depthTexture, GLuint shadowMapID, glm::vec3 position, glm::vec3 lookAt, bool inverseRotation);
		void setMatrices(glm::mat4 setViewMatrix, glm::mat4 setProjectionMatrix);

	private:
		glm::mat4 biasMatrix;
		glm::mat4 viewMatrix, projectionMatrix;
};

class Aela3DBasicRenderer {
	public:
		Aela3DBasicRenderer() {
			framebufferName = 0;
			flipMatrix = glm::mat3(
				-1.0, 0.0, 0.0,
				0.0, -1.0, 0.0,
				0.0, 0.0, -1.0
			);
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
		void renderTextureIn3DSpace(GLuint * texture, bool cullTexture, glm::vec3 position, glm::vec3 lookAt, bool inverseRotation);
		void renderBillboard(AelaBillboard * billboard);
		void setupBasicRendering();
		void setWindow(AelaWindow * setWindow);
		void setCamera(Aela3DCamera * camera);
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
		Aela3DCamera * camera;

		const GLfloat g_quad_vertex_buffer_data[18] = {
			-1.0f, -1.0f, 0.0f,
			1.0f, -1.0f, 0.0f,
			-1.0f,  1.0f, 0.0f,
			-1.0f,  1.0f, 0.0f,
			1.0f, -1.0f, 0.0f,
			1.0f,  1.0f, 0.0f,
		};

		glm::mat3 flipMatrix;

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
		Aela3DRenderer() {
			
		}

		Aela3DRenderer(AelaWindow * windowToSet) {
			temporarilySetupModels();
			setWindow(windowToSet);
			basicRenderer.setCamera(&camera);
			setupRendering();
		}

		~Aela3DRenderer() {
			models.resize(0);
		}

		void setup(AelaWindow * windowToSet) {
			temporarilySetupModels();
			setWindow(windowToSet);
			setupRendering();
		}

		void setWindow(AelaWindow * setWindow);
		void setTimeManager(AelaTimeManager * setTime);
		void updateCameraUsingControls(AelaControlManager * controls);
		void render();

	private:
		std::vector<Aela3DRendererFlag> flags;
		Aela3DBasicRenderer basicRenderer;
		Aela3DCamera camera;
		AelaTimeManager * timeManager;

		// TEMPORARY!
		std::vector<AelaModel> models;
		std::vector<AelaBillboard> billboards;

		void addFlag(Aela3DRendererFlag flag);
		void setupRendering();
		void setupGLFeatures();
		bool setupGLEW();
		void temporarilySetupModels() {
			// TEMPORARY! This won't exist once models are moved elsewhere.
			models.resize(6);
			models[0].loadTexture("textures/uvmap.DDS");
			models[1].loadTexture("textures/beretta.DDS");
			models[2].loadTexture("textures/mug.dds");
			models[3].loadTexture("textures/missile.dds");
			models[4].loadTexture("textures/cat.dds");
			models[5].loadTexture("textures/big_marble.dds");

			// This loads the models from OBJ files.
			models[0].loadModel("models/room_thickwalls.obj");
			models[1].loadModel("models/beretta.obj");
			models[2].loadModel("models/mug.obj");
			models[3].loadModel("models/missile.obj");
			models[4].loadModel("models/cat.obj");
			models[5].loadModel("models/big_marble.obj");

			// This sets model position.
			models[1].setPosition(10, 0, 0);
			models[2].setPosition(0, 10, 15);
			models[4].setPosition(0, 0, -15);
			models[5].setPosition(10, 20, 10);

			// This sets up a billboard.
			billboards.resize(1);
			billboards[0].loadTexture("textures/uvmap.DDS");
		}
		
};