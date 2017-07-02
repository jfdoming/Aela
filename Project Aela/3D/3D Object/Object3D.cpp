#include "Object3D.h"
#include <iostream>

#define PI 3.14159265358979323846

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

void Object3D::translate(glm::vec3 translation) {
	position += translation;
}

void Object3D::rotate(glm::vec3 rotation) {
	this->rotation += rotation;
	forceValuesWithinRange(&(this->rotation), 0, PI * 2);
}

void Object3D::translate(float x, float y, float z) {
	translate(glm::vec3(x, y, z));
}

void Object3D::rotate(float x, float y, float z) {
	rotate(glm::vec3(x, y, z));
}

void Object3D::forceValuesWithinRange(glm::vec3* vec3, float minimum, float maximum) {
	if (vec3->x < minimum) {
		vec3->x += (maximum - minimum) * ceil(abs((minimum - vec3->x) / (maximum - minimum)));
	}

	if (vec3->x > maximum) {
		vec3->x -= (maximum - minimum) * ceil(abs((maximum - vec3->x) / (maximum - minimum)));
	}

	if (vec3->y < minimum) {
		vec3->y += (maximum - minimum) * ceil(abs((minimum - vec3->y) / (maximum - minimum)));
	}

	if (vec3->y > maximum) {
		vec3->y -= (maximum - minimum) * ceil(abs((maximum - vec3->y) / (maximum - minimum)));
	}

	if (vec3->z < minimum) {
		vec3->z += (maximum - minimum) * ceil(abs((minimum - vec3->z) / (maximum - minimum)));
	}

	if (vec3->z > maximum) {
		vec3->z -= (maximum - minimum) * ceil(abs((maximum - vec3->z) / (maximum - minimum)));
	}
}


