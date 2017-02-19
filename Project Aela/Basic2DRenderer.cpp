#include "Basic2DRenderer.h"

void Basic2DRenderer::setup() {
	programID = loadShaders("res/shaders/quad.vertexshader", "res/shaders/quad.fragmentshader");
	textureID = glGetUniformLocation(programID, "quadTexture");
	positionID = glGetUniformLocation(programID, "positionOfTextureOnScreen");
	std::cout << positionID << "is the ID\n";
}

void Basic2DRenderer::renderTexture(GLuint texture, Rect<int> * windowDimensions, Rect<int> * output) {
	glDisable(GL_CULL_FACE);
	// The fullscreen quad's FBO
	GLuint quad_VertexArrayID;
	glGenVertexArrays(1, &quad_VertexArrayID);
	glBindVertexArray(quad_VertexArrayID);
	// int texX = output->getX(), texY = output->getY(), texWidth = output->getWidth(), texHeight = output->getHeight();

	int windowWidth = windowDimensions->getWidth(), windowHeight = windowDimensions->getHeight();
	float topLeftX = (float) (-1 + ((float) output->getX() / (windowWidth / 2))),
		topLeftY = (float) (-1 + ((float) output->getY() / (windowHeight / 2))),
		bottomRightX = (-1 + ((float) (output->getX() + output->getWidth()) / (windowWidth / 2))),
		bottomRightY = (float) (-1 + ((float) (output->getY() + output->getHeight()) / (windowWidth / 2)));

	/*static const GLfloat g_quad_vertex_buffer_data[] = {
		topLeftX, topLeftY, 0.0f,
		bottomRightX, topLeftY, 0.0f,
		topLeftX,  bottomRightY, 0.0f,
		topLeftX,  bottomRightY, 0.0f,
		bottomRightX, topLeftY, 0.0f,
		bottomRightX,  bottomRightY, 0.0f,
	};*/

	static const GLfloat g_quad_vertex_buffer_data[] = {
		topLeftX,  bottomRightY, 0.0f,
		topLeftX, topLeftY, 0.0f,
		bottomRightX, topLeftY, 0.0f,
		topLeftX,  bottomRightY, 0.0f,
		bottomRightX, topLeftY, 0.0f,
		bottomRightX,  bottomRightY, 0.0f
	};

	GLuint quad_vertexbuffer;
	glGenBuffers(1, &quad_vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, quad_vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_quad_vertex_buffer_data), g_quad_vertex_buffer_data, GL_STATIC_DRAW);

	// Render to the screen
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	// Render on the whole framebuffer, complete from the lower left corner to the upper right
	glViewport(0, 0, windowWidth, windowHeight);

	// Use our shader
	glUseProgram(programID);

	// Bind our texture in Texture Unit 0
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	// Set our "renderedTexture" sampler to user Texture Unit 0
	glUniform1i(textureID, 0);

	GLfloat topLeft[] = {
		(float) output->getX() / output->getWidth(),
		(float) output->getY() / output->getHeight()
	};
	glUniform2fv(positionID, 1, topLeft);

	std::cout << positionID << " is the other ID\n";

	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, quad_vertexbuffer);
	glVertexAttribPointer(
		0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*) 0            // array buffer offset
	);

	// Draw the triangles !
	glDrawArrays(GL_TRIANGLES, 0, 6); // 2*3 indices starting at 0 -> 2 triangles

	glDisableVertexAttribArray(0);
}