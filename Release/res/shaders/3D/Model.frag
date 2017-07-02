/*
 * Name: Model Shader
 * Author: Ekkon Games - Robert Ciborowski
 * Date: October 2016
 * Description: Project Aela's Depth Texture Renderer, which is used for shadows.
*/

#version 330 core

// This is the input data.
in vec2 UV;
in vec3 worldSpacePosition;
in vec3 modelViewProjectionPosition;
in vec3 cameraSpaceNormal;
in vec3 cameraSpaceEyeDirection;
in vec4 shadowCoordinate;
in vec3 normal;

// This is the output data.
vec3 colourAsVec3;
layout(location = 0) out vec4 colour;

// These are values that are hard-coded into the shader.
const int MAX_LIGHT_AMOUNT = 2;
float distanceToLightModifier = 0.1;
bool PCF = true;
float PI  = 3.14159265358979323846;
float far = 100.0;
float shadowScalingFactor = 1;
vec3 PCFDirections[20] = vec3[](
   vec3( 1,  1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1,  1,  1), 
   vec3( 1,  1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1,  1, -1),
   vec3( 1,  1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1,  1,  0),
   vec3( 1,  0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1,  0, -1),
   vec3( 0,  1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0,  1, -1)
);

// These are the uniforms used by the shader.
uniform sampler2D textureSampler;
uniform samplerCube shadowMaps[MAX_LIGHT_AMOUNT];
uniform int numberOfLights;
uniform vec3 lightPositions[2];
uniform vec3 lightDirections[2];
uniform vec3 lightColours[2];
uniform float lightPowers[2];
uniform vec3 cameraPosition;

// This calculates the shadow. It contains a few methods of making shadows look nicer,
// including the fixing of shadow acne and peter panning. It also includes percentage-closer
// filtering (PCF).
float shadowCalculation(vec3 positionInLightSpace, int whichLight, float bias) {
    vec3 fragToLight = positionInLightSpace - lightPositions[whichLight];
    float currentDepth = length(fragToLight) * shadowScalingFactor;
	if (currentDepth > far) {
		return 0;
	}
    // float shadow = currentDepth -  bias > closestDepth ? 1.0 : 0.0;
	float shadow = 0;
	
	// This undergoes the PCF process, if enabled.
	if (PCF) {
		// float PCFRadius = (1.0 + length(cameraPosition - worldSpacePosition)) / 500.0;
		float PCFRadius = 0.05;
		for (int i = 0; i < PCFDirections.length(); i++) {
			float closestDepth = texture(shadowMaps[whichLight], fragToLight + PCFDirections[i] * PCFRadius).r;
			closestDepth *= far * shadowScalingFactor;
			if (currentDepth - bias > closestDepth) {
				shadow += 1.0;
			}
		}
		shadow /= float(PCFDirections.length());
	} else {
		float closestDepth = texture(shadowMaps[whichLight], fragToLight).z;
		closestDepth *= far * shadowScalingFactor;
		shadow = currentDepth -  bias > closestDepth ? 1.0 : 0.0;
	}
	
	// return closestDepth / far;
    return shadow;
}  

void main(){
	// This calculates several colours.
	vec3 MaterialDiffuseColor = texture(textureSampler, UV).rgb;
	vec3 MaterialAmbientColor = vec3(0.15, 0.15, 0.15) * MaterialDiffuseColor;
	vec3 MaterialSpecularColor = vec3(0.3, 0.3, 0.3);
	vec3 diffuseColours[lightPositions.length()];
	vec3 finalDiffuseColour;
	
	float visibility = 1.0;
	
	for (int i = 0; i < numberOfLights; i++) {
		vec3 n = normal;
		vec3 l = normalize(lightPositions[i] - worldSpacePosition);
		
		// This calculates the cosine between the normal and the light direction. It is clamped.
		// If the light is at the vertical of the triangle, cosTheta is 1.
		// If the light perpendicular to the triangle, cosTheta is 0.
		// If the light behind the triangle, cosTheta is 0.
		float cosTheta = clamp(dot(n, l), 0, 1);
		
		float bias = 0.005 * tan(acos(cosTheta));
		bias = clamp(bias, 0,0.01);
		
		float shadow = shadowCalculation(worldSpacePosition, i, bias);
		visibility -= shadow;
		
		float distanceBetweenLightAndFragment = distance(worldSpacePosition, lightPositions[i]);
		
		diffuseColours[i] = MaterialDiffuseColor * lightColours[i] * lightPowers[i] * cosTheta * ((1 / distanceBetweenLightAndFragment) / distanceToLightModifier);
		visibility += cosTheta * lightPowers[i];
	}
	
	for (int i = 0; i < diffuseColours.length(); i++) {
		finalDiffuseColour += diffuseColours[i];
	}
	
	clamp(finalDiffuseColour, 0.0, 1.0);
	clamp(visibility, 0, 1);
	
	colourAsVec3 = 
		MaterialAmbientColor + visibility * finalDiffuseColour;
	colour = vec4(colourAsVec3, 1);
}