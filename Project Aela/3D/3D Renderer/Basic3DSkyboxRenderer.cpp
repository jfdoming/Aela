/*
* Name: Project Aela's Basic 3D Skybox Renderer
* Author: Robert Ciborowski
* Date: 21/06/2017
* Description: A class used by Project Aela's Basic 3D Renderer to render a skybox using a cubemap.
*/

#include "Basic3DSkyboxRenderer.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

void Basic3DSkyboxRenderer::renderSkybox(Skybox* skybox, GLuint skyboxProgramID, GLuint frameBuffer, GLuint skyboxID, GLuint modelViewMatrixID, GLuint projectionMatrixID) {
	glUseProgram(skyboxProgramID);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	glDisable(GL_CULL_FACE);

	float vertexValues[] = {
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f,  1.0f
	};

	// This loads buffers.
	GLuint vertexBuffer, vertexArray;
	glGenBuffers(1, &vertexBuffer);
	glGenVertexArrays(1, &vertexArray);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBindVertexArray(vertexArray);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexValues), &vertexValues, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glVertexAttribPointer(
		// Attribute.
		0,
		// Size.
		3,
		// Type.
		GL_FLOAT,
		// Is the vertex normalized?
		GL_FALSE,
		// Stride.
		3 * sizeof(float),
		// Array buffer offset.
		(void*) 0
	);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, *skybox->getTexture());
	glUniform1i(skyboxID, 0);

	// This is a really funny way to remove the fourth element of the view matrix (to remove positioning).
	glm::mat4 viewMatrix2 = glm::mat4(glm::mat3(viewMatrix));

	glUniformMatrix4fv(modelViewMatrixID, 1, GL_FALSE, &viewMatrix2[0][0]);
	glUniformMatrix4fv(projectionMatrixID, 1, GL_FALSE, &projectionMatrix[0][0]);

	glDepthFunc(GL_LEQUAL);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glDeleteBuffers(1, &vertexBuffer);
	glDeleteVertexArrays(1, &vertexArray);
	
}

void Basic3DSkyboxRenderer::setMatrices(glm::mat4 setViewMatrix, glm::mat4 setProjectionMatrix) {
	viewMatrix = setViewMatrix;
	projectionMatrix = setProjectionMatrix;
}