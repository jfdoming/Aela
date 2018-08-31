#include "BulletParticleEmitter.h"
#include "../Utilities/MathConstants.h"

Game::BulletParticleEmitter::BulletParticleEmitter(Clock* time) : ParticleEmitter(time) {
}

Game::BulletParticleEmitter::~BulletParticleEmitter() = default;

void Game::BulletParticleEmitter::setupParticles(std::vector<GLTexture*>* textures, float particleWidth, float particleHeight, unsigned int amount) {
	ParticleEmitter::setupParticles(textures, particleWidth, particleHeight, amount);
}

void Game::BulletParticleEmitter::update() {
	for (size_t i = 0; i < particles.size(); i++) {
		Particle* particle = &particles.at(i);
		if (particle->getProperty(Transformable3DProperty::Y_POSITION) >= particle->getDistance()) {
			setupParticlePositioning(i, particles.size());
		} else {
			float translation = time->getTimeBetweenFramesInNanos() * particle->getSpeed();
			switch (direction) {
				case TileDirection::RIGHT:
					particle->translate(-translation, 0, 0);
					break;
				case TileDirection::FORWARD:
					particle->translate(0, 0, translation);
					break;
				case TileDirection::LEFT:
					particle->translate(translation, 0, 0);
					break;
				case TileDirection::BACKWARD:
					particle->translate(0, 0, -translation);
					break;
				case TileDirection::UP:
					break;
				case TileDirection::DOWN:
					break;
			}
		}
	}
}

void Game::BulletParticleEmitter::setDirection(TileDirection direction) {
	this->direction = direction;

	switch (direction) {
		case TileDirection::RIGHT:
			particleRotation = glm::vec3(QUARTER_PI, 0, 0);
			break;
		case TileDirection::FORWARD:
			particleRotation = glm::vec3(QUARTER_PI, HALF_PI, 0);
			break;
		case TileDirection::LEFT:
			particleRotation = glm::vec3(QUARTER_PI, 0, 0);
			break;
		case TileDirection::BACKWARD:
			particleRotation = glm::vec3(QUARTER_PI, -HALF_PI, 0);
			break;
		case TileDirection::UP:
			break;
		case TileDirection::DOWN:
			break;
	}
}

void Game::BulletParticleEmitter::setupParticlePositioning(size_t whichParticle, size_t numberOfParticles) {
	particles[whichParticle].setPosition(position);
	particles[whichParticle].setRotation(particleRotation);
}
