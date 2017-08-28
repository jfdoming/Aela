#include "KeyFrame2DList.h"

std::vector<ColourRGBA>* KeyFrame2DList::getOriginalTints() {
	return &originalTints;
}

void KeyFrame2DList::storeOriginalTransformations() {
	for (KeyFrame2D keyFrame : keyFrames) {
		if (keyFrame.getObject() != nullptr) {
			originalTints.push_back(*keyFrame.getObject()->getTint());
		} else {
			// You still need to put something in the original property vectors.
			originalTints.push_back(ColourRGBA(1));
		}
	}
}

