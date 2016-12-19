#include <vector>
#include <string>
#include <glm/glm.hpp>
// #include <glm/gtc/matrix_transform.hpp>

// This makes GLEW Static to avoid errors.
#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif

// This includes GLEW.
#include <GL/glew.h>

#include "AelaWindow.h"
#include "AelaError.h"

void renderTexture(AelaWindow * window, GLuint texture, GLuint programID) {
	//glm::vec3 vertexes[] = {
	//	glm::vec3(-1.0, -1.0, 0),
	//	glm::vec3(1.0, -1.0, 0),
	//	glm::vec3(-1.0, 1.0, 0),
	//	glm::vec3(1.0, 1.0, 0)
	//};

	//// This loads our buffers.
	//GLuint vertexbuffer;
	//glGenBuffers(1, &vertexbuffer);
	//glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertexes), vertexes, GL_STATIC_DRAW);

	//glm::vec2 uvs[] = {
	//	glm::vec2(-1.0, -1.0),
	//	glm::vec2(1.0, -1.0),
	//	glm::vec2(-1.0, 1.0),
	//	glm::vec2(1.0, 1.0)
	//};

	//GLuint uvbuffer;
	//glGenBuffers(1, &uvbuffer);
	//glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(uvs), uvs, GL_STATIC_DRAW);

	//// Render to the screen
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//glEnable(GL_CULL_FACE);

	//// Use our shader
	//glUseProgram(programID);


	//// Bind our texture in Texture Unit 0
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, texture);

	//// 1rst attribute buffer : vertices
	//glEnableVertexAttribArray(0);
	//glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	//glVertexAttribPointer(
	//	0,                  // attribute
	//	4,                  // size
	//	GL_FLOAT,           // type
	//	GL_FALSE,           // normalized?
	//	0,                  // stride
	//	(void*)0            // array buffer offset
	//);

	//// 2nd attribute buffer : UVs
	//glEnableVertexAttribArray(1);
	//glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	//glVertexAttribPointer(
	//	1,                                // attribute
	//	2,                                // size
	//	GL_FLOAT,                         // type
	//	GL_FALSE,                         // normalized?
	//	0,                                // stride
	//	(void*)0                          // array buffer offset
	//);

	//glDrawArrays(GL_QUADS, 0, 4); // Starting from vertex 0; 3 vertices total -> 1 triangle

	////// Draw the triangles !
	////glDrawElements(
	////	GL_TRIANGLES,      // mode
	////	model->getIndexSize(),    // count
	////	GL_UNSIGNED_SHORT, // type
	////	(void*)0           // element array buffer offset
	////);

	//glDisableVertexAttribArray(0);
	//glDisableVertexAttribArray(1);
	//glDeleteBuffers(1, &vertexbuffer);
	//glDeleteBuffers(1, &uvbuffer);


	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	glm::vec3 vertices[3];
	vertices[0] = glm::vec3(1.0, -1.0, 0.0);
	vertices[1] = glm::vec3(-1.0, 1.0, 0.0);
	vertices[2] = glm::vec3(-1.0, -1.0, 0.0);
	
	glm::vec2 uvs[] = {
		glm::vec2(-1.0, -1.0),
		glm::vec2(1.0, -1.0),
		glm::vec2(-1.0, 1.0)
	};


	//glm::vec3 normals[3] = { glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, 0.0, 1.0) };

	for (int i = 0; i < 2; i++) {
		if (i == 0) {
			vertices[2] = glm::vec3(-1.0, -1.0, 0.0);
		} else {
			vertices[2] = glm::vec3(1.0, -1.0, 0.0);
			vertices[1] = glm::vec3(-1.0, 1.0, 0.0);
			vertices[0] = glm::vec3(1.0, 1.0, 0.0);
			uvs[0] = glm::vec2(1.0, 1.0);
		}



		GLuint vertexbuffer;
		// Generate 1 buffer, put the resulting identifier in vertexbuffer
		glGenBuffers(1, &vertexbuffer);
		// The following commands will talk about our 'vertexbuffer' buffer
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		// Give our vertices to OpenGL.
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


		GLuint uvbuffer;
		glGenBuffers(1, &uvbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(uvs), uvs, GL_STATIC_DRAW);

		/*GLuint normalbuffer;
		glGenBuffers(1, &normalbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(normals), normals, GL_STATIC_DRAW);*/

		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
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

		// 2nd attribute buffer : normals
		//glEnableVertexAttribArray(1);
		//glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
		//glVertexAttribPointer(
		//	1,                                // attribute
		//	3,                                // size
		//	GL_FLOAT,                         // type
		//	GL_FALSE,                         // normalized?
		//	0,                                // stride
		//	(void*)0                          // array buffer offset
		//);

		// Draw the triangle !
		glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDeleteBuffers(1, &vertexbuffer);
		// glDeleteBuffers(1, &uvbuffer);
		//glDeleteBuffers(1, &normalbuffer);
	}
}