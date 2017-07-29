/*
* Class: 3D Camera
* Author: Robert Ciborowski
* Date: November 2016
* Description: A class used by Project Aela's Renderer to store properties of a camera.
*/

#include "Camera3D.h"

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

void Camera3D::setViewMatrix(glm::mat4 setViewMatrix) {
	viewMatrix = setViewMatrix;
}

void Camera3D::setProjectionMatrix(glm::mat4 setProjectionMatrix) {
	projectionMatrix = setProjectionMatrix;
}

void Camera3D::focusAtPointOnPlane(glm::vec3 point, glm::vec3 offset) {
	glm::vec3 angle = glm::normalize(point - position);
	setRotation(angle.x + offset.x, angle.y + offset.y, offset.z);
}

void Camera3D::focusAtPointOnPlane(glm::vec3 point) {
	glm::vec3 angle = glm::normalize(point - position);
	setRotation(angle.x, angle.y, 0);
}

void Camera3D::focusAtPointOnPlane(float x, float y, float z) {
	glm::vec3 point(x, y, z);
	focusAtPointOnPlane(point);
}