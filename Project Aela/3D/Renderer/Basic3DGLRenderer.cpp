/*
* Class: Basic 3D GLRenderer
* Author: Robert Ciborowski
* Date: October 2016
* Description: A class used by Aela's GLRenderer that uses other, more basic objects
*              to render 3D items.
*/

#include "Basic3DGLRenderer.h"
#include "../../Utilities/flut.h"
#include <glm/gtx/component_wise.hpp>

using namespace Aela;

// This sets up the class.
void Basic3DGLRenderer::setup(unsigned int multisampling) {
	setupShaders();
	setupFrameBuffers(multisampling);
	getIDs();
}

// This creates and compiles the shaders.
void Basic3DGLRenderer::setupShaders() {
	depthProgramID = loadShaders("../../res/shaders/3D/Depth.vert", "../../res/shaders/3D/Depth.geom", "../../res/shaders/3D/Depth.frag");
	modelProgramID = loadShaders("../../res/shaders/3D/Model.vert", "../../res/shaders/3D/Model.frag");
	billboardProgramID = loadShaders("../../res/shaders/3D/Billboards.vert", "../../res/shaders/3D/Billboards.frag");
	skyboxProgramID = loadShaders("../../res/shaders/3D/Skybox.vert", "../../res/shaders/3D/Skybox.frag");
}

// This gets handles to GLSL uniforms.
void Basic3DGLRenderer::getIDs() {
	modelTextureID = glGetUniformLocation(modelProgramID, "textureSampler");
	projectionMatrixID = glGetUniformLocation(modelProgramID, "projectionMatrix");
	viewMatrixID = glGetUniformLocation(modelProgramID, "viewMatrix");
	modelMatrixID = glGetUniformLocation(modelProgramID, "modelMatrices");
	rotationMatrixID = glGetUniformLocation(modelProgramID, "rotationMatrices");
	cameraPositionID = glGetUniformLocation(modelProgramID, "cameraPosition");
	shadowMapID = glGetUniformLocation(modelProgramID, "shadowMaps");
	numberOfLightsID = glGetUniformLocation(modelProgramID, "numberOfLights");
	lightPositionsID = glGetUniformLocation(modelProgramID, "lightPositions");
	lightDirectionsID = glGetUniformLocation(modelProgramID, "lightDirections");
	lightColoursID = glGetUniformLocation(modelProgramID, "lightColours[0]");
	lightPowersID = glGetUniformLocation(modelProgramID, "lightPowers[0]");

	billboardTextureID = glGetUniformLocation(billboardProgramID, "textureSampler");
	billboardMVPMatrixID = glGetUniformLocation(billboardProgramID, "modelViewProjectionMatrix");

	shadowMatrixID = glGetUniformLocation(depthProgramID, "shadowMatrices");
	shadowModelMatrixID = glGetUniformLocation(depthProgramID, "modelMatrices");
	lightShadowPositionsID = glGetUniformLocation(depthProgramID, "lightPosition");

	skyboxTextureID = glGetUniformLocation(skyboxProgramID, "skyboxTexture");
	skyboxViewMatrixID = glGetUniformLocation(skyboxProgramID, "viewMatrix");
	skyboxProjectionMatrixID = glGetUniformLocation(skyboxProgramID, "projectionMatrix");
}

// This sets up all of the framebuffers, including the MSAA framebuffer.
// It is also called if the MSAA amount changes.
void Basic3DGLRenderer::setupFrameBuffers(unsigned int multisampling) {
	// This generates the colour framebuffer.
	glGenFramebuffers(1, &colourFrameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, colourFrameBuffer);

	glGenTextures(1, colourFrameBufferTexture.getTexture());
	glBindTexture(GL_TEXTURE_2D, *(colourFrameBufferTexture.getTexture()));
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, windowWidth, windowHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	/* Clamping to edges is important to prevent artifacts when scaling */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	/* Linear filtering usually looks best for text */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	colourFrameBufferTexture.setDimensions(0, 0, windowWidth, windowHeight);
	glDrawBuffer(GL_COLOR_ATTACHMENT0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, *(colourFrameBufferTexture.getTexture()), 0);

	if (multisampling > 0) {
		// This generates the multisampled colour framebuffer, which later is blitted to the regular colour framebuffer.
		glGenFramebuffers(1, &multisampledColourFrameBuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, multisampledColourFrameBuffer);

		glGenTextures(1, multisampledColourFrameBufferTexture.getTexture());
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, *(multisampledColourFrameBufferTexture.getTexture()));
		glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGBA, windowWidth, windowHeight, GL_TRUE);
		/* Clamping to edges is important to prevent artifacts when scaling */
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		/* Linear filtering usually looks best for text */
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		multisampledColourFrameBufferTexture.setDimensions(0, 0, windowWidth, windowHeight);
		glDrawBuffer(GL_COLOR_ATTACHMENT0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, *(multisampledColourFrameBufferTexture.getTexture()), 0);
	} else {
		// If the MSAA amount is changed from a value greater than zero to zero, the buffer and texture should be deleted.
		glDeleteBuffers(1, &multisampledColourFrameBuffer);
		glDeleteTextures(1, multisampledColourFrameBufferTexture.getTexture());
	}

	// This generates the depth renderbuffer, which is used alongside the multisampled colour framebuffer.
	glGenRenderbuffers(1, &depthRenderBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthRenderBuffer);
	if (multisampling > 0) {
		glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH_COMPONENT, windowWidth, windowHeight);
	} else {
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, windowWidth, windowHeight);
	}
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderBuffer);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		AelaErrorHandling::windowError("Project Aela 3D Rendering",
			"There was a problem setting movingUp the colour framebuffer.\nIt's probably OpenGL's fault.\nOr maybe your graphics processor is a potato.");
	}
}

// This generates a light's depth frame buffer.
void Basic3DGLRenderer::generateShadowMap(LightEntity* light) {
	GLuint* buffer = light->getShadowMapBuffer();
	glGenFramebuffers(1, buffer);

	// This generates the depth texture, which is sampled from later when rendering models.
	GLuint* texture = light->getShadowMapTexture();
	glGenTextures(1, texture);
	
	glBindTexture(GL_TEXTURE_CUBE_MAP, *texture);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	for (unsigned int i = 0; i < 6; i++) {
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT24, shadowRenderer.getDepthTextureWidth(), shadowRenderer.getDepthTextureHeight(), 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
	glBindFramebuffer(GL_FRAMEBUFFER, *buffer);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, *texture, 0);

	// There is no colour output in the bound framebuffer, only depth.
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		AelaErrorHandling::windowError("Aela 3D",
			"There was a problem setting movingUp the depth framebuffer.\nIt's probably OpenGL's fault.\nOr maybe your graphics processor is a potato.");
		light->setShadowMapBuffer(NULL);
		light->setShadowMapTexture(NULL);
	} else {
		light->setShadowMapBuffer(buffer);
		light->setShadowMapTexture(texture);
	}
}

void Basic3DGLRenderer::setWindow(Window* setWindow) {
	window = setWindow;
	window->getWindowDimensions(&windowWidth, &windowHeight);
}

void Basic3DGLRenderer::setCamera(Camera3D* setCamera) {
	camera = setCamera;
}

void Basic3DGLRenderer::bindLights(std::unordered_map<long long, LightEntity>* lights) {
	this->lights = lights;
}

Window* Basic3DGLRenderer::getWindow() {
	return window;
}

GLuint* Basic3DGLRenderer::getColourFrameBuffer() {
	return &colourFrameBuffer;
}

Image* Basic3DGLRenderer::getColourFrameBufferTexture() {
	return &colourFrameBufferTexture;
}

GLuint * Aela::Basic3DGLRenderer::getMultisampledColourFrameBuffer() {
	return &multisampledColourFrameBuffer;
}

Image* Aela::Basic3DGLRenderer::getMultisampledColourFrameBufferTexture() {
	return &multisampledColourFrameBufferTexture;
}

void Aela::Basic3DGLRenderer::renderShadows(Map3D* map) {
	glViewport(0, 0, shadowRenderer.getDepthTextureWidth(), shadowRenderer.getDepthTextureHeight());

	shadowRenderer.startRenderingShadows(depthProgramID);
	for (auto& pair : *map->getResourceGroupedModelsWithoutTransparency()) {
		for (unsigned long long start = 0; start < pair.second.size(); start += SHADOW_INSTANCE_LIMIT) {
			unsigned long long end = start + SHADOW_INSTANCE_LIMIT;
			if (end > pair.second.size()) {
				end = pair.second.size();
			}
			shadowRenderer.renderInstancedShadows(map, &pair.second, start, end, depthProgramID, shadowModelMatrixID, shadowMatrixID,
				lights, lightPositionsID);
		}
	}
	shadowRenderer.endRenderingShadows();
}

void Aela::Basic3DGLRenderer::renderModelEntities(Map3D* map, bool multisampling) {
	glViewport(0, 0, windowWidth, windowHeight);
	modelRenderer.setMatrices(camera->getViewMatrix(), camera->getProjectionMatrix());

	if (multisampling) {
		modelRenderer.startRenderingModelEntities(modelProgramID, multisampledColourFrameBuffer, viewMatrixID, projectionMatrixID);
	} else {
		modelRenderer.startRenderingModelEntities(modelProgramID, colourFrameBuffer, viewMatrixID, projectionMatrixID);
	}

	for (auto& pair : *map->getResourceGroupedModelsWithoutTransparency()) {
		for (unsigned long long start = 0; start < pair.second.size(); start += MODEL_INSTANCE_LIMIT) {
			unsigned long long end = start + MODEL_INSTANCE_LIMIT;
			if (end > pair.second.size()) {
				end = pair.second.size();
			}
			if (multisampling) {
				modelRenderer.renderInstancedModelEntities(map, &pair.second, start, end, modelProgramID, multisampledColourFrameBuffer,
					modelMatrixID, rotationMatrixID, modelTextureID);
			} else {
				modelRenderer.renderInstancedModelEntities(map, &pair.second, start, end, modelProgramID, colourFrameBuffer,
					modelMatrixID, rotationMatrixID, modelTextureID);
			}
		}
	}

	for (auto& pair : *map->getResourceGroupedModelsWithTransparency()) {
		for (unsigned long long start = 0; start < pair.second.size(); start += MODEL_INSTANCE_LIMIT) {
			unsigned long long end = start + MODEL_INSTANCE_LIMIT;
			if (end > pair.second.size()) {
				end = pair.second.size();
			}
			if (multisampling) {
				modelRenderer.renderInstancedModelEntities(map, &pair.second, start, end, modelProgramID, multisampledColourFrameBuffer,
					modelMatrixID, rotationMatrixID, modelTextureID);
			} else {
				modelRenderer.renderInstancedModelEntities(map, &pair.second, start, end, modelProgramID, colourFrameBuffer,
					modelMatrixID, rotationMatrixID, modelTextureID);
			}
		}
	}

	modelRenderer.endRenderingModelEntities();
}

// This clears the colour frame buffer.
void Basic3DGLRenderer::clearColourFrameBuffer(bool multisampling) {
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	if (multisampling) {
		glBindFramebuffer(GL_FRAMEBUFFER, multisampledColourFrameBuffer);
	} else {
		glBindFramebuffer(GL_FRAMEBUFFER, colourFrameBuffer);
	}
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

// This renders a texture in 3D space.
void Basic3DGLRenderer::renderTextureIn3DSpace(GLuint* texture, bool cullFaces, glm::vec3* position, glm::vec3* scaling, glm::vec3* lookAt, bool inverseRotation, bool multisampling) {
	// Note: for regular texture rendering, use:
	// renderTextureIn3DSpace((texture, false, position, position + glm::vec3(0.0, 0.0, 1.0), false);
	glViewport(0, 0, windowWidth, windowHeight);
	modelRenderer.setMatrices(camera->getViewMatrix(), camera->getProjectionMatrix());
	if (multisampling) {
		modelRenderer.renderTextureIn3DSpace(cullFaces, *texture, billboardTextureID, billboardProgramID, multisampledColourFrameBuffer, billboardMVPMatrixID, position, scaling, lookAt, inverseRotation);
	} else {
		modelRenderer.renderTextureIn3DSpace(cullFaces, *texture, billboardTextureID, billboardProgramID, colourFrameBuffer, billboardMVPMatrixID, position, scaling, lookAt, inverseRotation);
	}
}

// This renders a billboard, accounting for multisampling.
void Basic3DGLRenderer::renderBillboard(BillboardEntity* billboard, bool multisampling) {
	if (billboard->getTexture() != nullptr) {
		glViewport(0, 0, windowWidth, windowHeight);
		modelRenderer.setMatrices(camera->getViewMatrix(), camera->getProjectionMatrix());
		if (billboard->usingSpecifiedRotation()) {
			if (multisampling) {
				modelRenderer.renderTextureIn3DSpace(true, *billboard->getTexture()->getTexture(), billboardTextureID,
					billboardProgramID, multisampledColourFrameBuffer, billboardMVPMatrixID, billboard->getPosition(),
					billboard->getRotation(), billboard->getScaling());
			} else {
				modelRenderer.renderTextureIn3DSpace(true, *billboard->getTexture()->getTexture(), billboardTextureID,
					billboardProgramID, colourFrameBuffer, billboardMVPMatrixID, billboard->getPosition(),
					billboard->getRotation(), billboard->getScaling());
			}
		} else {
			if (multisampling) {
				modelRenderer.renderTextureIn3DSpace(true, *billboard->getTexture()->getTexture(), billboardTextureID,
					billboardProgramID, multisampledColourFrameBuffer, billboardMVPMatrixID, billboard->getPosition(),
					billboard->getScaling(), camera->getPosition(), true);
			} else {
				modelRenderer.renderTextureIn3DSpace(true, *billboard->getTexture()->getTexture(), billboardTextureID,
					billboardProgramID, colourFrameBuffer, billboardMVPMatrixID, billboard->getPosition(),
					billboard->getScaling(), camera->getPosition(), true);
			}
		}
	}
}

void Basic3DGLRenderer::clearShadowMaps() {
	shadowRenderer.clearShadowMaps(lights);
}

// This function tells the renderer to send the lights added through bindLights() to the shaders.
void Basic3DGLRenderer::sendLightDataToShader() {
	modelRenderer.sendLightDataToShader(lights, modelProgramID, numberOfLightsID, lightPositionsID, lightDirectionsID, lightColoursID, lightPowersID, shadowMapID);
}

// This renders a skybox, accounting for multisampling.
void Basic3DGLRenderer::renderSkybox(Skybox* skybox, bool multisampling) {
	glViewport(0, 0, windowWidth, windowHeight);
	skyboxRenderer.setMatrices(camera->getViewMatrix(), camera->getProjectionMatrix());
	if (multisampling) {
		skyboxRenderer.renderSkybox(skybox, skyboxProgramID, multisampledColourFrameBuffer, skyboxTextureID, skyboxViewMatrixID, skyboxProjectionMatrixID);
	} else {
		skyboxRenderer.renderSkybox(skybox, skyboxProgramID, colourFrameBuffer, skyboxTextureID, skyboxViewMatrixID, skyboxProjectionMatrixID);
	}
}

// This renders particles. Keep in mind that particles are sorted in the ParticleEmitter's list (based on their z position, relative to the 
// emitter). This function uses a (probably slow) technique to figure out whether the particles should be rendered according to their order in the
// list or according the backwards version of this order. This is necessary since if billboards (which is what a particle is) are not rendered
// from the greatest z-distance to the smallest z distance relative to position 0, 0, 0 in the camera space, their alpha blending will screw up and
// look bad.
void Aela::Basic3DGLRenderer::renderParticles(ParticleEmitter* particleEmitter, Camera3D* camera, bool multisampling) {
	glViewport(0, 0, windowWidth, windowHeight);
	modelRenderer.setMatrices(camera->getViewMatrix(), camera->getProjectionMatrix());
	glm::vec3 actualCameraRotation = glm::vec3(camera->getRotation()->y, camera->getRotation()->x, camera->getRotation()->z);
	glm::vec3 differenceA = actualCameraRotation - *particleEmitter->getRotation();
	glm::vec3 differenceB = *particleEmitter->getRotation() - actualCameraRotation;
	forceValuesWithinRange(&differenceA, 0, glm::pi<float>() * 2);
	forceValuesWithinRange(&differenceB, 0, glm::pi<float>() * 2);
	glm::vec3 difference = glm::vec3(glm::min(differenceA.x, differenceB.x), glm::min(differenceA.y, differenceB.y),
		glm::min(differenceA.z, differenceB.z));
	float angle = glm::compMax(difference);

	if (angle < glm::pi<float>() / 2) {
		for (Particle particle : *particleEmitter->getParticles()) {
			renderParticle(particle, particleEmitter->getPosition(), particleEmitter->getRotation(), particleEmitter->getScaling(),
				multisampling);
		}
	} else {
		for (unsigned long long i = particleEmitter->getParticles()->size() - 1; i > 0; i--) {
			renderParticle(particleEmitter->getParticles()->at(i), particleEmitter->getPosition(), particleEmitter->getRotation(),
				particleEmitter->getScaling(), multisampling);
		}
	}
}

void Aela::Basic3DGLRenderer::renderParticle(Particle particle, glm::vec3* positionOffset, glm::vec3* rotationOffset, glm::vec3* scalingOffset,
	bool multisampling) {
	glm::vec3 position = *particle.getPosition() + *positionOffset;
	glm::vec3 rotation = *particle.getRotation() + *rotationOffset;
	glm::vec3 scaling = *particle.getScaling() * *scalingOffset;

	if (particle.usingSpecifiedRotation()) {
		if (multisampling) {
			modelRenderer.renderTextureIn3DSpace(false, *particle.getTexture()->getTexture(), billboardTextureID, billboardProgramID, multisampledColourFrameBuffer, billboardMVPMatrixID, &position, &rotation, &scaling);
		} else {
			modelRenderer.renderTextureIn3DSpace(false, *particle.getTexture()->getTexture(), billboardTextureID, billboardProgramID, colourFrameBuffer, billboardMVPMatrixID, &position, &rotation, &scaling);
		}
	} else {
		if (multisampling) {
			modelRenderer.renderTextureIn3DSpace(false, *particle.getTexture()->getTexture(), billboardTextureID, billboardProgramID, multisampledColourFrameBuffer, billboardMVPMatrixID, &position, &scaling, camera->getPosition(), true);
		} else {
			modelRenderer.renderTextureIn3DSpace(false, *particle.getTexture()->getTexture(), billboardTextureID, billboardProgramID, colourFrameBuffer, billboardMVPMatrixID, &position, &scaling, camera->getPosition(), true);
		}
	}
}