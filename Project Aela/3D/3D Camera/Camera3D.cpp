/*
* Name: Project Aela's 3D Camera
* Author: Robert Ciborowski
* Date: November 2016
* Description: A class used by Project Aela's Renderer to store properties of a camera.
*/

#include "Camera3D.h"

void Camera3D::setPosition(float setX, float setY, float setZ) {
	position = glm::vec3(setX, setY, setZ);
}

void Camera3D::setPosition(glm::vec3 setPosition) {
	position = setPosition;
}

glm::vec3 Camera3D::getPosition() {
	return position;
}

void Camera3D::getPosition(float* setXPosition, float* setYPosition, float* setZPosition) {
	*setXPosition = position.x;
	*setYPosition = position.y;
	*setZPosition = position.z;
}

void Camera3D::setRotation(float setX, float setY, float setZ) {
	rotation = glm::vec3(setX, setY, setZ);
}

void Camera3D::setRotation(glm::vec3 setRotation) {
	rotation = setRotation;
}

glm::vec3 Camera3D::getRotation() {
	return rotation;
}

void Camera3D::getRotation(float* setXPosition, float* setYPosition, float* setZPosition) {
	*setXPosition = rotation.x;
	*setYPosition = rotation.y;
	*setZPosition = rotation.z;
}


void Camera3D::setProperty(Model3DProperty property, float value) {
	if (property == Model3DProperty::X_POSITION) {
		position.x = value;
	}
	if (property == Model3DProperty::Y_POSITION) {
		position.y = value;
	}
	if (property == Model3DProperty::Z_POSITION) {
		position.z = value;
	}
	if (property == Model3DProperty::X_ROTATION) {
		rotation.x = value;
	}
	if (property == Model3DProperty::Y_ROTATION) {
		rotation.y = value;
	}
	if (property == Model3DProperty::Z_ROTATION) {
		rotation.z = value;
	}
}

float Camera3D::getProperty(Model3DProperty property) {
	if (property == Model3DProperty::X_POSITION) {
		return position.x;
	}
	if (property == Model3DProperty::Y_POSITION) {
		return position.y;
	}
	if (property == Model3DProperty::Z_POSITION) {
		return position.z;
	}
	if (property == Model3DProperty::X_ROTATION) {
		return rotation.x;
	}
	if (property == Model3DProperty::Y_ROTATION) {
		return rotation.y;
	}
	if (property == Model3DProperty::Z_ROTATION) {
		return rotation.z;
	}
	return 0;
}

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
