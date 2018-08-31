#include "Bullet.h"

Game::Bullet::Bullet() = default;

Game::Bullet::Bullet(double speed) : speed(speed) {

}

void Game::Bullet::setSpeed(double speed) {
	this->speed = speed;
}

double Game::Bullet::getSpeed() {
	return speed;
}
