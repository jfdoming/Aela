/*
* Class: Control Manager
* Author: Ekkon Games
* Date: November 2016
* Description: A class that manages user controls.
*/

#pragma once
#include <glm/glm.hpp>
#include "../Window/Window.h"
#include "../3D/3D Camera/Camera3D.h"
#include "../Time Manager/TimeManager.h"
#include <time.h>

// These enums are used to set properties for the control manager.
enum class ControlManagerProperty {
	ALLOW_UPSIDE_DOWN_CAMERA,
	CAMERA_TRANSLATION_SPEED, CAMERA_TRANSLATION_SUPER_SPEED,
	CAMERA_ROTATION_SPEED
};

// This is the Control Manager class.
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

		// This is a test function for LUA.
		void test();

	private:
		// These are pointers to other Aela classes.
		TimeManager* timeManager;
		Window* window;

		// This stores the window's state.
		bool windowFocus;

		// Speed: 0.001f is 1 unit per tick.
		float speed, superSpeed, currentSpeed, mouseSpeed;

		// This keeps track of whether the camera shoudl be allowed to be upside down.
		bool allowUpsideDownCamera;

		// This is used when computing controls.
		const glm::vec3 straightUp = glm::vec3(0, 0.5, 0);
};
