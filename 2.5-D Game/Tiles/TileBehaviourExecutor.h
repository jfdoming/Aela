/*
* Class: TileBehaviourExecutor
* Author: Robert Ciborowski
* Date: 10/07/2017
* Description: A class used to store behaviours of tiles when they placed.
*/

#pragma once
#include "../Game Object Provider/GameObjectProvider.h"
#include "../Location/Location.h"
#include <unordered_map>
#include <functional>

namespace Game {
	class TileBehaviourExecutor {
		public:
			TileBehaviourExecutor();

			void setup();

			void addBehaviour(size_t tileTypeID, std::function<void(const Location&)>* behaviour);
			void runBehaviour(const Location& location, size_t tileTypeID);

		private:
			WorldManager* worldManager;

			std::unordered_map<size_t, std::function<void(const Location&)>> behaviours;
	};

}