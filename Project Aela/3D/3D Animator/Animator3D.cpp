/*
* Class: Project Aela's 3D Animator3D
* Author: Robert Ciborowski
* Date: 10/07/2017
* Description: A class used by Aela's 3D Animator to perform transformations on 3D objects.
*/

#include "Animator3D.h"

void Animator3D::update() {
	if (keyFrames.size() != 0) {
		timeSinceLastKeyFrame += (unsigned int) timeManager->getTimeBetweenFrames();
		unsigned int timeOfFrame0 = keyFrames.at(0).getTimeAfterPreviousKeyFrame();

		for (unsigned int whichFrame = 0; whichFrame == 0 || (whichFrame < keyFrames.size() && keyFrames.at(whichFrame).getTimeAfterPreviousKeyFrame() == 0); whichFrame++) {
			KeyFrame3D* keyFrame = &keyFrames.at(whichFrame);
			Object3D* object = keyFrame->getObject();

			if (keyFrames.at(whichFrame).getTimeAfterPreviousKeyFrame() <= timeSinceLastKeyFrame) {
				object->setPosition(*keyFrame->getTranslation());
				object->setRotation(*keyFrame->getRotation());
				keyFrames.erase(keyFrames.begin());
				originalPositions.erase(originalPositions.begin() + whichFrame);
				originalRotations.erase(originalRotations.begin() + whichFrame);
				timeSinceLastKeyFrame = 0;
				if (keyFrames.size() == 0) {
					break;
				} else {
					originalPositions.insert(originalPositions.begin() + whichFrame, *keyFrames.at(whichFrame).getObject()->getPosition());
					originalRotations.insert(originalRotations.begin() + whichFrame, *keyFrames.at(whichFrame).getObject()->getRotation());
					whichFrame--;
				}
			} else {
				unsigned int timing = keyFrames.at(whichFrame).getTimeAfterPreviousKeyFrame() + 1;
				glm::vec3* originalPosition = &originalPositions.at(whichFrame);
				glm::vec3* originalRotation = &originalRotations.at(whichFrame);
				if (keyFrame->getTranslation() != NULL) {
					glm::vec3* finalTranslation = keyFrame->getTranslation();
					glm::vec3 newPosition(originalPosition->x + (((float) finalTranslation->x - originalPosition->x) / timing) * timeSinceLastKeyFrame,
						originalPosition->y + (((float) finalTranslation->y - originalPosition->y) / timing) * timeSinceLastKeyFrame,
						originalPosition->z + (((float) finalTranslation->z - originalPosition->z) / timing) * timeSinceLastKeyFrame);
					object->setPosition(newPosition);
					std::cout << originalPosition->z + (((float) finalTranslation->z - originalPosition->z) / timing) * timeSinceLastKeyFrame << " is stuff, " << whichFrame << " is the frame.\n";
				}
				if (keyFrame->getRotation() != NULL) {
					glm::vec3* finalRotation = keyFrame->getRotation();
					glm::vec3 newRotation(originalRotation->x + (((float) finalRotation->x - originalRotation->x) / timing) * timeSinceLastKeyFrame,
						originalRotation->y + (((float) finalRotation->y - originalRotation->y) / timing) * timeSinceLastKeyFrame,
						originalRotation->z + (((float) finalRotation->z - originalRotation->z) / timing) * timeSinceLastKeyFrame);
					object->setRotation(newRotation);
				}
			}
			if ((whichFrame < keyFrames.size() && keyFrames.at(whichFrame).getTimeAfterPreviousKeyFrame() == 0)) {
				std::cout << "";
			}
		}
	}
}

void Animator3D::setTimeManager(TimeManager* timeManager) {
	this->timeManager = timeManager;
}

TimeManager* Animator3D::getTimeManager() {
	return timeManager;
}

void Animator3D::addKeyFrame(KeyFrame3D* keyFrame) {
	keyFrames.push_back(*keyFrame);
	if (keyFrames.size() == 1) {
		originalPositions.insert(originalPositions.begin(), *keyFrame->getObject()->getPosition());
		originalRotations.insert(originalRotations.begin(), *keyFrame->getObject()->getRotation());
		timeSinceLastKeyFrame = 0;
		std::cout << keyFrame->getTimeAfterPreviousKeyFrame() << " - Added a new key frame. \n";
	}
}

std::vector<KeyFrame3D>* Animator3D::getKeyFrames() {
	return &keyFrames;
}
