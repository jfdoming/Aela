/*
* Class: Project Aela's 3D Animator3D
* Author: Robert Ciborowski
* Date: 10/07/2017
* Description: A class used by Aela's 3D Animator to perform transformations on 3D objects.
*/

#include <vector>

#include "KeyFrame3D.h"
#include "../../Time Manager/TimeManager.h"

using namespace Aela;

class Animator3D {
	public:
		Animator3D() {

		}

		// This function should be run every loop in the thread specified for animations.
		void update();

		// These are getters and setters.
		void setTimeManager(TimeManager* timeManager);
		TimeManager* getTimeManager();
		void addKeyFrame(KeyFrame3D* keyFrame);
		std::vector<KeyFrame3D>* getKeyFrames();

	private:
		// These are the Aela objects that this class uses.
		TimeManager* timeManager;
		std::vector<KeyFrame3D> keyFrames;

		// This stores the time since the most recently finished key frame.
		unsigned int timeSinceLastKeyFrame = 0;

		// This stores the original properties of an object before it was modified by the key frame.
		std::vector<glm::vec3> originalPositions;
		std::vector<glm::vec3> originalRotations;
};