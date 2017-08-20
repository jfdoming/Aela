/*
* Class: 3D Camera
* Author: Robert Ciborowski
* Date: November 2016
* Description: A class used by Project Aela's Renderer to store properties of a camera.
*/

#include "Camera3D.h"

#include <glm\gtc\constants.hpp>

glm::mat4 Camera3D::getViewMatrix() {
	return viewMatrix;
}

glm::mat4 Camera3D::getProjectionMatrix() {
	return projectionMatrix;
}

void Camera3D::setFieldOfView(float setFieldOfView) {
	fieldOfView = setFieldOfView;
}

float Camera3D::getFieldOfView() {
	return fieldOfView;
}

void Camera3D::setInUse(bool inUse) {
	this->inUse = inUse;
}

bool Camera3D::isInUse() {
	return inUse;
}

void Camera3D::setUseControls(bool useControls) {
	this->useControls = useControls;
}

bool Camera3D::isUsingControls() {
	return useControls;
}

void Camera3D::setViewMatrix(glm::mat4 setViewMatrix) {
	viewMatrix = setViewMatrix;
}

void Camera3D::setProjectionMatrix(glm::mat4 setProjectionMatrix) {
	projectionMatrix = setProjectionMatrix;
}

void Camera3D::focusAtPointOnPlane(glm::vec3 point, glm::vec3 offset) {
	glm::vec3 angle = glm::normalize(point - position);
	setRotation(angle.x + offset.x, angle.y + offset.y, offset.z);
	forceValuesWithinRange(&rotation, 0, glm::pi<float>() * 2);
}

void Camera3D::focusAtPointOnPlane(glm::vec3 point) {
	glm::vec3 angle = glm::normalize(point - position);
	setRotation(angle.x, angle.y, 0);
}

void Camera3D::focusAtPointOnPlane(float x, float y, float z) {
	glm::vec3 point(x, y, z);
	focusAtPointOnPlane(point);
}

void Camera3D::calculateCartesionalDirection() {
	cartesionalDirection = glm::vec3(
		cos(rotation.y) * sin(rotation.x),
		sin(rotation.y),
		cos(rotation.y) * cos(rotation.x)
	);
}

void Camera3D::calculateRightVector() {
	right = glm::vec3(
		sin(rotation.x - 3.14f / 2.0f),
		0,
		cos(rotation.x - 3.14f / 2.0f)
	);
}

void Camera3D::calculateUpVector() {
	up = glm::cross(right, cartesionalDirection);
}

glm::vec3* Camera3D::getCartesionalDirection() {
	return &cartesionalDirection;
}

glm::vec3* Camera3D::getRightVector() {
	return &right;
}

glm::vec3* Camera3D::getUpVector() {
	return &up;
}
