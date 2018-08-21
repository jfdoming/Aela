/*
* Class: Saveable
* Author: Robert Ciborowski
* Date: 12/08/2018
* Description: A superclass used to make a subclass be able to be saved.
*/

#pragma once
#include "SaveState.h"

namespace Game {
	class Saveable {
		public:
			Saveable();

			virtual void saveDataToSaveState(SaveState* saveState);
			virtual void loadDataFromSaveState(SaveState* saveState);
	};
}