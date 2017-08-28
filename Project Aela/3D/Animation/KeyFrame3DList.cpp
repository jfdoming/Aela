#include "KeyFrame3DList.h"

std::vector<glm::vec3>* KeyFrame3DList::getOriginalPositions() {
	return &originalPositions;
}

std::vector<glm::vec3>* KeyFrame3DList::getOriginalRotations() {
	return &originalRotations;
}

std::vector<glm::vec3>* KeyFrame3DList::getOriginalScalings() {
	return &originalScalings;
}

void KeyFrame3DList::storeOriginalTransformations() {
	for (KeyFrame3D keyFrame : keyFrames) {
		if (keyFrame.getObject() != nullptr) {
			originalPositions.push_back(*keyFrame.getObject()->getPosition());
			originalRotations.push_back(*keyFrame.getObject()->getRotation());
			originalScalings.push_back(*keyFrame.getObject()->getScaling());
		} else {
			// You still need to put something in the original property vectors.
			originalPositions.push_back(glm::vec3(0));
			originalRotations.push_back(glm::vec3(0));
			originalScalings.push_back(glm::vec3(0));
		}
	}
}

