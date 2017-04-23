#include "Object3D.h"


void Object3D::setPosition(float setX, float setY, float setZ) {
	position = glm::vec3(setX, setY, setZ);
}

void Object3D::setPosition(glm::vec3 setPosition) {
	position = setPosition;
}

glm::vec3* Object3D::getPosition() {
	return &position;
}

void Object3D::getPosition(float* setXPosition, float* setYPosition, float* setZPosition) {
	*setXPosition = position.x;
	*setYPosition = position.y;
	*setZPosition = position.z;
}

void Object3D::setRotation(float setX, float setY, float setZ) {
	rotation = glm::vec3(setX, setY, setZ);
}

void Object3D::setRotation(glm::vec3 setRotation) {
	rotation = setRotation;
}

glm::vec3* Object3D::getRotation() {
	return &rotation;
}

void Object3D::getRotation(float* setXPosition, float* setYPosition, float* setZPosition) {
	*setXPosition = rotation.x;
	*setYPosition = rotation.y;
	*setZPosition = rotation.z;
}

void Object3D::setProperty(Object3DProperty property, float value) {
	if (property == Object3DProperty::X_POSITION) {
		position.x = value;
	}
	if (property == Object3DProperty::Y_POSITION) {
		position.y = value;
	}
	if (property == Object3DProperty::Z_POSITION) {
		position.z = value;
	}
	if (property == Object3DProperty::X_ROTATION) {
		rotation.x = value;
	}
	if (property == Object3DProperty::Y_ROTATION) {
		rotation.y = value;
	}
	if (property == Object3DProperty::Z_ROTATION) {
		rotation.z = value;
	}
}

float Object3D::getProperty(Object3DProperty property) {
	if (property == Object3DProperty::X_POSITION) {
		return position.x;
	}
	if (property == Object3DProperty::Y_POSITION) {
		return position.y;
	}
	if (property == Object3DProperty::Z_POSITION) {
		return position.z;
	}
	if (property == Object3DProperty::X_ROTATION) {
		return rotation.x;
	}
	if (property == Object3DProperty::Y_ROTATION) {
		return rotation.y;
	}
	if (property == Object3DProperty::Z_ROTATION) {
		return rotation.z;
	}
	return 0;
}