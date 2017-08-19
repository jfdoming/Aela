/*
* Class: Specifications Manager
* Author: Robert Ciborowski
* Date: 18/08/2017
* Description: A class for finding and storing the specifications of the user's machine.
*/

#pragma once
#include "../Renderer/Renderer.h"

namespace Aela {
	class SpecificationsManager {
		public:
			SpecificationsManager() {

			}

			SpecificationsManager(Renderer* renderer) {
				generateInfo(renderer);
			}

			// This generates the info on the user's machine.
			void generateInfo(Renderer* renderer);

			// These are getters and setters.
			std::vector<std::string>* getAudioDeviceNames();
			std::vector<std::string>* getAudioDeviceDrivers();
			std::string getCurrentAudioDriver();
			std::string getGraphicsVendor();
			std::string getGraphicsRenderer();
			std::string getOpenGLVersion();
			std::string getGLSLVersion();
			std::string getOpenGLExtensions();
			std::string getPlatform();
			unsigned int getL1CacheLineSize();
			unsigned int getLogicalCPUCores();
			unsigned int getSystemRAM();

		private:
			std::vector<std::string> audioDeviceNames;
			std::vector<std::string> audioDeviceDrivers;
			std::string currentAudioDriver;
			std::string graphicsVendor, graphicsRenderer, openGLVersion, GLSLVersion, openGLExtensions;
			std::string platform;	
			unsigned int l1CacheLineSize, logicalCPUCores, systemRAM;
	};
}
