/*
* Name: Project Aela's 2D Renderer
* Author: Ekkon Games
* Date: February 2017
* Description: A class used by Aela's Renderer to render textures as 2D objects.
*/

#include "Basic2DRenderer.h"

void Basic2DRenderer::setup() {
	programID = loadShaders("res/shaders/quad.vertexshader", "res/shaders/quad.fragmentshader");
	textureID = glGetUniformLocation(programID, "quadTexture");
	positionID = glGetUniformLocation(programID, "positionOfTextureOnScreen");
	textureDimensionsID = glGetUniformLocation(programID, "textureDimensions");
	windowDimensionsID = glGetUniformLocation(programID, "windowDimensions");
	boundingBoxID = glGetUniformLocation(programID, "boundingBoxDimensions");
	glGenVertexArrays(1, &quad_VertexArrayID);
	glGenBuffers(1, &quad_vertexbuffer);
}

void Basic2DRenderer::renderTexture(Texture * texture, Rect<int> * windowDimensions, Rect<int> * output) {
	// Due to the way that OpenGL seems to flip the texture, the shader needs to flip it back
	// and show the texture's opposite side. Or the renderer could just send normal data.
	// GL_CULL_FACE needs to be disabled if one uses the first option.
	glDisable(GL_CULL_FACE);
	glBindVertexArray(quad_VertexArrayID);

	int windowWidth = windowDimensions->getWidth(), windowHeight = windowDimensions->getHeight();
	float topLeftX = (float) (-1 + ((float) output->getX() / (windowWidth / 2))),
		topLeftY = (float) (-1 + ((float) output->getY() / (windowHeight / 2))),
		bottomRightX = (-1 + ((float) (output->getX() + output->getWidth()) / (windowWidth / 2))),
		bottomRightY = (float) (-1 + ((float) (output->getY() + output->getHeight()) / (windowHeight / 2)));

	static const GLfloat g_quad_vertex_buffer_data[] = {
		topLeftX,  bottomRightY, 0.0f,
		topLeftX, topLeftY, 0.0f,
		bottomRightX, topLeftY, 0.0f,
		topLeftX,  bottomRightY, 0.0f,
		bottomRightX, topLeftY, 0.0f,
		bottomRightX,  bottomRightY, 0.0f
	};

	int textureWidth = texture->getDimensions().getWidth(), textureHeight = texture->getDimensions().getHeight();

	glBindBuffer(GL_ARRAY_BUFFER, quad_vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_quad_vertex_buffer_data), g_quad_vertex_buffer_data, GL_STATIC_DRAW);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, windowWidth, windowHeight);
	glUseProgram(programID);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture->getTexture());
	glUniform1i(textureID, 0);

	GLfloat topLeftArray[] = {
		(float) 1 + topLeftX,
		(float) 1 + topLeftY
	};
	glUniform2fv(positionID, 1, topLeftArray);

	GLfloat widthAndHeightArray[] = {
		(float) output->getWidth(),
		(float) output->getHeight()
	};
	glUniform2fv(boundingBoxID, 1, widthAndHeightArray);

	GLfloat textureDimensionsArray[2] = {
		(float) textureWidth, (float) textureHeight
	};
	glUniform2fv(textureDimensionsID, 1, textureDimensionsArray);

	GLfloat windowDimensionsArray[2] = {
		(float) windowWidth, (float) windowHeight
	};
	glUniform2fv(windowDimensionsID, 1, windowDimensionsArray);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, quad_vertexbuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisableVertexAttribArray(0);
}