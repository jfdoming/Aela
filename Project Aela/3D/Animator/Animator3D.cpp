/*
* Class: Project Aela's 3D Animator3D
* Author: Robert Ciborowski
* Date: 10/07/2017
* Description: A class used by Aela's 3D Animator to perform transformations on 3D objects.
*/

#include "Animator3D.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>

// This updates 3D objects based on keyframes given to the animator.
void Animator3D::update() {
	if (keyFrameLists.size() != 0) {
		// This keeps track of timing.
		timeSinceLastKeyFrameList += (unsigned int) timeManager->getTimeBetweenFrames();
		unsigned int timeOfFrame0 = keyFrameLists.at(0).getTimeAfterPreviousKeyFrame();

		KeyFrame3DList* keyFrameList = &keyFrameLists.at(0);
		std::vector<KeyFrame3D>* keyFrameVector = keyFrameList->getKeyFrames();

		// This erases the current key frame if it has already passed.
		if (keyFrameList->getTimeAfterPreviousKeyFrame() <= timeSinceLastKeyFrameList) {
			// This sets the transformations of all of the objects to their final transformations.
			for (unsigned int whichFrame = 0; whichFrame < keyFrameVector->size(); whichFrame++) {
				KeyFrame3D* keyFrame = &keyFrameVector->at(whichFrame);
				Transformable3D* object = keyFrame->getObject();

				// This finds the final scaling of the model.
				object->setScaling(*keyFrame->getScaling());

				// This finds the matrix for the point rotation.
				glm::mat4 pointRotationMatrix = glm::mat4(1);
				glm::vec3* pointRotationRotation = keyFrame->getPointRotation()->getRotation();
				pointRotationMatrix *= glm::eulerAngleYXZ(pointRotationRotation->y, pointRotationRotation->x, pointRotationRotation->z);
				
				// This finds the final position and rotation values and applies them.
				object->setPosition(*keyFrame->getTranslation() + glm::vec3(pointRotationMatrix
					* glm::vec4(*keyFrame->getPointRotation()->getPoint() * glm::vec3(-1), 0)) + *keyFrame->getPointRotation()->getPoint());
				object->setRotation(*keyFrame->getRotation() + *pointRotationRotation);
			}
			keyFrameLists.erase(keyFrameLists.begin());
			timeSinceLastKeyFrameList = 0;
			if (keyFrameLists.size() == 0) {
				return;
			}
			
			// This sets up the next key frame list.
			keyFrameList = &keyFrameLists.at(0);
			keyFrameVector = keyFrameList->getKeyFrames();
			keyFrameList->storeOriginalTransformations();
		}

		// This updates objects using the current key frame list.
		for (unsigned int whichFrame = 0; whichFrame < keyFrameVector->size(); whichFrame++) {
			KeyFrame3D* keyFrame = &keyFrameVector->at(whichFrame);
			Transformable3D* object = keyFrame->getObject();

			// This figures out a bunch of transformational values and finds the final scaling value to use.
			unsigned int timing = keyFrameList->getTimeAfterPreviousKeyFrame() + 1;
			glm::vec3* originalPosition = &keyFrameList->getOriginalPositions()->at(whichFrame);
			glm::vec3* originalRotation = &keyFrameList->getOriginalRotations()->at(whichFrame);
			glm::vec3* originalScaling = &keyFrameList->getOriginalScalings()->at(whichFrame);
			glm::vec3* finalTranslation = keyFrame->getTranslation();
			glm::vec3 newPosition((((float) finalTranslation->x - originalPosition->x) / timing) * timeSinceLastKeyFrameList,
				(((float) finalTranslation->y - originalPosition->y) / timing) * timeSinceLastKeyFrameList,
				(((float) finalTranslation->z - originalPosition->z) / timing) * timeSinceLastKeyFrameList);
			glm::vec3* finalRotation = keyFrame->getRotation();
			glm::vec3 newRotation((((float) finalRotation->x - originalRotation->x) / timing) * timeSinceLastKeyFrameList,
				(((float) finalRotation->y - originalRotation->y) / timing) * timeSinceLastKeyFrameList,
				(((float) finalRotation->z - originalRotation->z) / timing) * timeSinceLastKeyFrameList);
			glm::vec3* finalScaling = keyFrame->getScaling();
			glm::vec3 newScaling(originalScaling->x + (((float) finalScaling->x - originalScaling->x) / timing) * timeSinceLastKeyFrameList,
				originalScaling->y + (((float) finalScaling->y - originalScaling->y) / timing) * timeSinceLastKeyFrameList,
				originalScaling->z + (((float) finalScaling->z - originalScaling->z) / timing) * timeSinceLastKeyFrameList);
			object->setScaling(newScaling);

			// This finds the matrix for the point rotation.
			glm::mat4 pointRotationMatrix = glm::mat4(1);
			glm::vec3* pointRotationRotation = keyFrame->getPointRotation()->getRotation();
			pointRotationMatrix *= glm::eulerAngleYXZ(pointRotationRotation->y / timing * timeSinceLastKeyFrameList,
				pointRotationRotation->x / timing * timeSinceLastKeyFrameList, pointRotationRotation->z / timing * timeSinceLastKeyFrameList);

			// This finds the final position and rotation values and applies them.
			object->setPosition(*originalPosition + newPosition + glm::vec3(pointRotationMatrix
				* glm::vec4(*keyFrame->getPointRotation()->getPoint() * glm::vec3(-1), 0)) + *keyFrame->getPointRotation()->getPoint());
			object->setRotation(*originalRotation + newRotation + *pointRotationRotation * glm::vec3((float) timeSinceLastKeyFrameList / timing));
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
