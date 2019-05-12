#include "KeyMovement.h"

Game::KeyMovement::KeyMovement() {
}

void Game::KeyMovement::setDuration(unsigned long long duration) {
	this->duration = duration;
}

unsigned long long Game::KeyMovement::getDuration() {
	return duration;
}

void Game::KeyMovement::setDirection(NoteDirection direction) {
	this->direction = direction;
}

Game::NoteDirection Game::KeyMovement::getDirection() {
	return direction;
}

void Game::KeyMovement::setX(float x) {
	this->x = x;
}

float Game::KeyMovement::getX() {
	return x;
}

void Game::KeyMovement::setY(float y) {
	this->y = y;
}

float Game::KeyMovement::getY() {
	return y;
}

void Game::KeyMovement::setCurvature(float curvature) {
	this->curvature = curvature;
}

float Game::KeyMovement::getCurvature() {
	return curvature;
}
