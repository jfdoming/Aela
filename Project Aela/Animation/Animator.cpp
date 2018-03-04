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
// called once anyways.
void Animator::update() {
	long long timePassed = timeManager->getTimeBetweenFramesInNanos();

	size_t track3DSize = tracks3D.size();
	for (size_t which3DTrack = 0; which3DTrack < track3DSize; which3DTrack++) {
		AnimationTrack3D& track = tracks3D[which3DTrack];
		track.updatePositionInTrack(timePassed);

		auto& firstFramePair = track.getKeyFrames()->at(0);

		KeyFrame3D& keyFrame = firstFramePair.second;
		Transformable3D* object = keyFrame.getObject();

		if (!keyFrame.hasBeenStarted()) {
			keyFrame.start();
		}

		long long endTime = firstFramePair.first;
		long long timeSinceKeyFrameStart = track.getPositionInTrack() + 1;

		std::cout << firstFramePair.first << " vs " << track.getPositionInTrack() << "\n";

		// Check if this keyframe should have ended by now. If it has, perform the actions necessary.
		if (firstFramePair.first <= track.getPositionInTrack()) {
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
					glm::vec3 finalTranslation = *keyFrame.getPosition();
					glm::vec3* originalPosition = keyFrame.getOriginalPosition();
					object->setPosition(*keyFrame.getPosition() + glm::vec3(pointRotationMatrix
						* glm::vec4(*keyFrame.getPointRotation()->getPoint() * glm::vec3(-1), 0)) + *keyFrame.getPointRotation()->getPoint());
					object->timePassedAfterAnimationEnd = track.getPositionInTrack() - firstFramePair.first;
				}

				if (keyFrame.isUsingPointRotation() && keyFrame.isUsingRotation()) {
					object->setRotation(*keyFrame.getRotation() + *pointRotationValue);
				} else if (keyFrame.isUsingRotation()) {
					object->setRotation(*keyFrame.getRotation());
				} else if (keyFrame.isUsingPointRotation()) {
					object->setRotation(*pointRotationValue);
				}
			}

			if (keyFrame.getEndingAction() != nullptr) {
				keyFrame.getEndingAction()();
			}

			// This gets rid of the keyframe.
			track.getKeyFrames()->erase(track.getKeyFrames()->begin());

			if (track.getKeyFrames()->size() == 0) {
				tracks3D.erase(tracks3D.begin() + which3DTrack);
				which3DTrack--;
				track3DSize--;
				continue;
			}

			// long long difference = track.getPositionInTrack() - firstFramePair.first;
			track.resetPosition();
			// track.updatePositionInTrack(difference);
			continue;
		}

		// This occurs if the keyframe has not ended yet, and must update the object its linked to accordingly.
		if (object != nullptr) {
			// This figures out a bunch of transformational values and finds the final scaling value to use.
			glm::vec3* originalPosition = keyFrame.getOriginalPosition();
			glm::vec3* originalRotation = keyFrame.getOriginalRotation();
			glm::vec3* originalScaling = keyFrame.getOriginalScaling();
			glm::vec3 newPosition, newRotation;
			if (keyFrame.isUsingTranslation()) {
				glm::vec3* finalTranslation = keyFrame.getPosition();
				newPosition = glm::vec3((((float) finalTranslation->x - originalPosition->x) / endTime) * timeSinceKeyFrameStart,
					(((float) finalTranslation->y - originalPosition->y) / endTime) * timeSinceKeyFrameStart,
					(((float) finalTranslation->z - originalPosition->z) / endTime) * timeSinceKeyFrameStart);
			}
			if (keyFrame.isUsingRotation()) {
				glm::vec3* finalRotation = keyFrame.getRotation();
				newRotation = glm::vec3((((float) finalRotation->x - originalRotation->x) / endTime) * timeSinceKeyFrameStart,
					(((float) finalRotation->y - originalRotation->y) / endTime) * timeSinceKeyFrameStart,
					(((float) finalRotation->z - originalRotation->z) / endTime) * timeSinceKeyFrameStart);
			}
			if (keyFrame.isUsingScaling()) {
				glm::vec3* finalScaling = keyFrame.getScaling();
				glm::vec3 newScaling(originalScaling->x + (((float) finalScaling->x - originalScaling->x) / endTime) * timeSinceKeyFrameStart,
					originalScaling->y + (((float) finalScaling->y - originalScaling->y) / endTime) * timeSinceKeyFrameStart,
					originalScaling->z + (((float) finalScaling->z - originalScaling->z) / endTime) * timeSinceKeyFrameStart);
				object->setScaling(newScaling);
			}

			// This finds the matrix for the point rotation.
			glm::mat4 pointRotationMatrix = glm::mat4(1);
			glm::vec3* pointRotationRotation = keyFrame.getPointRotation()->getRotation();
			if (keyFrame.isUsingPointRotation()) {
				pointRotationMatrix *= glm::eulerAngleYXZ(pointRotationRotation->y / endTime * timeSinceKeyFrameStart,
					pointRotationRotation->x / endTime * timeSinceKeyFrameStart, pointRotationRotation->z / endTime * timeSinceKeyFrameStart);
			}

			// This finds the final position values and applies them.
			if (keyFrame.isUsingTranslation() && keyFrame.isUsingPointRotation()) {
				object->setPosition(*originalPosition + newPosition + glm::vec3(pointRotationMatrix
					* glm::vec4(*keyFrame.getPointRotation()->getPoint() * glm::vec3(-1), 0)) + *keyFrame.getPointRotation()->getPoint());
				glm::vec3 pos = *originalPosition + newPosition;
				std::cout << track.getTag() << " pos: " << pos.x << ", " << pos.y << ", " << pos.z << "\n";
			} else if (keyFrame.isUsingTranslation()) {
				object->setPosition(*originalPosition + newPosition);
				glm::vec3 pos = *originalPosition + newPosition;
				std::cout << track.getTag() << " pos: " << pos.x << ", " << pos.y << ", " << pos.z << "\n";
			}

			// This finds the final rotation values and applies them.
			if (keyFrame.isUsingRotation() && keyFrame.isUsingPointRotation()) {
				object->setRotation(*originalRotation + newRotation + *pointRotationRotation * glm::vec3((float) timeSinceKeyFrameStart / endTime));
			} else if (keyFrame.isUsingRotation()) {
				object->setRotation(*originalRotation + newRotation);
			} else if (keyFrame.isUsingPointRotation()) {
				object->setRotation(*pointRotationRotation * glm::vec3((float) timeSinceKeyFrameStart / endTime));
			}
		}
		// which3DTrack++;
	}

	// This regains the time for accuracy.
	timePassed = timeManager->getTimeBetweenFramesInNanos();

	size_t track2DSize = tracks2D.size();
	for (size_t which2DTrack = 0; which2DTrack < track2DSize; which2DTrack++) {
		AnimationTrack2D& track = tracks2D[which2DTrack];
		track.updatePositionInTrack(timePassed);

		if (track.getKeyFrames()->size() == 0) {
			continue;
		}

		auto& firstFramePair = track.getKeyFrames()->at(0);

		KeyFrame2D& keyFrame = firstFramePair.second;
		std::shared_ptr<Transformable2D> object = keyFrame.getObject();

		if (!keyFrame.hasBeenStarted()) {
			keyFrame.start();
		}

		// Check if this keyframe should have ended by now. If it has, perform the actions necessary.
		if (firstFramePair.first <= track.getPositionInTrack()) {
			if (object != nullptr) {
				std::shared_ptr<Transformable2D> object = keyFrame.getObject();

				if (object != nullptr) {
					object->setTint(keyFrame.getTint());
					object->setDimensions(keyFrame.getDimensions());
				}
			}

			if (keyFrame.getEndingAction() != nullptr) {
				keyFrame.getEndingAction()();
			}

			track.getKeyFrames()->erase(track.getKeyFrames()->begin());

			if (track.getKeyFrames()->size() == 0) {
				tracks2D.erase(tracks2D.begin() + which2DTrack);
				which2DTrack--;
				track2DSize--;
				continue;
			}

			// Here is where I stopped. This should happen but it breaks since track.getPositionInTrack() can be a huge number.
			// long long difference = track.getPositionInTrack() - firstFramePair.first;
			// std::cout << difference << " " << track.getPositionInTrack() << " " << firstFramePair.first << " is the diff.\n";
			track.resetPosition();
			// track.updatePositionInTrack(difference);
			continue;
		}

		// This occurs if the keyframe has not ended yet, and must update the object its linked to accordingly.
		if (object != nullptr) {
			long long endTime = firstFramePair.first;
			long long timeSinceKeyFrameStart = track.getPositionInTrack() + 1;

			// This figures out a bunch of transformational values and finds the final value to use.
			ColourRGBA* originalTint = keyFrame.getOriginalTint();
			ColourRGBA* finalTint = keyFrame.getTint();
			ColourRGBA newTint((float) ((finalTint->getR() - originalTint->getR()) / endTime) * timeSinceKeyFrameStart + originalTint->getR(),
				(float) ((finalTint->getG() - originalTint->getG()) / endTime) * timeSinceKeyFrameStart + originalTint->getG(),
				(float) ((finalTint->getB() - originalTint->getB()) / endTime) * timeSinceKeyFrameStart + originalTint->getB(),
				(float) ((finalTint->getA() - originalTint->getA()) / endTime) * timeSinceKeyFrameStart + originalTint->getA());
			object->setTint(&newTint);

			Rect<int>* originalDimensions = keyFrame.getOriginalDimensions();
			Rect<int>* finalDimensions = keyFrame.getDimensions();
			Rect<int> newDimensions((int) ((float) (finalDimensions->getX() - originalDimensions->getX()) / endTime * timeSinceKeyFrameStart + originalDimensions->getX()),
				(int) ((float) (finalDimensions->getY() - originalDimensions->getY()) / endTime * timeSinceKeyFrameStart + originalDimensions->getY()),
				(int) ((float) (finalDimensions->getWidth() - originalDimensions->getWidth()) / endTime * timeSinceKeyFrameStart + originalDimensions->getWidth()),
				(int) ((float) (finalDimensions->getHeight() - originalDimensions->getHeight()) / endTime * timeSinceKeyFrameStart + originalDimensions->getHeight()));
			object->setDimensions(&newDimensions);
		}
		// which2DTrack++;
	}
}

void Animator::setTimeManager(TimeManager* timeManager) {
	this->timeManager = timeManager;
}

void Animator::addAnimationTrack3D(AnimationTrack3D* track) {
	if (track->getKeyFrames()->size() != 0) {
		tracks3D.push_back(*track);
	}
}

AnimationTrack3D* Animator::get3DTrack(std::string name) {
	for (auto& track : tracks3D) {
		if (track.getTag() == name) {
			return &track;
		}
	}
	return nullptr;
}

void Animator::addAnimationTrack2D(AnimationTrack2D* track) {
	if (track->getKeyFrames()->size() != 0) {
		tracks2D.push_back(*track);
	}
}

AnimationTrack2D* Animator::get2DTrack(std::string name) {
	for (auto& track : tracks2D) {
		if (track.getTag() == name) {
			return &track;
		}
	}
	return nullptr;
}

int Animator::delete3DTrackByTag(std::string tag) {
	int counter = 0;
	for (unsigned int i = 0; i < tracks3D.size(); i++) {
		if (tracks3D[i].getTag() == tag) {
			counter++;
			if (i != 0) {
				tracks3D.erase(tracks3D.begin() + i);
			}
		}
	}
	return counter;
}

int Animator::delete2DListsByTag(std::string tag) {
	int counter = 0;
	for (unsigned int i = 0; i < tracks2D.size(); i++) {
		if (tracks2D[i].getTag() == tag) {
			counter++;
			if (i != 0) {
				tracks2D.erase(tracks2D.begin() + i);
			}
		}
	}
	return counter;
}

int Animator::deleteListsByTag(std::string tag) {
	int counter = 0;
	counter += delete3DTrackByTag(tag);
	counter += delete2DListsByTag(tag);
	return counter;
}

bool Animator::trackWithTagExists(std::string tag) {
	for (auto& track : tracks3D) {
		if (track.getTag() == tag) {
			return true;
		}
	}

	for (auto& track : tracks2D) {
		if (track.getTag() == tag) {
			return true;
		}
	}
	return false;
}

long long Animator::tracksWithTag(std::string tag) {
	long long counter = 0;

	for (auto& track : tracks3D) {
		if (track.getTag() == tag) {
			counter++;
		}
	}

	for (auto& track : tracks2D) {
		if (track.getTag() == tag) {
			counter++;
		}
	}

	return counter;
}
