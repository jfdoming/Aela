#include "Renderer.h"
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>

void Basic3DModelShader::shade(Model3D * model, GLuint depthProgramID, GLuint depthMatrixID) {
	// This loads the shadow renderer's buffers.
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

	// This culls triangles.
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	// This says "use this shader".
	glUseProgram(depthProgramID);

	// These are positions and rotations for light and the model.
	glm::vec3 lightInvDir = glm::vec3(0.5f, 2, 2);
	glm::vec3 position = model->getPosition();
	glm::vec3 rotation = model->getRotation();

	// This calculates the MVP (model view projection) matrix from the view of the light. Note: glm::ortho creates a mtrix.
	glm::mat4 depthProjectionMatrix = glm::ortho<float>(-10, 10, -10, 10, -10, 20);
	glm::mat4 depthViewMatrix = glm::lookAt(lightInvDir, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	
	// This is for a spotlight, use either this or the code above.
	// glm::vec3 lightPos(5, 20, 20);
	// glm::mat4 depthProjectionMatrix = glm::perspective<float>(45.0f, 1.0f, 2.0f, 50.0f);
	// glm::mat4 depthViewMatrix = glm::lookAt(lightPos, lightPos-lightInvDir, glm::vec3(0,1,0));


	// gOrientation1.y += (3.14159f / 2.0f * getTimeInterval()) / 1000.0f;
	// This computes more matrices.
	glm::mat4 rotationMatrix = glm::eulerAngleYXZ(rotation.y, rotation.x, rotation.z);
	glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0), position) * rotationMatrix;
	glm::mat4 depthMVP = depthProjectionMatrix * depthViewMatrix * modelMatrix;

	// This sends all transformations to the shader.
	glUniformMatrix4fv(depthMatrixID, 1, GL_FALSE, &depthMVP[0][0]);

	// These are the vertex attributes.
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);

	// This is the index buffer.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);

	// This draws the elements.
	glDrawElements(GL_TRIANGLES, model->getIndexSize(), GL_UNSIGNED_SHORT, (void*) 0);

	// This frees the buffers from the memory. Delete this if you want to screw over your users.
	glDisableVertexAttribArray(0);
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &uvbuffer);
	glDeleteBuffers(1, &normalbuffer);
	glDeleteBuffers(1, &elementbuffer);
}