#include "Transformable3D.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>

void Transformable3D::setPosition(float setX, float setY, float setZ) {
	position = glm::vec3(setX, setY, setZ);
}

void Transformable3D::setPosition(glm::vec3 setPosition) {
	position = setPosition;
}

glm::vec3* Transformable3D::getPosition() {
	return &position;
}

void Transformable3D::getPosition(float* setX, float* setY, float* setZ) {
	*setX = position.x;
	*setY = position.y;
	*setZ = position.z;
}

void Transformable3D::setRotation(float setX, float setY, float setZ) {
	rotation = glm::vec3(setX, setY, setZ);
}

void Transformable3D::setRotation(glm::vec3 setRotation) {
	rotation = setRotation;
}

glm::vec3* Transformable3D::getRotation() {
	return &rotation;
}

void Transformable3D::getRotation(float* setX, float* setY, float* setZ) {
	*setX = rotation.x;
	*setY = rotation.y;
	*setZ = rotation.z;
}

void Transformable3D::setScaling(float setX, float setY, float setZ) {
	scaling = glm::vec3(setX, setY, setZ);
}

void Transformable3D::setScaling(glm::vec3 setScaling) {
	scaling = setScaling;
}

glm::vec3* Transformable3D::getScaling() {
	return &scaling;
}

void Transformable3D::getScaling(float* setX, float* setY, float* setZ) {
	*setX = scaling.x;
	*setY = scaling.y;
	*setZ = scaling.z;
}

void Transformable3D::setProperty(Transformable3DProperty property, float value) {
	switch (property) {
		case Transformable3DProperty::X_POSITION:
			position.x = value;
			break;
		case Transformable3DProperty::Y_POSITION:
			position.y = value;
			break;
		case Transformable3DProperty::Z_POSITION:
			position.z = value;
			break;
		case Transformable3DProperty::X_ROTATION:
			rotation.x = value;
			break;
		case Transformable3DProperty::Y_ROTATION:
			rotation.y = value;
			break;
		case Transformable3DProperty::Z_ROTATION:
			rotation.z = value;
			break;
		case Transformable3DProperty::X_SCALING:
			scaling.x = value;
			break;
		case Transformable3DProperty::Y_SCALING:
			scaling.y = value;
			break;
		case Transformable3DProperty::Z_SCALING:
			scaling.z = value;
			break;
	}
}

float Transformable3D::getProperty(Transformable3DProperty property) {
	switch (property) {
	case Transformable3DProperty::X_POSITION:
		return position.x;
		break;
	case Transformable3DProperty::Y_POSITION:
		return position.y;
		break;
	case Transformable3DProperty::Z_POSITION:
		return position.z;
		break;
	case Transformable3DProperty::X_ROTATION:
		return rotation.x;
		break;
	case Transformable3DProperty::Y_ROTATION:
		return rotation.y;
		break;
	case Transformable3DProperty::Z_ROTATION:
		return rotation.z;
		break;
	case Transformable3DProperty::X_SCALING:
		return scaling.x;
		break;
	case Transformable3DProperty::Y_SCALING:
		return scaling.y;
		break;
	case Transformable3DProperty::Z_SCALING:
		return scaling.z;
		break;
	default:
		return 0;
	}
}

void Transformable3D::translate(glm::vec3 translation) {
	position += translation;
}

void Transformable3D::rotate(glm::vec3 rotation) {
	this->rotation += rotation;
	forceValuesWithinRange(&(this->rotation), 0, (float) glm::pi<float>() * 2);
}

void Transformable3D::translate(float x, float y, float z) {
	translate(glm::vec3(x, y, z));
}

void Transformable3D::rotate(float x, float y, float z) {
	rotate(glm::vec3(x, y, z));
}

void Transformable3D::rotateAroundPoint(PointRotation3D* pointRotation) {
	rotateAroundPoint(pointRotation->getRotation(), pointRotation->getPoint());
}

void Transformable3D::rotateAroundPoint(glm::vec3* rotation, glm::vec3* point) {
	glm::mat4 pointRotationMatrix = glm::mat4(1);
	pointRotationMatrix *= glm::eulerAngleYXZ(rotation->y, rotation->x, rotation->z);
	position += glm::vec3(pointRotationMatrix * glm::vec4(*point * glm::vec3(-1), 0)) + *point;
	this->rotation += *rotation;
}

void Transformable3D::forceValuesWithinRange(glm::vec3* vec3, float minimum, float maximum) {
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

void Transformable3D::cloneTransformable(Transformable3D* transformable) {
	this->position = *transformable->getPosition();
	this->rotation = *transformable->getRotation();
	this->scaling = *transformable->getScaling();
}


