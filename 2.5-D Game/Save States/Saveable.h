/*
* Class: Saveable
* Author: Robert Ciborowski
* Date: 12/08/2018
* Description: A superclass used to make a subclass be able to be saved.
*/

#pragma once

namespace Game {
	class SaveState;
	class Saveable {
		public:
			Saveable();
			virtual ~Saveable();

			// Warning! In each child class's implementation of this function, it
			// is responsible for calling delete on the old version of the data
			// inside the passed-in SaveState.
			virtual void saveDataToSaveState(SaveState* saveState);

			
			virtual void loadDataFromSaveState(SaveState* saveState);
	};
}