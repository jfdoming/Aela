/*
* Name: Project Aela's Basic 3D Renderer
* Author: Ekkon Games
* Date: October 2016
* Description: A class as well as its smaller classes used by Aela's Renderer to
*              shade 3D models.
*/

#pragma once
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

	void shade(Model3D* model, GLuint depthProgramID, GLuint depthMatrixID, GLuint frameBuffer);
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

	void renderTextures(Model3D* model, GLuint frameBuffer, GLuint programID, GLuint depthMatrixID,
		GLuint matrixID, GLuint modelMatrixID, GLuint viewMatrixID, GLuint depthBiasID, GLuint lightInvDirID,
		GLuint textureID, GLuint depthTexture, GLuint shadowMapID);
	void renderTextureIn3DSpace(Window* window, bool cullFaces, GLuint texture, GLuint textureID,
		GLuint programID, GLuint frameBuffer, GLuint viewMatrixID, GLuint matrixID, GLuint modelMatrixID,
		GLuint depthBiasID, GLuint depthTexture, GLuint shadowMapID, glm::vec3 position, glm::vec3 lookAt, bool inverseRotation);
	void setMatrices(glm::mat4 setViewMatrix, glm::mat4 setProjectionMatrix);

private:
	glm::mat4 biasMatrix;
	glm::mat4 viewMatrix, projectionMatrix;
};

class Basic3DRenderer {
	public:
		Basic3DRenderer() {
			depthFrameBuffer = 0;
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
			glDeleteFramebuffers(1, &depthFrameBuffer);
			glDeleteTextures(1, &depthTexture);
			glDeleteBuffers(1, &quad_vertexbuffer);
			glDeleteVertexArrays(1, &vertexArrayID);
		}

		void shade(Model3D* model);
		void renderTextures(Model3D* model);
		void clearDepthTexture();
		void clearColourFrameBuffer();
		void renderTextureIn3DSpace(GLuint* texture, bool cullTexture, glm::vec3 position, glm::vec3 lookAt, bool inverseRotation);
		void renderBillboard(Billboard* billboard);
		void setupBasicRendering();
		void setWindow(Window* setWindow);
		void setCamera(Camera3D* camera);
		void resetDepthTexture();
		Window* getWindow();
		GLuint* getColourFrameBuffer();
		Texture* getColourFrameBufferTexture();
		int windowWidth, windowHeight;

	private:
		Basic3DModelShader modelShader;
		Basic3DTextureRenderer textureRenderer;

		GLuint vertexArrayID, depthProgramID, depthMatrixID, quad_programID, texID, programID;
		GLuint textureID, matrixID, viewMatrixID, modelMatrixID, depthBiasID, shadowMapID, lightInvDirID;
		GLuint depthTexture;
		GLuint quad_vertexbuffer;

		// These properties are used for the framebuffers.
		GLuint colourFrameBuffer, depthFrameBuffer, depthRenderBuffer;
		Texture colourFrameBufferTexture;

		Window* window;
		Camera3D* camera;

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
		void setupQuadVertexBuffer();
		void setupFrameBuffers();
};