#include "Animator3D.h"

void Animator3D::update() {
	if (keyFrames.size() != 0) {
		timeSinceLastKeyFrame += (unsigned int) timeManager->getTimeBetweenFrames();
		std::cout << keyFrames.size() << " is the size\n";
		unsigned int timeOfFrame0 = keyFrames.at(0).getTimeAfterPreviousKeyFrame();
		std::cout << "A!\n";

		for (int whichFrame = 0; keyFrames.at(whichFrame).getTimeAfterPreviousKeyFrame() <= timeOfFrame0; whichFrame++) {
			std::cout << "B!\n";
			KeyFrame3D* keyFrame = &keyFrames.at(whichFrame);
			Object3D* object = keyFrame->getObject();
			
			if (keyFrames.at(whichFrame).getTimeAfterPreviousKeyFrame() <= timeSinceLastKeyFrame) {
				std::cout << "C!\n";
				keyFrames.erase(keyFrames.begin());
				std::cout << keyFrames.size() << " is the NEW size\n";
				timeSinceLastKeyFrame = 0;
				if (keyFrames.size() == 0) {
					std::cout << "Done!\n";
					break;
				} else {
					std::cout << "New frame!\n";
					originalPosition = *keyFrames.at(whichFrame).getObject()->getPosition();
					originalRotation = *keyFrames.at(whichFrame).getObject()->getRotation();
					whichFrame--;
					std::cout << "New frame 2!\n";
				}
			} else {
				std::cout << "D!\n";
				unsigned int timing = keyFrames.at(whichFrame).getTimeAfterPreviousKeyFrame() + 1;
				if (keyFrame->getTranslation() != NULL) {
					std::cout << "E!\n";
					glm::vec3* finalTranslation = keyFrame->getTranslation();
					glm::vec3 newPosition(originalPosition.x + ((finalTranslation->x - originalPosition.x) / timing) * timeSinceLastKeyFrame,
						originalPosition.y + ((finalTranslation->y - originalPosition.y) / timing) * timeSinceLastKeyFrame,
						originalPosition.z + ((finalTranslation->z - originalPosition.z) / timing) * timeSinceLastKeyFrame);
					object->setPosition(newPosition);
				}
				if (keyFrame->getRotation() != NULL) {
					std::cout << "F!\n";
					glm::vec3* finalRotation = keyFrame->getRotation();
					glm::vec3 newRotation(originalRotation.x + ((finalRotation->x - originalRotation.x) / timing) * timeSinceLastKeyFrame,
						originalRotation.y + ((finalRotation->y - originalRotation.y) / timing) * timeSinceLastKeyFrame,
						originalRotation.z + ((finalRotation->z - originalRotation.z) / timing) * timeSinceLastKeyFrame);
					object->setRotation(newRotation);
				}
				std::cout << whichFrame << " " << keyFrames.size() << " is the frame info!\n";
			}
		}

		/*while (keyFrames.at(0).getTimeAfterPreviousKeyFrame() <= timeSinceLastKeyFrame) {
			KeyFrame3D* keyFrame = &keyFrames.at(0);
			Object3D* object = keyFrame->getObject();
			if (keyFrame->getTranslation() != NULL) {
				object->setPosition(*keyFrame->getTranslation());
			}
			if (keyFrame->getRotation() != NULL) {
				object->setRotation(*keyFrame->getRotation());
			}
			keyFrames.erase(keyFrames.begin());
			std::cout << "lol\n";
			timeSinceLastKeyFrame = 0;
			if (keyFrames.size() == 0) {
				break;
			}
		}*/
	}
	std::cout << "H!\n";
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
		originalPosition = *keyFrame->getObject()->getPosition();
		originalRotation = *keyFrame->getObject()->getRotation();
	}
}

std::vector<KeyFrame3D>* Animator3D::getKeyFrames() {
	return &keyFrames;
}
