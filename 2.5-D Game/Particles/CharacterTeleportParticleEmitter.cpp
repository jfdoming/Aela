#include "CharacterTeleportParticleEmitter.h"
#include "../Utilities/MathConstants.h"
#include <glm/gtc/constants.hpp>

using namespace Game;

Game::CharacterTeleportParticleEmitter::CharacterTeleportParticleEmitter(Clock* time) : ParticleEmitter(time) {
}

Game::CharacterTeleportParticleEmitter::~CharacterTeleportParticleEmitter() {
	character->setVisibility(true);
}

void Game::CharacterTeleportParticleEmitter::setSecondaryPathOffset(float secondaryPathOffset) {
	this->secondaryPathOffset = secondaryPathOffset;
}

void CharacterTeleportParticleEmitter::setupParticles(std::vector<GLTexture*>* textures, float particleWidth, float particleHeight, unsigned int amount) {
	ParticleEmitter::setupParticles(textures, particleWidth, particleHeight, amount);
	texture = textures->at(0);
	this->particleWidth = particleWidth;
	this->particleHeight = particleHeight;
	this->amount = amount;
}

void CharacterTeleportParticleEmitter::update() {
	if (!halfLifeHasPassed && time->getCurrentTimeInNanos() >= (startTime + lifeTime / 2)) {
		halfLifeHasPassed = true;
		particles.clear();
		std::vector<GLTexture*> textures;
		textures.push_back(texture);
		setupParticles(&textures, particleWidth, particleHeight, amount);

		if (onHalfLife != NULL) {
			onHalfLife();
		}
	}


	// Note: the particle positioning is relative to the emitter. The rotation, position and scaling of the emitter is looked at by the renderer,
	// which renders the particles while taking these transformations into account.
	for (size_t i = 0; i < particles.size(); i++) {
		Particle* particle = &particles.at(i);
		if (particle->getProperty(Transformable3DProperty::Y_POSITION) >= particle->getDistance()) {
			setupParticlePositioning(i, particles.size());
		} else {
			if (halfLifeHasPassed) {
				particle->translate(0, time->getTimeBetweenFramesInNanos() * -particle->getSpeed(), 0);
			} else {
				particle->translate(0, time->getTimeBetweenFramesInNanos() * particle->getSpeed(), 0);
			}
		}
	}
}

void Game::CharacterTeleportParticleEmitter::setCharacter(Character* character) {
	this->character = character;
	character->setVisibility(false);
}

void Game::CharacterTeleportParticleEmitter::setLocations(Location* oldLocation, Location* newLocation) {
	glm::vec3 oldWorldSpacePosition = oldLocation->getWorldSpaceLocation();
	glm::vec3 newWorldSpacePosition = newLocation->getWorldSpaceLocation();
	dimensions1 = Rect<float>(oldWorldSpacePosition.x, oldWorldSpacePosition.z, 1, 1);
	dimensions2 = Rect<float>(newWorldSpacePosition.x, newWorldSpacePosition.z, 1, 1);
}

void Game::CharacterTeleportParticleEmitter::setActionOnHalfLife(std::function<void()> onHalfLife) {
	this->onHalfLife = onHalfLife;
}

void CharacterTeleportParticleEmitter::setupParticlePositioning(size_t whichParticle, size_t numberOfParticles) {
	if (halfLifeHasPassed) {
		particles[whichParticle].getPosition()->y = secondaryPathOffset;
		particles[whichParticle].getPosition()->x = dimensions2.getX() + particles[whichParticle].getScaling()->x / 2;
		particles[whichParticle].getPosition()->z = dimensions2.getY() + particles[whichParticle].getScaling()->y / 2;
	} else {
		particles[whichParticle].getPosition()->y = pathOffset;
		particles[whichParticle].getPosition()->x = dimensions1.getX() + particles[whichParticle].getScaling()->x / 2;
		particles[whichParticle].getPosition()->z = dimensions1.getY() + particles[whichParticle].getScaling()->y / 2;
	}
	
	particles[whichParticle].setRotation(glm::vec3(QUARTER_PI, 0, 0));
}
