#include "AelaControls.h"

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include "time.h"
#include "AelaError.h"
#include <iostream>

//// Initial horizontal angle : toward -Z
//float horizontalAngle = 3.14f;
//// Initial vertical angle : none
//float verticalAngle = 0.0f;
//// Initial Field of View
//float initialFoV = 45.0f;

void AelaControlManager::setWindow(AelaWindow * setWindow) {
	window = setWindow;
}

void AelaControlManager::setTimeManager(AelaTimeManager * setTime) {
	timeManager = setTime;
}

void AelaControlManager::computeMatricesWithInputs(Aela3DCamera * camera) {
	// This will only run if the window is focused.
	if (window->isFocused()) {
		windowFocus = true;
		float deltaTime = timeManager->getTimeBetweenFrames();

		// This is temporarily hard-coded and enables going fast.
		if (window->keyPressed(225)) {
			currentSpeed = superSpeed;
		} else {
			currentSpeed = speed;
		}
		// This gets the cursor's position.
		int xpos, ypos;
		window->getCursorPositionInWindow(&xpos, &ypos);

		// This teleports the cursor back to the middle of the window.
		int width, height;
		window->getWindowDimensions(&width, &height);
		window->setCursorPositionInWindow(width / 2, height / 2);

		// This gets the horizontal and vertical angles.
		float horizontalAngle = camera->getProperty(Aela3DProperty::X_ROTATION);
		float verticalAngle = camera->getProperty(Aela3DProperty::Y_ROTATION);

		// This computes the new horizontal and vertical angles.
		horizontalAngle += mouseSpeed * float(width / 2 - xpos);
		verticalAngle += mouseSpeed * float(height / 2 - ypos);

		camera->setProperty(Aela3DProperty::X_ROTATION, horizontalAngle);
		camera->setProperty(Aela3DProperty::Y_ROTATION, verticalAngle);

		// This converts the coordinates from rotational to cartesian-planal.
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
		if (window->keyPressed(26)) {
			position += direction * deltaTime * currentSpeed;
		}
		// This occurs when 's' is pressed.
		if (window->keyPressed(22)) {
			position -= direction * deltaTime * currentSpeed;
		}
		// This occurs when 'd' is pressed.
		if (window->keyPressed(7)) {
			position += right * deltaTime * currentSpeed;
		}
		// This occurs when 'a' is pressed.
		if (window->keyPressed(4)) {
			position -= right * deltaTime * currentSpeed;
		}

		// This occurs when space is pressed.
		if (window->keyPressed(44)) {
			position += straightUp * deltaTime * currentSpeed;
		}

		// This occurs when left shift is pressed.
		if (window->keyPressed(224)) {
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