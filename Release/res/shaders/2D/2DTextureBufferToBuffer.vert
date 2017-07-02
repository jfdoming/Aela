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
layout(location = 1) in vec2 positionOfTextureOnScreen;
layout(location = 2) in vec2 boundingBoxDimensions;
layout(location = 3) in vec2 textureDimensions;
layout(location = 4) in vec2 windowDimensions;

// This is the output data.
out vec2 UV;

// This is main(), which applies scaling to the texture.
void main(){
		gl_Position = vec4(vertexPosition.x, vertexPosition.y, vertexPosition.z, 1);
		vec2 UVScaling = (textureDimensions * 2 / windowDimensions) * (boundingBoxDimensions / textureDimensions);
		UV = vec2(vertexPosition.x - positionOfTextureOnScreen.x, vertexPosition.y - positionOfTextureOnScreen.y) / UVScaling + (windowDimensions / 2 / boundingBoxDimensions);
}

