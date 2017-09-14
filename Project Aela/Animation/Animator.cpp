/*
* Class: Project Aela's 3D Animator
* Author: Robert Ciborowski
* Date: 10/07/2017
* Description: A class used by Aela's 3D Animator to perform transformations on 3D objects.
*/

#include "Animator.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>

// This updates 3D objects based on keyframes given to the Animator. I could split this function to smaller functions
// but its easier to read what is going on when everything is layed out for you. Besides, those functions would be
// called in one spot anyways.
void Animator::update() {
	if (keyFrame3DLists.size() != 0) {
		// This keeps track of timing.
		timeSinceLastKeyFrame3DList += (unsigned int) timeManager->getTimeBetweenFrames();
		unsigned int timeOfFrame0 = keyFrame3DLists.at(0).getTimeAfterPreviousKeyFrame();

		KeyFrame3DList* keyFrameList = &keyFrame3DLists.at(0);
		std::vector<KeyFrame3D>* keyFrameVector = keyFrameList->getKeyFrames();

		// This erases the current key frame if it has already passed.
		if (keyFrameList->getTimeAfterPreviousKeyFrame() <= timeSinceLastKeyFrame3DList) {
			// This sets the transformations of all of the objects to their final transformations. It also runs their
			// action.
			for (KeyFrame3D keyFrame : *keyFrameVector) {
				Transformable3D* object = keyFrame.getObject();

				if (keyFrame.getEndingAction() != nullptr) {
					keyFrame.getEndingAction()();
				}

				if (object != nullptr) {
					// This finds the final scaling of the model.
					if (keyFrame.isUsingScaling()) {
						object->setScaling(*keyFrame.getScaling());
					}

					glm::mat4 pointRotationMatrix = glm::mat4(1);
					glm::vec3* pointRotationValue = nullptr;
					if (keyFrame.isUsingPointRotation()) {
						// This finds the matrix for the point rotation.
						pointRotationValue = keyFrame.getPointRotation()->getRotation();
						pointRotationMatrix *= glm::eulerAngleYXZ(pointRotationValue->y, pointRotationValue->x, pointRotationValue->z);
					}

					// This finds the final position and rotation values and applies them.
					if (keyFrame.isUsingTranslation()) {
						object->setPosition(*keyFrame.getTranslation() + glm::vec3(pointRotationMatrix
							* glm::vec4(*keyFrame.getPointRotation()->getPoint() * glm::vec3(-1), 0)) + *keyFrame.getPointRotation()->getPoint());
					}

					if (keyFrame.isUsingPointRotation() && keyFrame.isUsingRotation()) {
						object->setRotation(*keyFrame.getRotation() + *pointRotationValue);
					} else if (keyFrame.isUsingRotation()) {
						object->setRotation(*keyFrame.getRotation());
					} else if (keyFrame.isUsingPointRotation()) {
						object->setRotation(*pointRotationValue);
					}
				}
			}
			keyFrame3DLists.erase(keyFrame3DLists.begin());
			timeSinceLastKeyFrame3DList = 0;
			if (keyFrame3DLists.size() == 0) {
				return;
			}
			
			// This sets up the next key frame list.
			keyFrameList = &keyFrame3DLists.at(0);
			keyFrameVector = keyFrameList->getKeyFrames();
			keyFrameList->storeOriginalTransformations();
		}

		// This updates objects using the current key frame list.
		for (unsigned int whichFrame = 0; whichFrame < keyFrameVector->size(); whichFrame++) {
			KeyFrame3D* keyFrame = &keyFrameVector->at(whichFrame);
			Transformable3D* object = keyFrame->getObject();

			if (object != nullptr) {
				// This figures out a bunch of transformational values and finds the final scaling value to use.
				unsigned int timing = keyFrameList->getTimeAfterPreviousKeyFrame() + 1;
				glm::vec3* originalPosition = &keyFrameList->getOriginalPositions()->at(whichFrame);
				glm::vec3* originalRotation = &keyFrameList->getOriginalRotations()->at(whichFrame);
				glm::vec3* originalScaling = &keyFrameList->getOriginalScalings()->at(whichFrame);
				glm::vec3 newPosition, newRotation;
				if (keyFrame->isUsingTranslation()) {
					glm::vec3* finalTranslation = keyFrame->getTranslation();
					newPosition = glm::vec3((((float)finalTranslation->x - originalPosition->x) / timing) * timeSinceLastKeyFrame3DList,
						(((float)finalTranslation->y - originalPosition->y) / timing) * timeSinceLastKeyFrame3DList,
						(((float)finalTranslation->z - originalPosition->z) / timing) * timeSinceLastKeyFrame3DList);
				}
				if (keyFrame->isUsingRotation()) {
					glm::vec3* finalRotation = keyFrame->getRotation();
					newRotation = glm::vec3((((float)finalRotation->x - originalRotation->x) / timing) * timeSinceLastKeyFrame3DList,
						(((float)finalRotation->y - originalRotation->y) / timing) * timeSinceLastKeyFrame3DList,
						(((float)finalRotation->z - originalRotation->z) / timing) * timeSinceLastKeyFrame3DList);
				}
				if (keyFrame->isUsingScaling()) {
					glm::vec3* finalScaling = keyFrame->getScaling();
					glm::vec3 newScaling(originalScaling->x + (((float)finalScaling->x - originalScaling->x) / timing) * timeSinceLastKeyFrame3DList,
						originalScaling->y + (((float)finalScaling->y - originalScaling->y) / timing) * timeSinceLastKeyFrame3DList,
						originalScaling->z + (((float)finalScaling->z - originalScaling->z) / timing) * timeSinceLastKeyFrame3DList);
					object->setScaling(newScaling);
				}

				// This finds the matrix for the point rotation.
				glm::mat4 pointRotationMatrix = glm::mat4(1);
				glm::vec3* pointRotationRotation = keyFrame->getPointRotation()->getRotation();
				if (keyFrame->isUsingPointRotation()) {
					pointRotationMatrix *= glm::eulerAngleYXZ(pointRotationRotation->y / timing * timeSinceLastKeyFrame3DList,
						pointRotationRotation->x / timing * timeSinceLastKeyFrame3DList, pointRotationRotation->z / timing * timeSinceLastKeyFrame3DList);
				}

				// This finds the final position values and applies them.
				if (keyFrame->isUsingTranslation() && keyFrame->isUsingPointRotation()) {
					object->setPosition(*originalPosition + newPosition + glm::vec3(pointRotationMatrix
						* glm::vec4(*keyFrame->getPointRotation()->getPoint() * glm::vec3(-1), 0)) + *keyFrame->getPointRotation()->getPoint());
				} else if (keyFrame->isUsingTranslation()) {
					object->setPosition(*originalPosition + newPosition);
				}

				// This finds the final rotation values and applies them.
				if (keyFrame->isUsingRotation() && keyFrame->isUsingPointRotation()) {
					object->setRotation(*originalRotation + newRotation + *pointRotationRotation * glm::vec3((float) timeSinceLastKeyFrame3DList / timing));
				} else if (keyFrame->isUsingRotation()) {
					object->setRotation(*originalRotation + newRotation);
				} else if (keyFrame->isUsingPointRotation()) {
					object->setRotation(*pointRotationRotation * glm::vec3((float) timeSinceLastKeyFrame3DList / timing));
				}
			}
		}
	}

	if (keyFrame2DLists.size() != 0) {
		// This keeps track of timing.
		timeSinceLastKeyFrame2DList += (unsigned int) timeManager->getTimeBetweenFrames();
		unsigned int timeOfFrame0 = keyFrame2DLists.at(0).getTimeAfterPreviousKeyFrame();

		KeyFrame2DList* keyFrameList = &keyFrame2DLists.at(0);
		std::vector<KeyFrame2D>* keyFrameVector = keyFrameList->getKeyFrames();

		// This erases the current key frame if it has already passed.
		if (keyFrameList->getTimeAfterPreviousKeyFrame() <= timeSinceLastKeyFrame2DList) {
			// This sets the transformations of all of the objects to their final transformations. It also runs their
			// action.
			for (KeyFrame2D keyFrame : *keyFrameVector) {
				std::shared_ptr<Transformable2D> object = keyFrame.getObject();

				if (keyFrame.getEndingAction() != nullptr) {
					keyFrame.getEndingAction()();
				}

				if (object != nullptr) {
					// This finds the final scaling of the model.
					object->setTint(keyFrame.getTint());
				}
			}
			keyFrame2DLists.erase(keyFrame2DLists.begin());
			timeSinceLastKeyFrame2DList = 0;
			if (keyFrame2DLists.size() == 0) {
				return;
			}

			// This sets up the next key frame list.
			keyFrameList = &keyFrame2DLists.at(0);
			keyFrameVector = keyFrameList->getKeyFrames();
			keyFrameList->storeOriginalTransformations();
		}

		// This updates objects using the current key frame list.
		for (unsigned int whichFrame = 0; whichFrame < keyFrameVector->size(); whichFrame++) {
			KeyFrame2D* keyFrame = &keyFrameVector->at(whichFrame);
			std::shared_ptr<Transformable2D> object = keyFrame->getObject();

			if (object != nullptr) {
				// This figures out a bunch of transformational values and finds the final scaling value to use.
				unsigned int timing = keyFrameList->getTimeAfterPreviousKeyFrame() + 1;
				ColourRGBA* originalTint = &keyFrameList->getOriginalTints()->at(whichFrame);
				ColourRGBA* finalTint = keyFrame->getTint();
				ColourRGBA newTint((float) ((finalTint->getR() - originalTint->getR()) / timing) * timeSinceLastKeyFrame2DList + originalTint->getR(),
					(float) ((finalTint->getG() - originalTint->getG()) / timing) * timeSinceLastKeyFrame2DList + originalTint->getG(),
					(float) ((finalTint->getB() - originalTint->getB()) / timing) * timeSinceLastKeyFrame2DList + originalTint->getB(),
					(float) ((finalTint->getA() - originalTint->getA()) / timing) * timeSinceLastKeyFrame2DList + originalTint->getA());
				object->setTint(&newTint);
			}
		}
	}
}

void Animator::setTimeManager(TimeManager* timeManager) {
	this->timeManager = timeManager;
}

void Animator::addKeyFrame3DList(KeyFrame3DList* keyFrameList) {
	if (keyFrame3DLists.size() == 0) {
		keyFrameList->storeOriginalTransformations();
	}
	keyFrame3DLists.push_back(*keyFrameList);
}

std::vector<KeyFrame3DList>* Animator::get3DKeyFrames() {
	return &keyFrame3DLists;
}

void Animator::addKeyFrame2DList(KeyFrame2DList* keyFrameList) {
	if (keyFrame2DLists.size() == 0) {
		keyFrameList->storeOriginalTransformations();
	}
	keyFrame2DLists.push_back(*keyFrameList);
}

std::vector<KeyFrame2DList>* Animator::get2DKeyFrames() {
	return &keyFrame2DLists;
}

int Animator::delete3DListsByTag(std::string tag) {
	int counter = 0;
	for (unsigned int i = 0; i < keyFrame3DLists.size(); i++) {
		if (keyFrame3DLists[i].getTag() == tag) {
			counter++;
			if (i == 0) {
				keyFrame3DLists[i].setTimeAfterPreviousKeyFrameInMillis(0);
			} else {
				keyFrame3DLists.erase(keyFrame3DLists.begin() + i);
			}
		}
	}
	return counter;
}

int Animator::delete2DListsByTag(std::string tag) {
	int counter = 0;
	for (unsigned int i = 0; i < keyFrame2DLists.size(); i++) {
		if (keyFrame2DLists[i].getTag() == tag) {
			counter++;
			if (i == 0) {
				keyFrame2DLists[i].setTimeAfterPreviousKeyFrameInMillis(0);
			} else {
				keyFrame2DLists.erase(keyFrame2DLists.begin() + i);
			}
		}
	}
	return counter;
}

int Animator::deleteListsByTag(std::string tag) {
	int counter = 0;
	counter += delete3DListsByTag(tag);
	counter += delete2DListsByTag(tag);
	return counter;
}
