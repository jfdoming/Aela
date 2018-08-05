#include "CharacterDeathParticleEmitter.h"
#include "../Utilities/MathConstants.h"
#include <glm/gtc/constants.hpp>

using namespace Game;

Game::CharacterDeathParticleEmitter::CharacterDeathParticleEmitter(Time* time) : ParticleEmitter(time) {
}

Game::CharacterDeathParticleEmitter::~CharacterDeathParticleEmitter() {
	character->setVisibility(true);
}

void CharacterDeathParticleEmitter::setupParticles(std::vector<GLTexture*>* textures, float particleWidth, float particleHeight, unsigned int amount) {
	ParticleEmitter::setupParticles(textures, particleWidth, particleHeight, amount);
	texture = textures->at(0);
	this->particleWidth = particleWidth;
	this->particleHeight = particleHeight;
	this->amount = amount;
}

void CharacterDeathParticleEmitter::update() {


	// Note: the particle positioning is relative to the emitter. The rotation, position and scaling of the emitter is looked at by the renderer,
	// which renders the particles while taking these transformations into account.
	for (size_t i = 0; i < particles.size(); i++) {
		Particle* particle = &particles.at(i);
		if (particle->getProperty(Transformable3DProperty::Y_POSITION) >= particle->getDistance()) {
			setupParticlePositioning(i, particles.size());
		} else {
			particle->translate(0, time->getTimeBetweenFramesInNanos() * particle->getSpeed(), 0);
		}
	}
}

void Game::CharacterDeathParticleEmitter::setCharacter(Character* character) {
	this->character = character;
	character->setVisibility(false);
}

void Game::CharacterDeathParticleEmitter::setLocation(Location* oldLocation) {
	glm::vec3 oldWorldSpacePosition = oldLocation->getWorldSpaceLocation();
	dimensions = Rect<float>(oldWorldSpacePosition.x, oldWorldSpacePosition.z, 1, 1);
}

void CharacterDeathParticleEmitter::setupParticlePositioning(size_t whichParticle, size_t numberOfParticles) {
	particles[whichParticle].getPosition()->y = pathOffset;
	particles[whichParticle].getPosition()->x = dimensions.getX() + particles[whichParticle].getScaling()->x / 2;
	particles[whichParticle].getPosition()->z = dimensions.getY() + particles[whichParticle].getScaling()->y / 2;
	
	particles[whichParticle].setRotation(glm::vec3(QUARTER_PI, 0, 0));
}
