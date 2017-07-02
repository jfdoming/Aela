/*
 * Name: Skybox Shader
 * Author: Ekkon Games - Robert Ciborowski
 * Date: June 2017
 * Description: Project Aela's skybox fragment shader.
*/

#version 330 core

out vec4 colour;
in vec3 coordinates;

uniform samplerCube skyboxTexture;

void main() {    
    colour = texture(skyboxTexture, coordinates);
	colour.a = 1.0;
}