/*
* Class: Specifications Manager
* Author: Robert Ciborowski
* Date: 18/08/2017
* Description: A class for finding and storing the specifications of the user's machine.
*/

#include "SpecificationsManager.h"

void Aela::SpecificationsManager::generateInfo(Renderer* renderer) {
	graphicsVendor = renderer->getInformation(RendererInformation::VENDOR);
	graphicsRenderer = renderer->getInformation(RendererInformation::RENDERER);
	openGLVersion = renderer->getInformation(RendererInformation::OPENGL_VERSION);
	GLSLVersion = renderer->getInformation(RendererInformation::GLSL_VERSION);
	openGLExtensions = renderer->getInformation(RendererInformation::OPENGL_EXTENSIONS);

	for (int i = 0; i < SDL_GetNumAudioDevices(0); i++) {
		audioDeviceNames.push_back(std::string(SDL_GetAudioDeviceName(i, 0)));
	}

	for (int i = 0; i < SDL_GetNumAudioDrivers(); i++) {
		audioDeviceDrivers.push_back(std::string(SDL_GetAudioDriver(i)));
	}

	platform = std::string(SDL_GetPlatform());
	l1CacheLineSize = SDL_GetCPUCacheLineSize();
	logicalCPUCores = SDL_GetCPUCount();
	systemRAM = SDL_GetSystemRAM();
}

std::vector<std::string>* Aela::SpecificationsManager::getAudioDeviceNames() {
	return &audioDeviceNames;
}

std::vector<std::string>* Aela::SpecificationsManager::getAudioDeviceDrivers() {
	return &audioDeviceDrivers;
}

std::string Aela::SpecificationsManager::getCurrentAudioDriver() {
	return currentAudioDriver;
}

std::string Aela::SpecificationsManager::getGraphicsVendor() {
	return graphicsVendor;
}

std::string Aela::SpecificationsManager::getGraphicsRenderer() {
	return graphicsRenderer;
}

std::string Aela::SpecificationsManager::getOpenGLVersion() {
	return openGLVersion;
}

std::string Aela::SpecificationsManager::getGLSLVersion() {
	return GLSLVersion;
}

std::string Aela::SpecificationsManager::getOpenGLExtensions() {
	return openGLExtensions;
}

std::string Aela::SpecificationsManager::getPlatform() {
	return platform;
}

unsigned int Aela::SpecificationsManager::getL1CacheLineSize() {
	return l1CacheLineSize;
}

unsigned int Aela::SpecificationsManager::getLogicalCPUCores() {
	return logicalCPUCores;
}

unsigned int Aela::SpecificationsManager::getSystemRAM() {
	return systemRAM;
}
