/*
 * Name: Model Shader
 * Author: Ekkon Games - Robert Ciborowski
 * Date: October 2016
 * Description: Project Aela's modelMatrix vertex shader.
*/

#version 330 core

// This is the input data.
layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec2 vertexUV;
layout(location = 2) in vec3 vertexNormal;

// This is the output data.
out vec2 UV;
out vec3 worldSpacePosition;
out vec3 modelViewProjectionPosition;
out vec3 cameraSpaceNormal;
out vec3 cameraSpaceEyeDirection;
out vec3 normal;

// These are values that stay constant for the whole mesh.
uniform mat4 modelViewProjectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

void main(){

	// This is the output position transformed by the model, view and projection matrices.
	gl_Position = modelViewProjectionMatrix * vec4(vertexPosition, 1);
	
	modelViewProjectionPosition = vertexPosition;
	
	normal = vertexNormal;
	
	// This is the position of the vertex in the world space.
	worldSpacePosition = vec3(modelMatrix * vec4(vertexPosition, 1.0));
	
	// This is a vector that goes from the vertex to the camera.
	cameraSpaceEyeDirection = vec3(0,0,0) - (viewMatrix * modelMatrix * vec4(vertexPosition,1)).xyz;
	
	// This calculates the normal of the vertex.
	cameraSpaceNormal = (viewMatrix * modelMatrix * vec4(vertexNormal,0)).xyz;
	
	UV = vertexUV;
}

