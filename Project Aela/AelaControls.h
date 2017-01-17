#pragma once
#include <glm/glm.hpp>
#include "AelaWindow.h"
#include "Aela3DCamera.h"
#include "AelaTime.h"
#include <time.h>

class AelaControlManager {
	public:
		AelaControlManager() {
			windowFocus = true;
			speed = 0.003f;
			superSpeed = 0.006f;
			currentSpeed = 0.0f;
			mouseSpeed = 0.005f;
		}

		void setWindow(AelaWindow * setWindow);
		void setTimeManager(AelaTimeManager * setTime);
		void computeMatricesWithInputs(Aela3DCamera * camera);

	private:
		AelaTimeManager * timeManager;
		AelaWindow * window;
		bool windowFocus;

		// 0.001f is 1 unit per tick.
		float speed, superSpeed, currentSpeed, mouseSpeed;

		const glm::vec3 straightUp = glm::vec3(0, 0.5, 0);
};