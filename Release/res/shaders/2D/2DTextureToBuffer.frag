/*
 * Name: 2D Texture Rendering
 * Author: Ekkon Games - Robert Ciborowski
 * Date: 15/03/2017
 * Description: Project Aela's Texture Renderer, which renders a 2D texture to a buffer.
*/

#version 410

// This is the output data.
layout(location = 0) out vec4 colour;

// This is the texture to be used.
uniform sampler2D quadTexture;

// This is the input data.
in vec2 UV;

// This is main(), which simply gets colour.
void main(){
	colour = texture(quadTexture, UV);
}