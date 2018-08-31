#include "LaserParticleEmitter.h"
#include "../Utilities/MathConstants.h"

Game::LaserParticleEmitter::LaserParticleEmitter(Clock* time) : ParticleEmitter(time) {
}

Game::LaserParticleEmitter::~LaserParticleEmitter() = default;

void Game::LaserParticleEmitter::setupParticles(std::vector<GLTexture*>* textures, float particleWidth, float particleHeight, unsigned int amount) {
	ParticleEmitter::setupParticles(textures, particleWidth, particleHeight, amount);
}

void Game::LaserParticleEmitter::update() {
	//for (size_t i = 0; i < particles.size(); i++) {
	//	Particle* particle = &particles.at(i);
	//	if (particle->getProperty(Transformable3DProperty::Y_POSITION) >= particle->getDistance()) {
	//		setupParticlePositioning(i, particles.size());
	//	} else {
	//		particle->translate(0, time->getTimeBetweenFramesInNanos() * particle->getSpeed(), 0);
	//		std::cout << particle->getPosition()->x << " " << particle->getPosition()->y << " " <<
	//			particle->getPosition()->z << " is the p\n";
	//		std::cout << particle->getRotation()->x << " " << particle->getRotation()->y << " " <<
	//			particle->getRotation()->z << " is the r\n";
	//		// float scaling = (particle->getDistance() - particle->getScaling()->y) / particle->getDistance();
	//		// std::cout << scaling * particle->getScaling()->x << "\n";
	//		// particle->rotate(glm::vec3(0, time->getTimeBetweenFramesInNanos() * particle->getSpeed() / 4, 0));
	//		// particle->setScaling(glm::vec3(scaling * particle->getScaling()->x, 1, scaling * particle->getScaling()->z));
	//		// particle->setRotation(glm::vec3(HALF_PI, scaling * DOUBLE_PI, 0));
	//	}
	//}
}

void Game::LaserParticleEmitter::setDirection(TileDirection direction) {
	this->direction = direction;

	switch (direction) {
		case TileDirection::RIGHT:
			particleRotation = glm::vec3(HALF_PI, 0, 0);
			break;
		case TileDirection::FORWARD:
			particleRotation = glm::vec3(HALF_PI, HALF_PI, 0);
			break;
		case TileDirection::LEFT:
			particleRotation = glm::vec3(HALF_PI, 0, 0);
			break;
		case TileDirection::BACKWARD:
			particleRotation = glm::vec3(HALF_PI, -HALF_PI, 0);
			break;
		case TileDirection::UP:
			break;
		case TileDirection::DOWN:
			break;
	}
}

void Game::LaserParticleEmitter::setupParticlePositioning(size_t whichParticle, size_t numberOfParticles) {
	glm::vec3 position;

	switch (direction) {
		case TileDirection::RIGHT:
			position.x -= 1 + whichParticle;
			break;
		case TileDirection::FORWARD:
			position.z += 1 + whichParticle;
			break;
		case TileDirection::LEFT:
			position.x += 1 + whichParticle;
			break;
		case TileDirection::BACKWARD:
			position.z -= 1 + whichParticle;
			break;
		case TileDirection::UP:
			break;
		case TileDirection::DOWN:
			break;
	}
	position.z += 0.5f;
	position.x += 0.5f;
	position.y += 0.3f;

	particles[whichParticle].setPosition(position);
	particles[whichParticle].setRotation(particleRotation);
}
