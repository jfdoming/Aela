#include "Aela3DRenderer.h"

void Aela3DBasicTextureRenderer::renderTextures(AelaModel * model, GLuint depthMatrixID, GLuint programID,
	GLuint matrixID, GLuint modelMatrixID, GLuint viewMatrixID, GLuint depthBiasID, GLuint lightInvDirID, GLuint textureID, GLuint depthTexture, GLuint shadowMapID) {

	// This loads our buffers.
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

	// Render to the screen
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glEnable(GL_CULL_FACE);


	glm::vec3 lightInvDir = glm::vec3(0.5f, 2, 2);

	// Compute the MVP matrix from the light's point of view
	glm::mat4 depthProjectionMatrix = glm::ortho<float>(-10, 10, -10, 10, -10, 20);
	glm::mat4 depthViewMatrix = glm::lookAt(lightInvDir, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	// or, for spot light :
	//glm::vec3 lightPos(5, 20, 20);
	//glm::mat4 depthProjectionMatrix = glm::perspective<float>(45.0f, 1.0f, 2.0f, 50.0f);
	//glm::mat4 depthViewMatrix = glm::lookAt(lightPos, lightPos-lightInvDir, glm::vec3(0,1,0));

	glm::mat4 depthModelMatrix = glm::mat4(1.0);
	glm::mat4 depthMVP = depthProjectionMatrix * depthViewMatrix * depthModelMatrix;

	// Send our transformation to the currently bound shader, 
	// in the "MVP" uniform
	glUniformMatrix4fv(depthMatrixID, 1, GL_FALSE, &depthMVP[0][0]);

	// Use our shader
	glUseProgram(programID);

	glm::vec3 position = model->getPosition();

	// Compute the MVP matrix from keyboard and mouse input
	glm::mat4 ProjectionMatrix = getProjectionMatrix();
	glm::mat4 ViewMatrix = getViewMatrix();
	glm::mat4 ModelMatrix = glm::translate(glm::mat4(1.0), position);
	glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
	glm::mat4 depthBiasMVP = biasMatrix * depthMVP;

	// Send our transformation to the currently bound shader 
	// in the "MVP" uniform
	glUniformMatrix4fv(matrixID, 1, GL_FALSE, &MVP[0][0]);
	glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
	glUniformMatrix4fv(viewMatrixID, 1, GL_FALSE, &ViewMatrix[0][0]);
	glUniformMatrix4fv(depthBiasID, 1, GL_FALSE, &depthBiasMVP[0][0]);

	glUniform3f(lightInvDirID, lightInvDir.x, lightInvDir.y, lightInvDir.z);

	// Bind our texture in Texture Unit 0
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, *model->getTexture());
	// Set our "myTextureSampler" sampler to user Texture Unit 0
	glUniform1i(textureID, 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, depthTexture);
	glUniform1i(shadowMapID, 1);

	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glVertexAttribPointer(
		0,                  // attribute
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);

	// 2nd attribute buffer : UVs
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glVertexAttribPointer(
		1,                                // attribute
		2,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
	);

	// 3rd attribute buffer : normals
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glVertexAttribPointer(
		2,                                // attribute
		3,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
	);

	// Index buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);

	// Draw the triangles !
	glDrawElements(
		GL_TRIANGLES,      // mode
		model->getIndexSize(),    // count
		GL_UNSIGNED_SHORT, // type
		(void*)0           // element array buffer offset
	);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &uvbuffer);
	glDeleteBuffers(1, &normalbuffer);
	glDeleteBuffers(1, &elementbuffer);
}