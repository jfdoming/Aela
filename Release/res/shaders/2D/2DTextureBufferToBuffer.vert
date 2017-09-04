/*
 * Name: 2D Buffer to Buffer
 * Author: Ekkon Games - Robert Ciborowski
 * Date: 15/03/2017
 * Description: Project Aela's 2D Buffer to Buffer Renderer, which is made to
 *              specifically render a framebuffer's texture to another buffer.
*/

#version 330 core

// This is the input data.
layout(location = 0) in vec3 vertexPosition;
uniform vec2 positionOfTextureOnScreen;
uniform vec2 windowDimensions;

// This is the output data.
out vec2 UV;

float convertFromPositionToUV(float value) {
	return (value + 1) / 2;
}

// This is main(), which applies scaling to the texture.
void main(){
		gl_Position = vec4(vertexPosition.x, vertexPosition.y, vertexPosition.z, 1);
		UV = vec2(convertFromPositionToUV(vertexPosition.x) - positionOfTextureOnScreen.x, convertFromPositionToUV(vertexPosition.y) - positionOfTextureOnScreen.y);
}

