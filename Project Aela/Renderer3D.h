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

#include "ErrorHandler.h"
#include "shader.hpp"
#include "Models.h"
#include "Billboards.h"
#include "Window.h"
#include "Camera3D.h"
#include "ControlManager.h"

class Basic3DModelShader {
	public:
		Basic3DModelShader() {

		}

		void shade(Model3D * model, GLuint depthProgramID, GLuint depthMatrixID);
};

class Basic3DTextureRenderer {
	public:
		Basic3DTextureRenderer() {
			biasMatrix = glm::mat4(
				0.5, 0.0, 0.0, 0.0,
				0.0, 0.5, 0.0, 0.0,
				0.0, 0.0, 0.5, 0.0,
				0.5, 0.5, 0.5, 1.0
			);
		}

		void renderTextures(Model3D * model, GLuint depthMatrixID, GLuint programID,
			GLuint matrixID, GLuint modelMatrixID, GLuint viewMatrixID, GLuint depthBiasID, GLuint lightInvDirID, GLuint textureID, GLuint depthTexture, GLuint shadowMapID);
		void renderTextureIn3DSpace(Window * window, bool cullFaces, GLuint texture, GLuint textureID, GLuint programID, GLuint viewMatrixID, GLuint matrixID, GLuint modelMatrixID, GLuint depthBiasID, GLuint depthTexture, GLuint shadowMapID, glm::vec3 position, glm::vec3 lookAt, bool inverseRotation);
		void setMatrices(glm::mat4 setViewMatrix, glm::mat4 setProjectionMatrix);

	private:
		glm::mat4 biasMatrix;
		glm::mat4 viewMatrix, projectionMatrix;
};

class Basic3DRenderer {
	public:
		Basic3DRenderer() {
			framebufferName = 0;
			flipMatrix = glm::mat3(
				-1.0, 0.0, 0.0,
				0.0, -1.0, 0.0,
				0.0, 0.0, -1.0
			);
		}

		~Basic3DRenderer() {
			// This cleans all VBOs and shaders.
			glDeleteProgram(programID);
			glDeleteProgram(depthProgramID);
			glDeleteProgram(quad_programID);
			glDeleteFramebuffers(1, &framebufferName);
			glDeleteTextures(1, &depthTexture);
			glDeleteBuffers(1, &quad_vertexbuffer);
			glDeleteVertexArrays(1, &vertexArrayID);
		}

		void shade(Model3D * model);
		void renderTextures(Model3D * model);
		void renderTextureIn3DSpace(GLuint * texture, bool cullTexture, glm::vec3 position, glm::vec3 lookAt, bool inverseRotation);
		void renderBillboard(Billboard * billboard);
		void setupBasicRendering();
		void setWindow(Window * setWindow);
		void setCamera(Camera3D * camera);
		Window * getWindow();
		GLuint * getFramebuffer();
		int windowWidth, windowHeight;

	private:
		Basic3DModelShader modelShader;
		Basic3DTextureRenderer textureRenderer;

		GLuint vertexArrayID, depthProgramID, depthMatrixID, quad_programID, texID, programID;
		GLuint textureID, matrixID, viewMatrixID, modelMatrixID, depthBiasID, shadowMapID, lightInvDirID;
		GLuint depthTexture;
		GLuint framebufferName;
		GLuint quad_vertexbuffer;

		Window * window;
		Camera3D * camera;

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

enum class Renderer3DFlag {
	RENDER_SHADOWS, RENDER_TEXTURES
};

class Renderer3D {
	public:
		Renderer3D() {
			
		}

		Renderer3D(Window * windowToSet) {
			temporarilySetupModels();
			setWindow(windowToSet);
			basicRenderer.setCamera(&camera);
			setupRendering();
		}

		~Renderer3D() {
			models.resize(0);
		}

		void setup(Window * windowToSet) {
			temporarilySetupModels();
			setWindow(windowToSet);
			setupRendering();
		}

		void setWindow(Window * setWindow);
		void setTimeManager(TimeManager * setTime);
		void updateCameraUsingControls(ControlManager * controls);
		void render();

	private:
		std::vector<Renderer3DFlag> flags;
		Basic3DRenderer basicRenderer;
		Camera3D camera;
		TimeManager * timeManager;

		// TEMPORARY!
		std::vector<Model3D> models;
		std::vector<Billboard> billboards;

		void addFlag(Renderer3DFlag flag);
		void setupRendering();
		void setupGLFeatures();
		bool setupGLEW();
		void temporarilySetupModels() {
			// TEMPORARY! This won't exist once models are moved elsewhere.
			models.resize(6);
			models[0].loadTexture("res/textures/uvmap.DDS");
			models[1].loadTexture("res/textures/beretta.DDS");
			models[2].loadTexture("res/textures/mug.dds");
			models[3].loadTexture("res/textures/missile.dds");
			models[4].loadTexture("res/textures/cat.dds");
			models[5].loadTexture("res/textures/big_marble.dds");

			// This loads the models from OBJ files.
			models[0].loadModel("res/models/room_thickwalls.obj");
			models[1].loadModel("res/models/beretta.obj");
			models[2].loadModel("res/models/mug.obj");
			models[3].loadModel("res/models/missile.obj");
			models[4].loadModel("res/models/cat.obj");
			models[5].loadModel("res/models/big_marble.obj");

			// This sets model position.
			models[1].setPosition(10, 0, 0);
			models[2].setPosition(0, 10, 15);
			models[4].setPosition(0, 0, -15);
			models[5].setPosition(10, 20, 10);

			// This sets up a billboard.
			billboards.resize(1);
			billboards[0].loadTexture("res/textures/uvmap.DDS");
		}
		
};