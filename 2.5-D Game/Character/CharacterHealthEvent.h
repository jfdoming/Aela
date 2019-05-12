/*
* Class: Player Health Event
* Author: Robert Ciborowski
* Date: 12/01/2019
* Description: A class used to alert about the 
*/

#pragma once
#include "../../Project Aela/Events/Event.h"
#include "Character.h"

namespace Game {
	static constexpr int CHARACTER_HEALTH{11};

	class CharacterHealthEvent : public Event {
		public:
			CharacterHealthEvent(Character* character);
			virtual ~CharacterHealthEvent();

			Character* getCharacter();
			int getHealth();

		private:
			Character* character;
			int health;
	};
}
