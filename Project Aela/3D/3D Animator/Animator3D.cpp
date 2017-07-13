/*
* Class: Project Aela's 3D Animator3D
* Author: Robert Ciborowski
* Date: 10/07/2017
* Description: A class used by Aela's 3D Animator to perform transformations on 3D objects.
*/

#include "Animator3D.h"

void Animator3D::update() {
	if (keyFrameLists.size() != 0) {
		timeSinceLastKeyFrameList += (unsigned int) timeManager->getTimeBetweenFrames();
		unsigned int timeOfFrame0 = keyFrameLists.at(0).getTimeAfterPreviousKeyFrame();

		KeyFrame3DList* keyFrameList = &keyFrameLists.at(0);
		std::vector<KeyFrame3D>* keyFrameVector = keyFrameList->getKeyFrames();

		if (keyFrameList->getTimeAfterPreviousKeyFrame() <= timeSinceLastKeyFrameList) {
			for (unsigned int whichFrame = 0; whichFrame < keyFrameVector->size(); whichFrame++) {
				KeyFrame3D* keyFrame = &keyFrameVector->at(whichFrame);
				Object3D* object = keyFrame->getObject();

				object->setPosition(*keyFrame->getTranslation());
				object->setRotation(*keyFrame->getRotation());
			}
			keyFrameLists.erase(keyFrameLists.begin());
			timeSinceLastKeyFrameList = 0;
			if (keyFrameLists.size() == 0) {
				return;
			}
			keyFrameList = &keyFrameLists.at(0);
			keyFrameVector = keyFrameList->getKeyFrames();
			keyFrameList->storeOriginalTransformations();
		}

		for (unsigned int whichFrame = 0; whichFrame < keyFrameVector->size(); whichFrame++) {
			KeyFrame3D* keyFrame = &keyFrameVector->at(whichFrame);
			Object3D* object = keyFrame->getObject();

			unsigned int timing = keyFrameList->getTimeAfterPreviousKeyFrame() + 1;
			glm::vec3* originalPosition = &keyFrameList->getOriginalPositions()->at(whichFrame);
			glm::vec3* originalRotation = &keyFrameList->getOriginalRotations()->at(whichFrame);
			if (keyFrame->getTranslation() != NULL) {
				glm::vec3* finalTranslation = keyFrame->getTranslation();
				glm::vec3 newPosition(originalPosition->x + (((float) finalTranslation->x - originalPosition->x) / timing) * timeSinceLastKeyFrameList,
					originalPosition->y + (((float) finalTranslation->y - originalPosition->y) / timing) * timeSinceLastKeyFrameList,
					originalPosition->z + (((float) finalTranslation->z - originalPosition->z) / timing) * timeSinceLastKeyFrameList);
				object->setPosition(newPosition);
			}
			if (keyFrame->getRotation() != NULL) {
				glm::vec3* finalRotation = keyFrame->getRotation();
				glm::vec3 newRotation(originalRotation->x + (((float) finalRotation->x - originalRotation->x) / timing) * timeSinceLastKeyFrameList,
					originalRotation->y + (((float) finalRotation->y - originalRotation->y) / timing) * timeSinceLastKeyFrameList,
					originalRotation->z + (((float) finalRotation->z - originalRotation->z) / timing) * timeSinceLastKeyFrameList);
				object->setRotation(newRotation);
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

void Animator3D::addKeyFrameList(KeyFrame3DList* keyFrameList) {
	if (keyFrameLists.size() == 0) {
		keyFrameList->storeOriginalTransformations();
	}
	keyFrameLists.push_back(*keyFrameList);
}

std::vector<KeyFrame3DList>* Animator3D::getKeyFrames() {
	return &keyFrameLists;
}
