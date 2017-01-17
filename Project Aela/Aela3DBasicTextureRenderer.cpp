#include "Aela3DRenderer.h"
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include "AelaControls.h"

// This sets matrices.
void Aela3DBasicTextureRenderer::setMatrices(glm::mat4 setViewMatrix, glm::mat4 setProjectionMatrix) {
	viewMatrix = setViewMatrix;
	projectionMatrix = setProjectionMatrix;
}

void Aela3DBasicTextureRenderer::renderTextures(AelaModel * model, GLuint depthMatrixID, GLuint programID,
	GLuint matrixID, GLuint modelMatrixID, GLuint viewMatrixID, GLuint depthBiasID, GLuint lightInvDirID, GLuint textureID, GLuint depthTexture, GLuint shadowMapID) {

	// This loads buffers.
	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, model->getVertexSize() * sizeof(glm::vec3), model->getVertices(), GL_STATIC_DRAW);

	GLuint uvbuffer;
	glGenBuffers(1, &uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, model->getUVSize() * sizeof(glm::vec2), model->getUVs(), GL_STATIC_DRAW);

	GLuint normalbuffer;
	glGenBuffers(1, &normalbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glBufferData(GL_ARRAY_BUFFER, model->getNormalSize() * sizeof(glm::vec3), model->getNormals(), GL_STATIC_DRAW);

	GLuint elementbuffer;
	glGenBuffers(1, &elementbuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, model->getIndexSize() * sizeof(unsigned short), model->getIndices(), GL_STATIC_DRAW);

	// This binds the framebuffer.
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glEnable(GL_CULL_FACE);

	// This is positioning/rotation of light and the model.
	glm::vec3 lightInvDir = glm::vec3(0.5f, 2, 2);
	glm::vec3 position = model->getPosition();
	glm::vec3 rotation = model->getRotation();

	// This calculates the MVP matrix using the light's point of view. Note: glm::ortho creates a matrix.
	glm::mat4 depthProjectionMatrix = glm::ortho<float>(-10, 10, -10, 10, -10, 20);
	glm::mat4 depthViewMatrix = glm::lookAt(lightInvDir, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

	// USe this if you want a spot light. Otherwise, use the code above.
	//glm::vec3 lightPos(5, 20, 20);
	//glm::mat4 depthProjectionMatrix = glm::perspective<float>(45.0f, 1.0f, 2.0f, 50.0f);
	//glm::mat4 depthViewMatrix = glm::lookAt(lightPos, lightPos-lightInvDir, glm::vec3(0,1,0));

	// These are more matrices.
	glm::mat4 depthModelMatrix = glm::mat4(1.0);
	glm::mat4 depthMVP = depthProjectionMatrix * depthViewMatrix * depthModelMatrix;

	// This sends the transformations to the shader.
	glUniformMatrix4fv(depthMatrixID, 1, GL_FALSE, &depthMVP[0][0]);
	glUseProgram(programID);

	// This computes more matrices.
	glm::mat4 rotationMatrix = glm::eulerAngleYXZ(rotation.y, rotation.x, rotation.z);
	glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0), position) * rotationMatrix;
	glm::mat4 MVP = projectionMatrix * viewMatrix * modelMatrix;
	glm::mat4 depthBiasMVP = biasMatrix * depthMVP;

	// This sends more transformations to the shader.
	glUniformMatrix4fv(matrixID, 1, GL_FALSE, &MVP[0][0]);
	glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, &modelMatrix[0][0]);
	glUniformMatrix4fv(viewMatrixID, 1, GL_FALSE, &viewMatrix[0][0]);
	glUniformMatrix4fv(depthBiasID, 1, GL_FALSE, &depthBiasMVP[0][0]);
	glUniform3f(lightInvDirID, lightInvDir.x, lightInvDir.y, lightInvDir.z);

	// This binds the texture to "slot" zero. A similar thing happens to the depth texture.
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, *model->getTexture());
	glUniform1i(textureID, 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, depthTexture);
	glUniform1i(shadowMapID, 1);

	// These are attributes for the vertex buffer.
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
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
		0,
		// Array buffer offset.
		(void*)0
	);

	// These are attributes for the UV buffer.
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*) 0);

	// These are attributes for the normal buffer.
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);

	// This binds the index buffer.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);

	// This draws the elements to the screen.
	glDrawElements(GL_TRIANGLES, model->getIndexSize(), GL_UNSIGNED_SHORT, (void*) 0);

	// This deletes buffers.
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &uvbuffer);
	glDeleteBuffers(1, &normalbuffer);
	glDeleteBuffers(1, &elementbuffer);
}

// This function renders a 2D texture in 3D space. It could be used for billboards.
// For billboards, use getPositionOfCamera() to make the texture look at the camera.
// To specify a rotation for the camera as a vec3, use the texture's position and add the direction (position + direction) for the lookAt parameter.
// Note: for the lookAt parameter, position + glm::vec3(0.0, 0.0, 1.0) will not rotate the texture. Use this for no rotation.
void Aela3DBasicTextureRenderer::renderTextureIn3DSpace(AelaWindow * window, bool cullFaces, GLuint texture, GLuint textureID, GLuint programID, GLuint viewMatrixID, GLuint matrixID, GLuint modelMatrixID, GLuint depthBiasID, GLuint depthTexture, GLuint shadowMapID, glm::vec3 position, glm::vec3 lookAt, bool inverseRotation) {
	glUseProgram(programID);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	if (cullFaces) {
		glEnable(GL_CULL_FACE);
	} else {
		glDisable(GL_CULL_FACE);
	}

	glm::vec3 vertices[3];
	glm::vec2 uvs[3];

	for (int i = 0; i < 2; i++) {
		if (i == 0) {
			vertices[0] = glm::vec3(1.0, 1.0, 0.0);
			vertices[1] = glm::vec3(1.0, -1.0, 0.0);
			vertices[2] = glm::vec3(-1.0, 1.0, 0.0);
			uvs[0] = glm::vec2(0.0, 0.0);
			uvs[1] = glm::vec2(0.0, 1.0);
			uvs[2] = glm::vec2(1.0, 0.0);
		} else {
			vertices[0] = glm::vec3(-1.0, -1.0, 0.0);
			vertices[1] = glm::vec3(-1.0, 1.0, 0.0);
			vertices[2] = glm::vec3(1.0, -1.0, 0.0);
			uvs[0] = glm::vec2(1.0, 1.0);
			uvs[1] = glm::vec2(1.0, 0.0);
			uvs[2] = glm::vec2(0.0, 1.0);
		}

		// This binds the texture to "slot" zero.
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		glUniform1i(textureID, 0);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, depthTexture);
		glUniform1i(shadowMapID, 1);

		// Buffer generation.
		GLuint vertexbuffer;
		glGenBuffers(1, &vertexbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		GLuint uvbuffer;
		glGenBuffers(1, &uvbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(uvs), uvs, GL_STATIC_DRAW);

		// Computes matrices based on control input.
		glm::mat4 modelMatrix = glm::lookAt(position, lookAt, glm::vec3(0, 1, 0));
		if (inverseRotation) {
			modelMatrix = glm::inverse(modelMatrix);
		}
		glm::mat4 MVP = projectionMatrix * viewMatrix * modelMatrix;

		glUniformMatrix4fv(matrixID, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, &modelMatrix[0][0]);
		glUniformMatrix4fv(viewMatrixID, 1, GL_FALSE, &viewMatrix[0][0]);

		// Vertex buffer attributes.
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			// Attribute
			0,
			// Size
			3,
			// Type
			GL_FLOAT,
			// Is it normalized?
			GL_FALSE,
			// Stride
			0,
			// Array buffer offset.
			(void*)0
		);

		// UV buffer attributes.
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*) 0);

		// This draws triangles!
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// This deletes stuff from the memory.
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDeleteBuffers(1, &vertexbuffer);
		glDeleteBuffers(1, &uvbuffer);
	}
}
