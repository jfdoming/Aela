/*
 * Name: Depth Shader
 * Author: Ekkon Games - Robert Ciborowski
 * Date: October 2016
 * Description: Project Aela's depth vertex shader.
*/

#version 330 core

// This is the input data.
layout(location = 0) in vec3 vertexPosition;

// This is the only uniform. The depth vertex shader does not need that much information.
uniform mat4 modelMatrix;

void main(){
	gl_Position = modelMatrix * vec4(vertexPosition, 1);
}

