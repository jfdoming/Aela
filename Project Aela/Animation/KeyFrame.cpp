/*
* Class: Project Aela's Key Frame
* Author: Robert Ciborowski
* Date: 27/08/2017
* Description: A class used for keyframing with the animator.
*/

#include "KeyFrame.h"

void KeyFrame::setEndingAction(std::function<void()> endingAction) {
	this->endingAction = endingAction;
}

std::function<void()> KeyFrame::getEndingAction() {
	return endingAction;
}