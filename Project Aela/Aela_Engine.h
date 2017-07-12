#pragma once

namespace Aela {
	class Engine {
		public:
			int setupWindow(unsigned int width, unsigned int height, unsigned int windowXPosition, unsigned int windowYPosition);
			int setupRenderer();
			int setupControlManager();
			int setupLUA();
			int setupEventHandler();
			int setupAudioPlayer();
			int setupAnimator();
			void start();
		private:
			int runningLoop();
	};
}