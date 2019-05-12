/*
* Class: Battle Note
* Author: Robert Ciborowski
* Date: 07/01/2019
* Description: A class used to represent a note of a battle.
*/

#pragma once
#include <string>

namespace Game {
	enum class NoteDirection {
		RIGHT, UP, LEFT, DOWN, RANDOM, RANDOM_LEFT_RIGHT, RANDOM_UP_DOWN
	};

	class BattleNote {
		public:
			BattleNote();
			BattleNote(NoteDirection direction, unsigned long long timeTillDestination, unsigned long long tailLength);

			void setDirection(NoteDirection direction);
			NoteDirection getDirection();
			unsigned long long getTiming();
			unsigned long long getTailLength();
			bool isPressed();
			void press();

		private:
			NoteDirection direction = NoteDirection::RANDOM;

			// This represents the time between when the note enters and when it passes under
			// the player's keyboard key.
			unsigned long long timeTillDestination;

			// This is the length of the note's "tail", in milliseconds.
			unsigned long long tailLength;

			bool pressed = false;
	};
}
