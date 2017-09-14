/*
 * Name: 2D Texture Rendering
 * Author: Ekkon Games - Robert Ciborowski
 * Date: 15/03/2017
 * Description: Project Aela's Texture Renderer, which renders a 2D texture to a buffer.
*/

#version 430 core

// These is the input data.
layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec2 UVPosition;
layout(location = 0) uniform vec2 positionOfTextureOnScreen;

// This is the output data.
out vec2 UV;

float convertFromPositionToUV(float value) {
	return (value + 1) / 2;
}

// This is main(), which applies scaling to the texture. It also flips it since it would
// otherwise be rendered upside-down.
void main(){
	gl_Position = vec4(vertexPosition.x, 0 - vertexPosition.y, vertexPosition.z, 1);
	UV = UVPosition;
	// UV = vec2(convertFromPositionToUV(vertexPosition.x), convertFromPositionToUV(vertexPosition.y));
	
	// gl_Position = vec4(vertexPosition.x, 0 - vertexPosition.y, vertexPosition.z, 1);
	// vec2 UVScaling = (textureDimensions * 2 / windowDimensions) * (boundingBoxDimensions / textureDimensions);
	// UV = vec2(vertexPosition.x - positionOfTextureOnScreen.x, vertexPosition.y - positionOfTextureOnScreen.y) / UVScaling + (windowDimensions / 2 / boundingBoxDimensions);
}

