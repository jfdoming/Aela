/*
* Class: Project Aela's 2D Key Frame
* Author: Robert Ciborowski
* Date: 27/08/2017
* Description: A class used by Aela's animator to perform transformations on 2D objects.
*/

#pragma once

#include <memory>

#include "../../Animation/KeyFrame.h"
#include "../Transformable/Transformable2D.h"

class KeyFrame2D : public KeyFrame {
	public:
		KeyFrame2D() : tint(1, 1, 1, 1) {

		}

		KeyFrameType getType();
		void setObject(std::shared_ptr<Transformable2D> object);
		std::shared_ptr<Transformable2D> getObject();
		void setTint(ColourRGBA* tint);
		ColourRGBA* getTint();

	private:
		std::shared_ptr<Transformable2D> object = nullptr;
		ColourRGBA tint;
};