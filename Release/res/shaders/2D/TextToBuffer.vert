/*
 * Name: 2D Text Rendering
 * Author: Ekkon Games - Robert Ciborowski
 * Date: 15/03/2017
 * Description: Project Aela's Text Renderer, which renders text.
*/

#version 430 core

// This is the input data.
layout(location = 0) in vec3 vertexPosition;
uniform vec2 positionOfTextureOnScreen;
uniform vec2 textureDimensions;
uniform vec2 windowDimensions;
uniform vec4 textColour;

// This is the output data.
out vec4 textColourForFragment;
out vec2 UV;

// This is main(), which applies scaling and passes colour to the fragment shader.
void main(){
	gl_Position = vec4(vertexPosition.x, 0 - vertexPosition.y, vertexPosition.z, 1);
	UV = (vec2(vertexPosition.x - positionOfTextureOnScreen.x, vertexPosition.y - positionOfTextureOnScreen.y) / textureDimensions * windowDimensions + (windowDimensions / textureDimensions)) / 2;
	textColourForFragment = textColour;
}

