#include <glm/glm.hpp>
#include "AelaWindow.h"
#include <time.h>

#ifndef CONTROLS_HPP
#define CONTROLS_HPP

void computeMatricesFromInputs(AelaWindow * window);
glm::mat4 getViewMatrix();
glm::mat4 getProjectionMatrix();
glm::vec3 getPositionOfCamera();
clock_t getTime();
float getTimeInterval();

#endif
