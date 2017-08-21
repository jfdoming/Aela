#include "UserEnvironment.h"

void Aela::UserEnvironment::generateInfo(Renderer* renderer) {
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

std::vector<std::string>* Aela::UserEnvironment::getAudioDeviceNames() {
	return &audioDeviceNames;
}

std::vector<std::string>* Aela::UserEnvironment::getAudioDeviceDrivers() {
	return &audioDeviceDrivers;
}

std::string Aela::UserEnvironment::getCurrentAudioDriver() {
	return currentAudioDriver;
}

std::string Aela::UserEnvironment::getGraphicsVendor() {
	return graphicsVendor;
}

std::string Aela::UserEnvironment::getGraphicsRenderer() {
	return graphicsRenderer;
}

std::string Aela::UserEnvironment::getOpenGLVersion() {
	return openGLVersion;
}

std::string Aela::UserEnvironment::getGLSLVersion() {
	return GLSLVersion;
}

std::string Aela::UserEnvironment::getOpenGLExtensions() {
	return openGLExtensions;
}

std::string Aela::UserEnvironment::getPlatform() {
	return platform;
}

unsigned int Aela::UserEnvironment::getL1CacheLineSize() {
	return l1CacheLineSize;
}

unsigned int Aela::UserEnvironment::getLogicalCPUCores() {
	return logicalCPUCores;
}

unsigned int Aela::UserEnvironment::getSystemRAM() {
	return systemRAM;
}