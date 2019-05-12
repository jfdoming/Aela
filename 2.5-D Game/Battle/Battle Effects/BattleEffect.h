/*
* Class: Battle Effect
* Author: Robert Ciborowski
* Date: 22/02/2019
* Description: A base class for any effect that modifies the look and feel of battles.
*/

#pragma once
#include <memory>
#include "../../../Project Aela/Menus/ImageComponent.h"

namespace Game {
	class BattleNote;

	class BattleEffect {
		public:
			BattleEffect();

			std::shared_ptr<ImageComponent> addNewNote(BattleNote* note, unsigned long long timeOfEntry, unsigned long long battleStartTime);
			
	};
}
