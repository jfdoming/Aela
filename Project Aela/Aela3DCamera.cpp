#include "Aela3DCamera.h"

void Aela3DCamera::setPosition(float setX, float setY, float setZ) {
	xPosition = setX;
	yPosition = setY;
	zPosition = setZ;
	position = glm::vec3(setX, setY, setZ);
}

void Aela3DCamera::setPosition(glm::vec3 setPosition) {
	position = setPosition;
	xPosition = position.x;
	yPosition = position.y;
	zPosition = position.z;
}

glm::vec3 Aela3DCamera::getPosition() {
	return position;
}

void Aela3DCamera::getPosition(float * setXPosition, float * setYPosition, float * setZPosition) {
	*setXPosition = xPosition;
	*setYPosition = yPosition;
	*setZPosition = zPosition;
}

void Aela3DCamera::setRotation(float setX, float setY, float setZ) {
	xRotation = setX;
	yRotation = setY;
	zRotation = setZ;
	rotation = glm::vec3(setX, setY, setZ);
}

void Aela3DCamera::setRotation(glm::vec3 setRotation) {
	rotation = setRotation;
	xRotation = rotation.x;
	yRotation = rotation.y;
	zRotation = rotation.z;
}

glm::vec3 Aela3DCamera::getRotation() {
	return rotation;
}

void Aela3DCamera::getRotation(float * setXPosition, float * setYPosition, float * setZPosition) {
	*setXPosition = xRotation;
	*setYPosition = yRotation;
	*setZPosition = zRotation;
}


void Aela3DCamera::setProperty(Aela3DProperty property, float value) {
	if (property == Aela3DProperty::X_POSITION) {
		xPosition = value;
		position = glm::vec3(xPosition, yPosition, zPosition);
	}
	if (property == Aela3DProperty::Y_POSITION) {
		yPosition = value;
		position = glm::vec3(xPosition, yPosition, zPosition);
	}
	if (property == Aela3DProperty::Z_POSITION) {
		zPosition = value;
		position = glm::vec3(xPosition, yPosition, zPosition);
	}
	if (property == Aela3DProperty::X_ROTATION) {
		xRotation = value;
		rotation = glm::vec3(xRotation, yRotation, zRotation);
	}
	if (property == Aela3DProperty::Y_ROTATION) {
		yRotation = value;
		rotation = glm::vec3(xRotation, yRotation, zRotation);
	}
	if (property == Aela3DProperty::Z_ROTATION) {
		zRotation = value;
		rotation = glm::vec3(xRotation, yRotation, zRotation);
	}
}

float Aela3DCamera::getProperty(Aela3DProperty property) {
	if (property == Aela3DProperty::X_POSITION) {
		return xPosition;
	}
	if (property == Aela3DProperty::Y_POSITION) {
		return yPosition;
	}
	if (property == Aela3DProperty::Z_POSITION) {
		return zPosition;
	}
	if (property == Aela3DProperty::X_ROTATION) {
		return xRotation;
	}
	if (property == Aela3DProperty::Y_ROTATION) {
		return yRotation;
	}
	if (property == Aela3DProperty::Z_ROTATION) {
		return zRotation;
	}
}

glm::mat4 Aela3DCamera::getViewMatrix() {
	return viewMatrix;
}

glm::mat4 Aela3DCamera::getProjectionMatrix() {
	return projectionMatrix;
}

void Aela3DCamera::setFieldOfView(float setFieldOfView) {
	fieldOfView = setFieldOfView;
}

float Aela3DCamera::getFieldOfView() {
	return fieldOfView;
}

void Aela3DCamera::setViewMatrix(glm::mat4 setViewMatrix) {
	viewMatrix = setViewMatrix;
}

void Aela3DCamera::setProjectionMatrix(glm::mat4 setProjectionMatrix) {
	projectionMatrix = setProjectionMatrix;
}