/*
* Class: Key Movement
* Author: Robert Ciborowski
* Date: 25/02/2019
* Description: A class used to represent a movement of one of a battle's keyboard key.
*/

#pragma once
#include "BattleNote.h"

namespace Game {
	class KeyMovement {
		public:
			KeyMovement();

			void setDuration(unsigned long long duration);
			unsigned long long getDuration();
			void setDirection(NoteDirection firection);
			NoteDirection getDirection();
			void setX(float x);
			float getX();
			void setY(float y);
			float getY();
			void setCurvature(float curvature);
			float getCurvature();

		private:
			unsigned long long duration = 0;
			NoteDirection direction = NoteDirection::RIGHT;
			float x = 0, y = 0;
			float curvature = 0;
	};
}
