/*
* Name: Project Aela's Control Manager
* Author: Ekkon Games
* Date: November 2016
* Description: A class that manages user controls.
*/

#include "ControlManager.h"

// This includes GLM.
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include "../Error Handler/ErrorHandler.h"
#include <iostream>

// This disables the following compilation warning:
// 'int': forcing value to bool 'true' or 'false' (performance warning)
#pragma warning(disable : 4800)

void ControlManager::setWindow(Window* setWindow) {
	window = setWindow;
}

void ControlManager::setTimeManager(TimeManager* setTime) {
	timeManager = setTime;
}

void ControlManager::updateEvents(SDL_Event* _event) {
	event = _event;
}

bool ControlManager::keyPressed(int keycode) {
	return event->key.keysym.scancode == keycode;
}

void ControlManager::computeMatricesWithInputs(Camera3D* camera) {
	// This will only run if the window is focused.
	if (window->isFocused()) {
		windowFocus = true;
		float deltaTime = timeManager->getTimeBetweenFrames();

		// This is temporarily hard-coded and enables going fast.
		if (keyPressed(225)) {
			currentSpeed = superSpeed;
		} else {
			currentSpeed = speed;
		}

		// This gets the cursor's position.
		int xpos, ypos;
		window->getCursorPositionInWindow(&xpos, &ypos);

		// This moves the cursor back to the middle of the window.
		int width, height;
		window->getWindowDimensions(&width, &height);
		window->setCursorPositionInWindow(width / 2, height / 2);

		// This gets the horizontal and vertical angles.
		float horizontalAngle = camera->getProperty(Model3DProperty::X_ROTATION);
		float verticalAngle = camera->getProperty(Model3DProperty::Y_ROTATION);

		// This computes the new horizontal angle.
		horizontalAngle += mouseSpeed * float(width / 2 - xpos);

		// This adjusts the horizontal angle so that it stays between 0 and PI * 2.
		if (horizontalAngle >= glm::pi<float>() * 2) {
			horizontalAngle -= glm::pi<float>() * 2;
		}
		if (horizontalAngle <= 0) {
			horizontalAngle += glm::pi<float>() * 2;
		}

		// This computes the new vertical angle.
		float verticalModifier = mouseSpeed * float(height / 2 - ypos);

		// This checks to see if the user is trying to make the camera go upside down by moving the camera up
		// too far (vertical angle of PI/2 in radians). This also allows the camera to go upside down as long as
		// allowUpsideDownCamera is true.
		if ((!allowUpsideDownCamera && verticalModifier > 0 && verticalAngle + verticalModifier <= glm::pi<float>() / 2) || allowUpsideDownCamera) {
			verticalAngle += mouseSpeed * float(height / 2 - ypos);
		} else if (!allowUpsideDownCamera && verticalModifier > 0) {
			verticalAngle = glm::pi<float>() / 2;
		}

		// This checks to see if the user is trying to make the camera go upside down by moving the camera down
		// too far (vertical angle of -PI/2 in radians). This also allows the camera to go upside down as long as
		// allowUpsideDownCamera is true.
		if ((!allowUpsideDownCamera && verticalModifier < 0 && verticalAngle + mouseSpeed * float(height / 2 - ypos) >= glm::pi<float>() / -2) || allowUpsideDownCamera) {
			verticalAngle += mouseSpeed * float(height / 2 - ypos);
		} else if (!allowUpsideDownCamera && verticalModifier < 0) {
			verticalAngle = glm::pi<float>() / -2;
		}

		camera->setProperty(Model3DProperty::X_ROTATION, horizontalAngle);
		camera->setProperty(Model3DProperty::Y_ROTATION, verticalAngle);

		// This converts the coordinates from rotational to cartesian-planar.
		glm::vec3 direction(
			cos(verticalAngle) * sin(horizontalAngle),
			sin(verticalAngle),
			cos(verticalAngle) * cos(horizontalAngle)
		);

		// These are vectors for the cartesian-plane system.
		glm::vec3 right = glm::vec3(
			sin(horizontalAngle - 3.14f / 2.0f),
			0,
			cos(horizontalAngle - 3.14f / 2.0f)
		);
		glm::vec3 up = glm::cross(right, direction);

		// This is a position vector.
		glm::vec3 position = camera->getPosition();

		// This occurs when 'w' is pressed.
		if (keyPressed(26)) {
			position += direction * deltaTime * currentSpeed;
		}
		// This occurs when 's' is pressed.
		if (keyPressed(22)) {
			position -= direction * deltaTime * currentSpeed;
		}
		// This occurs when 'd' is pressed.
		if (keyPressed(7)) {
			position += right * deltaTime * currentSpeed;
		}
		// This occurs when 'a' is pressed.
		if (keyPressed(4)) {
			position -= right * deltaTime * currentSpeed;
		}

		// This occurs when space is pressed.
		if (keyPressed(44)) {
			position += straightUp * deltaTime * currentSpeed;
		}

		// This occurs when left shift is pressed.
		if (keyPressed(224)) {
			position -= straightUp * deltaTime * currentSpeed;
		}

		// This sets all of the camera's position and view related properties.
		camera->setPosition(position);
		glm::mat4 projectionMatrix = glm::perspective(camera->getFieldOfView(), 4.0f / 3.0f, 0.1f, 100.0f);
		glm::mat4 viewMatrix = glm::lookAt(position, position + direction, up);
		camera->setProjectionMatrix(projectionMatrix);
		camera->setViewMatrix(viewMatrix);

	} else {
		windowFocus = false;
	}
}

void ControlManager::setCameraUpsideDownAllowance(bool allowUpsideDown) {
	allowUpsideDownCamera = allowUpsideDown;
}

void ControlManager::setProperty(ControlManagerProperty property, float value) {
	switch (property) {
		case ControlManagerProperty::ALLOW_UPSIDE_DOWN_CAMERA:
			// Note: there is apparently a performance warning with forcing a float value to a boolean.
			allowUpsideDownCamera = (bool) value;
			break;
		case ControlManagerProperty::CAMERA_ROTATION_SPEED:
			mouseSpeed = value;
			break;
		case ControlManagerProperty::CAMERA_TRANSLATION_SPEED:
			speed = value;
			break;
		case ControlManagerProperty::CAMERA_TRANSLATION_SUPER_SPEED:
			superSpeed = value;
			break;
	}
}

void ControlManager::test() {
	// Add stuff here.
}