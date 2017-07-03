/*
 * Name: Billboard Shader
 * Author: Ekkon Games - Robert Ciborowski
 * Date: November 2016
 * Description: Project Aela's billboard fragment shader.
*/

#version 330 core

// This is the output data.
layout(location = 0) out vec4 colour;

in vec2 UV;

// Values that stay constant for the whole mesh.
uniform sampler2D textureSampler;

void main(){
	colour = texture(textureSampler, UV);
}