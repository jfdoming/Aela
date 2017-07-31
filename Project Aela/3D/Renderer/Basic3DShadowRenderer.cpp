/*
* Name: Project Aela's Shadow Renderer
* Author: Robert Ciborowski
* Date: October 2016
* Description: A class used by Project Aela's Basic 3D Renderer to render shadows of 3D models.
*/

#include "Basic3DShadowRenderer.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <iostream>

using namespace Aela;

// This clears all shadow maps that would be rendered.
void Basic3DShadowRenderer::clearShadowMaps(std::vector<Light3D>* lights) {
	unsigned int numberOfLights = lights->size();
	if (numberOfLights > MAX_LIGHT_AMOUNT) {
		numberOfLights = numberOfLights;
	}
	for (unsigned int i = 0; i < numberOfLights; i++) {
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glBindFramebuffer(GL_FRAMEBUFFER, *lights->at(i).getShadowMapBuffer());
		glClear(GL_DEPTH_BUFFER_BIT);
	}
}

// This renders a shadow of a entity to each light's depth buffer.
void Basic3DShadowRenderer::renderShadow(Entity3D* entity, GLuint depthProgramID, GLuint shadowModelMatrixID, GLuint shadowMatrixID, std::vector<Light3D>* lights, GLuint lightPositionsID) {
	unsigned int numberOfLights = lights->size();
	if (numberOfLights > MAX_LIGHT_AMOUNT) {
		numberOfLights = MAX_LIGHT_AMOUNT;
	}

	// This enables face culling.
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);

	glUseProgram(depthProgramID);

	// This sets up necessary buffers.
	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);

	GLuint elementbuffer;
	glGenBuffers(1, &elementbuffer);

	for (SubModel subModel : *entity->getModel()->getSubModels()) {
		for (unsigned int whichLight = 0; whichLight < numberOfLights; whichLight++) {
			Light3D* light = &lights->at(whichLight);
			
			// This sends data to the buffers.
			glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
			glBufferData(GL_ARRAY_BUFFER, subModel.getVertexSize() * sizeof(glm::vec3), &subModel.getVertices()->at(0), GL_STATIC_DRAW);
			
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, subModel.getIndexSize() * sizeof(unsigned short), &subModel.getIndices()->at(0), GL_STATIC_DRAW);

			// These are positions and rotations for light and the subModel.
			glm::vec3* lightPosition = light->getPosition();
			glm::vec3* rotation = entity->getRotation();
			glm::vec3* position = entity->getPosition();
			glm::vec3* scaling = entity->getScaling();

			// This calculates more matrices.
			float near = 1, far = 100;
			glm::mat4 depthProjectionMatrix = glm::perspective(glm::radians(90.0f), (float) DEPTH_TEXTURE_WIDTH / (float) DEPTH_TEXTURE_HEIGHT, near, far);
			glm::mat4 shadowTransformations[6];
			shadowTransformations[0] = depthProjectionMatrix * glm::lookAt(*lightPosition, *lightPosition + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
			shadowTransformations[1] = depthProjectionMatrix * glm::lookAt(*lightPosition, *lightPosition + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
			shadowTransformations[2] = depthProjectionMatrix * glm::lookAt(*lightPosition, *lightPosition + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
			shadowTransformations[3] = depthProjectionMatrix * glm::lookAt(*lightPosition, *lightPosition + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f));
			shadowTransformations[4] = depthProjectionMatrix * glm::lookAt(*lightPosition, *lightPosition + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f));
			shadowTransformations[5] = depthProjectionMatrix * glm::lookAt(*lightPosition, *lightPosition + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f));

			glBindFramebuffer(GL_FRAMEBUFFER, *light->getShadowMapBuffer());

			for (unsigned int i = 0; i < 6; i++) {
				glUniformMatrix4fv(shadowMatrixID + i, 1, GL_FALSE, &shadowTransformations[i][0][0]);
			}
			glUniform3fv(lightPositionsID, 1, &(light->getPosition()->x));

			// This computes more matrices.
			glm::mat4 modelMatrix = glm::translate(glm::scale(glm::eulerAngleYXZ(rotation->y, rotation->x, rotation->z), *scaling), *position);

			// This sends all transformations to the shader.
			glUniformMatrix4fv(shadowModelMatrixID, 1, GL_FALSE, &modelMatrix[0][0]);

			// These are the vertex attributes.
			glEnableVertexAttribArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);

			// This is the index buffer.
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);

			// This draws the elements.
			glDrawElements(GL_TRIANGLES, subModel.getIndexSize(), GL_UNSIGNED_SHORT, (void*) 0);

			// This frees the buffers from the memory. Delete this if you want to screw over your users.
			glDisableVertexAttribArray(0);

		}
	}

	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &elementbuffer);
}

unsigned int Basic3DShadowRenderer::getDepthTextureWidth() {
	return DEPTH_TEXTURE_WIDTH;
}

unsigned int Basic3DShadowRenderer::getDepthTextureHeight() {
	return DEPTH_TEXTURE_HEIGHT;
}

// This is for debugging.
void Basic3DShadowRenderer::renderTestCube() {
	unsigned int cubeVAO = 0;
	unsigned int cubeVBO = 0;
	if (cubeVAO == 0) {
		// Order: back face, front face, left face, right face, bottom face, top face
		float vertices[] = {
			-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,
			1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
			1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f,
			1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
			-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,
			-1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f,

			-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f,
			1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f,
			1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f,
			1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f,
			-1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f,
			-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f,

			-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
			-1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
			-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
			-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,

			1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
			1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
			1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
			1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
			1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
			1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f,

			-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f,
			1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f,
			1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
			1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
			-1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f,
			-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f,

			1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
			1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f,
			1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
			-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f,
			-1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f
		};
		glGenVertexArrays(1, &cubeVAO);
		glGenBuffers(1, &cubeVBO);
		// fill buffer
		glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		// link vertex attributes
		glBindVertexArray(cubeVAO);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) 0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (3 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (6 * sizeof(float)));
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
	glBindVertexArray(cubeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}