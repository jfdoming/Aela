/*
* Name: Project Aela's Control Manager
* Author: Ekkon Games
* Date: November 2016
* Description: A class that manages user controls.
*/

#pragma once
#include <glm/glm.hpp>
#include "Window.h"
#include "Camera3D.h"
#include "TimeManager.h"
#include <time.h>

enum class ControlManagerProperty {
	ALLOW_UPSIDE_DOWN_CAMERA,
	CAMERA_TRANSLATION_SPEED, CAMERA_TRANSLATION_SUPER_SPEED,
	CAMERA_ROTATION_SPEED
};

class ControlManager {
	public:
		ControlManager() {
			windowFocus = true;
			speed = 0.003f;
			superSpeed = 0.006f;
			currentSpeed = 0.0f;
			mouseSpeed = 0.005f;
			allowUpsideDownCamera = true;
		}

		// These are setters and getters of the control manager.
		void setWindow(Window* setWindow);
		void setTimeManager(TimeManager* setTime);
		void setCameraUpsideDownAllowance(bool allowUpsdideDown);
		void setProperty(ControlManagerProperty property, float value);

		// This computes matrices for a Camera3D.
		void computeMatricesWithInputs(Camera3D* camera);

	private:
		TimeManager* timeManager;
		Window* window;
		bool windowFocus;

		// Speed: 0.001f is 1 unit per tick.
		float speed, superSpeed, currentSpeed, mouseSpeed;

		// This keeps track of whether the camera shoudl be allowed to be upside down.
		bool allowUpsideDownCamera;

		const glm::vec3 straightUp = glm::vec3(0, 0.5, 0);
};
