#include "KeyFrame3DList.h"

void KeyFrame3DList::setTimeAfterPreviousKeyFrame(unsigned int time) {
	if (time != 0) {
		timeAfterPreviousKeyFrame = time;
	}
}

unsigned int KeyFrame3DList::getTimeAfterPreviousKeyFrame() {
	return timeAfterPreviousKeyFrame;
}

void KeyFrame3DList::addKeyFrame(KeyFrame3D* keyFrame) {
	keyFrames.push_back(*keyFrame);
}

std::vector<KeyFrame3D>* KeyFrame3DList::getKeyFrames() {
	return &keyFrames;
}

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
		originalPositions.push_back(*keyFrame.getObject()->getPosition());
		originalRotations.push_back(*keyFrame.getObject()->getRotation());
		originalScalings.push_back(*keyFrame.getObject()->getScaling());
	}
}

