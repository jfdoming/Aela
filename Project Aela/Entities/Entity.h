/*
* Class: Entity
* Author: Robert Ciborowski
* Date: 30/07/2017
* Description: A class used to represent an Entity in any dimension.
*/

#pragma once

namespace Aela {
	enum class EntityType {
		GENERIC, MODEL, LIGHT, BILLBOARD, SKYBOX
	};

	class Entity {
		public:
			Entity() {

			}

			~Entity() {

			}

			virtual EntityType getEntityType();
		private:
	};
}