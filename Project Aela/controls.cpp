// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include "time.h"

#include "controls.hpp"
#include "AelaError.h"
#include <iostream>

glm::mat4 ViewMatrix;
glm::mat4 ProjectionMatrix;

glm::mat4 getViewMatrix() {
	return ViewMatrix;
}
glm::mat4 getProjectionMatrix() {
	return ProjectionMatrix;
}

// Initial position : on +Z
glm::vec3 position = glm::vec3(0, 0, 5);

glm::vec3 getPositionOfCamera() {
	return position;
}

// Initial horizontal angle : toward -Z
float horizontalAngle = 3.14f;
// Initial vertical angle : none
float verticalAngle = 0.0f;
// Initial Field of View
float initialFoV = 45.0f;

float speed = 0.003f; // 3 units per tick -or- approx. 0.003 units per second
float superSpeed = 0.006f;
float currentSpeed = 0.0f;
float mouseSpeed = 0.005f;
clock_t currentTime = clock(), lastTime;



void computeMatricesFromInputs(AelaWindow * window) {
	if (window->isFocused()) {
		lastTime = currentTime;

		// Compute time difference between current and last frame
		currentTime = clock();
		float deltaTime = float(currentTime - lastTime);

		if (window->keyPressed(225)) {
			currentSpeed = superSpeed;
		} else {
			currentSpeed = speed;
		}

		// Get mouse position
		int xpos, ypos;
		window->getCursorPositionInWindow(&xpos, &ypos);

		// Reset mouse position for next frame
		int width, height;
		window->getWindowDimensions(&width, &height);
		window->setCursorPositionInWindow(width / 2, height / 2);


		// Compute new orientation
		horizontalAngle += mouseSpeed * float(width / 2 - xpos);
		verticalAngle += mouseSpeed * float(height / 2 - ypos);


		// Direction : Spherical coordinates to Cartesian coordinates conversion
		glm::vec3 direction(
			cos(verticalAngle) * sin(horizontalAngle),
			sin(verticalAngle),
			cos(verticalAngle) * cos(horizontalAngle)
		);

		// Right vector
		glm::vec3 right = glm::vec3(
			sin(horizontalAngle - 3.14f / 2.0f),
			0,
			cos(horizontalAngle - 3.14f / 2.0f)
		);

		glm::vec3 straightUp(
			0,
			0.5,
			0
		);

		// Up vector
		glm::vec3 up = glm::cross(right, direction);

		// Move forward
		if (window->keyPressed(26)) {
			position += direction * deltaTime * currentSpeed;
		}
		// Move backward
		if (window->keyPressed(22)) {
			position -= direction * deltaTime * currentSpeed;
		}
		// Strafe right
		if (window->keyPressed(7)) {
			position += right * deltaTime * currentSpeed;
		}
		// Strafe left
		if (window->keyPressed(4)) {
			position -= right * deltaTime * currentSpeed;
		}

		if (window->keyPressed(44)) {
			position += straightUp * deltaTime * currentSpeed;
		}

		if (window->keyPressed(224)) {
			position -= straightUp * deltaTime * currentSpeed;
		}


		float FoV = initialFoV;

		// Projection matrix : 45� Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
		ProjectionMatrix = glm::perspective(FoV, 4.0f / 3.0f, 0.1f, 100.0f);
		// Camera matrix
		ViewMatrix = glm::lookAt(
			position,           // Camera is here
			position + direction, // and looks here : at the same position, plus "direction"
			up                  // Head is up (set to 0,-1,0 to look upside-down)
		);

		// For the next frame, the "last time" will be "now"
		lastTime = currentTime;
	}
}