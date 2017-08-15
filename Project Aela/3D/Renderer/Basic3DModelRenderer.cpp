/*
* Class: Basic 3D Model Renderer
* Author: Robert Ciborowski
* Date: October 2016
* Description: A class used by Project Aela's Basic 3D Renderer to specifically render 3D models.
*/

#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Basic3DModelRenderer.h"
#include "../../Control Manager/ControlManager.h"

using namespace Aela;

// This function is called in order to update camera-related matrices.
void Basic3DModelRenderer::setMatrices(glm::mat4 setViewMatrix, glm::mat4 setProjectionMatrix) {
	viewMatrix = setViewMatrix;
	projectionMatrix = setProjectionMatrix;
}

// This function sends light data to the shader.
void Basic3DModelRenderer::sendLightDataToShader(std::unordered_map<int, LightEntity>* lights, GLuint modelProgramID, GLuint numberOfLightsID,
	GLuint lightPositionsID, GLuint lightDirectionsID, GLuint lightColoursID, GLuint lightPowersID, GLuint shadowMapID) {
	glUseProgram(modelProgramID);

	GLuint numberOfLights = lights->size();
	if (numberOfLights > MAX_LIGHT_AMOUNT) {
		numberOfLights = MAX_LIGHT_AMOUNT;
	}

	glUniform1i(numberOfLightsID, numberOfLights);

	if (numberOfLights > 0) {
		unsigned int i = 0;
		for (auto light : *lights) {
			if (i + 1 > MAX_LIGHT_AMOUNT) {
				break;
			}
			glUniform3fv(lightPositionsID + i, 1, &light.second.getPosition()->x);
			glUniform3fv(lightDirectionsID + i, 1, &light.second.getRotation()->x);

			glm::vec3 value = light.second.getColour()->getVec3();
			glUniform3fv(lightColoursID + i, 1, &value.x);

			float power = light.second.getPower();
			glUniform1fv(lightPowersID + i, numberOfLights, &power);

			glActiveTexture(GL_TEXTURE1 + i);
			glBindTexture(GL_TEXTURE_CUBE_MAP, *lights->at(i).getShadowMapTexture());
			glUniform1i(shadowMapID + i, 1 + i);
			i++;
		}

		// If the number of lights is less than the max light amount, the first light's shadow map is sent to fill in the remaining spots in the
		// shader's shadow map array. Not doing so causes the shader to crash.
		for (unsigned int i = lights->size(); i < MAX_LIGHT_AMOUNT; i++) {
			glActiveTexture(GL_TEXTURE1 + i);
			glBindTexture(GL_TEXTURE_CUBE_MAP, *lights->begin()->second.getShadowMapTexture());
			glUniform1i(shadowMapID + i, 1 + i);
		}
	}
}

// This function renders a subModel.
void Basic3DModelRenderer::renderModelEntity(ModelEntity* entity, GLuint frameBuffer, GLuint modelProgramID, GLuint modelMVPMatrixID,
	GLuint modelMatrixID, GLuint modelViewMatrixID, GLuint modelTextureID, GLuint cameraPositionID, glm::vec3* cameraPosition) {

	// This sets up buffers.
	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);

	GLuint uvbuffer;
	glGenBuffers(1, &uvbuffer);

	GLuint normalbuffer;
	glGenBuffers(1, &normalbuffer);

	GLuint elementbuffer;
	glGenBuffers(1, &elementbuffer);

	glUseProgram(modelProgramID);

	// This binds the framebuffer.
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	// Ranged for loops break here for some reason and I don't have the time to see why, so here's a regular for loop instead.
	for (unsigned int whichSubModel = 0; whichSubModel < entity->getModel()->getSubModels()->size(); whichSubModel++) {
		SubModel& subModel = entity->getModel()->getSubModels()->at(whichSubModel);
		// This loads buffers.
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glBufferData(GL_ARRAY_BUFFER, subModel.getVertexSize() * sizeof(glm::vec3), &subModel.getVertices()->at(0), GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
		glBufferData(GL_ARRAY_BUFFER, subModel.getUVSize() * sizeof(glm::vec2), &subModel.getUVs()->at(0), GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
		glBufferData(GL_ARRAY_BUFFER, subModel.getNormalSize() * sizeof(glm::vec3), &subModel.getNormals()->at(0), GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, subModel.getIndexSize() * sizeof(unsigned short), &subModel.getIndices()->at(0), GL_STATIC_DRAW);

		// This is positioning/rotation of the subModel.
		glm::vec3* position = entity->getPosition();
		glm::vec3* rotation = entity->getRotation();
		glm::vec3* scaling = entity->getScaling();

		// This computes more matrices.
		glm::mat4 modelMatrix = glm::scale(glm::translate(glm::mat4(1.0), *position), *scaling) * glm::eulerAngleYXZ(rotation->y, rotation->x, rotation->z);
		glm::mat4 MVP = projectionMatrix * viewMatrix * modelMatrix;

		// This sends more uniforms to the shader.
		glUniformMatrix4fv(modelMVPMatrixID, 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, &modelMatrix[0][0]);
		glUniformMatrix4fv(modelViewMatrixID, 1, GL_FALSE, &viewMatrix[0][0]);
		glUniform3fv(cameraPositionID, 1, &(cameraPosition->x));

		// This binds the texture to "slot" zero.
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, *(subModel.getMaterial()->getTexture()->getTexture()));
		glUniform1i(modelTextureID, 0);

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
			(void*) 0
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
		glDrawElements(GL_TRIANGLES, subModel.getIndexSize(), GL_UNSIGNED_SHORT, (void*) 0);
	}

	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &uvbuffer);
	glDeleteBuffers(1, &normalbuffer);
	glDeleteBuffers(1, &elementbuffer);

	// This deletes buffers.
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}

// This function renders a 2D texture in 3D space. It could be used for billboards.
// For billboards, use getPositionOfCamera() to make the texture look at the camera.
// To specify a rotation for the camera as a vec3, use the texture's position and add the direction (position + direction) for the lookAt parameter.
// Note: for the lookAt parameter, position + glm::vec3(0.0, 0.0, 1.0) will not rotate the texture. Use this for no rotation.
void Basic3DModelRenderer::renderTextureIn3DSpace(bool cullFaces, GLuint texture, GLuint modelTextureID,
	GLuint programID, GLuint frameBuffer, GLuint billboardMVPMatrixID, glm::vec3* position, glm::vec3* scale, glm::vec3* lookAt, bool inverseRotation) {
	glUseProgram(programID);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

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
		glUniform1i(modelTextureID, 0);

		// Buffer generation.
		GLuint vertexbuffer;
		glGenBuffers(1, &vertexbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		GLuint uvbuffer;
		glGenBuffers(1, &uvbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(uvs), uvs, GL_STATIC_DRAW);

		// This computes matrices based on control input.
		glm::mat4 modelMatrix = glm::lookAt(*position, *lookAt, glm::vec3(0, 1, 0));
		if (inverseRotation) {
			modelMatrix = glm::inverse(modelMatrix);
		}
		modelMatrix = glm::translate(glm::scale(modelMatrix, *scale), *position);
		glm::mat4 MVP = projectionMatrix * viewMatrix * modelMatrix;

		glUniformMatrix4fv(billboardMVPMatrixID, 1, GL_FALSE, &MVP[0][0]);

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
			(void*) 0
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

// This function renders a texture in the 3D space.
void Basic3DModelRenderer::renderTextureIn3DSpace(bool cullFaces, GLuint texture, GLuint billboardTextureID, GLuint programID, GLuint frameBuffer,
	GLuint billboardMVPMatrixID, glm::vec3* position, glm::vec3* rotation, glm::vec3* scale) {
	glUseProgram(programID);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

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
		glUniform1i(billboardTextureID, 0);

		// Buffer generation.
		GLuint vertexbuffer;
		glGenBuffers(1, &vertexbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		GLuint uvbuffer;
		glGenBuffers(1, &uvbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(uvs), uvs, GL_STATIC_DRAW);

		// This computes matrices based on control input.
		glm::mat4 modelMatrix = glm::translate(glm::scale(glm::eulerAngleYXZ(rotation->y, rotation->x, rotation->z), *scale), *position);
		glm::mat4 MVP = projectionMatrix * viewMatrix * modelMatrix;

		glUniformMatrix4fv(billboardMVPMatrixID, 1, GL_FALSE, &MVP[0][0]);

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
			(void*) 0
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

// This function is for debugging purposes.
void Basic3DModelRenderer::drawTestQuad() {
	glColor3f(1.0, 1.0, 0.5);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);
	glVertex2f(0, 0);
	glTexCoord2f(1, 0);
	glVertex2f(1, 0);
	glTexCoord2f(1, 1);
	glVertex2f(1, 1);
	glTexCoord2f(0, 1);
	glVertex2f(0, 1);
	glEnd();
}

// This function is for debugging purposes.
void Basic3DModelRenderer::renderTestCube() {
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