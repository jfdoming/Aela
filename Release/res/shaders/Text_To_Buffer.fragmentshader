/*
 * Name: 2D Text Rendering
 * Author: Ekkon Games - Robert Ciborowski
 * Date: 15/03/2017
 * Description: Project Aela's Text Renderer, which renders text.
*/

#version 330 core

// This is the output data.
layout(location = 0) out vec4 colour;

// This is the input data.
in vec2 UV;
in vec4 textColourForFragment;

// This is the texture to be used.
uniform sampler2D quadTexture;

// This is main, which applies text colour based on a text's alpha value.
void main(){
	if (texture(quadTexture, UV).a == 1) {
		colour = textColourForFragment;
	} else {
		colour = vec4(0, 0, 0, 0);
	}
}