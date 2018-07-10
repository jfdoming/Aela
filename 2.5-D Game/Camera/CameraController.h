/*
* Class: Camera Controller
* Author: Robert Ciborowski
* Date: 09/07/2017
* Description: A class used to control the camera.
*/

#pragma once
#include "../Game Object Provider/GameObjectProvider.h"
#include "../Character/Character.h"

namespace Game {
	class CameraController {
		public:
			CameraController();

			void setup();
			void update();

			void setLockCameraToPlayer(bool lockCameraToPlayer);

		private:
			Player* player;
			Character* playerCharacter;
			Camera3D* camera;

			bool lockCameraToPlayer = true;
	};
}