#include "Camera3D.h"

void Camera3D::setPosition(float setX, float setY, float setZ) {
	xPosition = setX;
	yPosition = setY;
	zPosition = setZ;
	position = glm::vec3(setX, setY, setZ);
}

void Camera3D::setPosition(glm::vec3 setPosition) {
	position = setPosition;
	xPosition = position.x;
	yPosition = position.y;
	zPosition = position.z;
}

glm::vec3 Camera3D::getPosition() {
	return position;
}

void Camera3D::getPosition(float * setXPosition, float * setYPosition, float * setZPosition) {
	*setXPosition = xPosition;
	*setYPosition = yPosition;
	*setZPosition = zPosition;
}

void Camera3D::setRotation(float setX, float setY, float setZ) {
	xRotation = setX;
	yRotation = setY;
	zRotation = setZ;
	rotation = glm::vec3(setX, setY, setZ);
}

void Camera3D::setRotation(glm::vec3 setRotation) {
	rotation = setRotation;
	xRotation = rotation.x;
	yRotation = rotation.y;
	zRotation = rotation.z;
}

glm::vec3 Camera3D::getRotation() {
	return rotation;
}

void Camera3D::getRotation(float * setXPosition, float * setYPosition, float * setZPosition) {
	*setXPosition = xRotation;
	*setYPosition = yRotation;
	*setZPosition = zRotation;
}


void Camera3D::setProperty(Model3DProperty property, float value) {
	if (property == Model3DProperty::X_POSITION) {
		xPosition = value;
		position = glm::vec3(xPosition, yPosition, zPosition);
	}
	if (property == Model3DProperty::Y_POSITION) {
		yPosition = value;
		position = glm::vec3(xPosition, yPosition, zPosition);
	}
	if (property == Model3DProperty::Z_POSITION) {
		zPosition = value;
		position = glm::vec3(xPosition, yPosition, zPosition);
	}
	if (property == Model3DProperty::X_ROTATION) {
		xRotation = value;
		rotation = glm::vec3(xRotation, yRotation, zRotation);
	}
	if (property == Model3DProperty::Y_ROTATION) {
		yRotation = value;
		rotation = glm::vec3(xRotation, yRotation, zRotation);
	}
	if (property == Model3DProperty::Z_ROTATION) {
		zRotation = value;
		rotation = glm::vec3(xRotation, yRotation, zRotation);
	}
}

float Camera3D::getProperty(Model3DProperty property) {
	if (property == Model3DProperty::X_POSITION) {
		return xPosition;
	}
	if (property == Model3DProperty::Y_POSITION) {
		return yPosition;
	}
	if (property == Model3DProperty::Z_POSITION) {
		return zPosition;
	}
	if (property == Model3DProperty::X_ROTATION) {
		return xRotation;
	}
	if (property == Model3DProperty::Y_ROTATION) {
		return yRotation;
	}
	if (property == Model3DProperty::Z_ROTATION) {
		return zRotation;
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