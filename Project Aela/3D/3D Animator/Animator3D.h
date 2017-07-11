/*
* Class: Project Aela's 3D Animator3D
* Author: Robert Ciborowski
* Date: 10/07/2017
* Description: A class used by Aela's 3D Animator3D to perform transformations on 3D objects.
*/

#include <vector>

#include "KeyFrame3D.h"
#include "../../Time Manager/TimeManager.h"

using namespace Aela;

class Animator3D {
	public:
		Animator3D() {

		}

		void update();

		void setTimeManager(TimeManager* timeManager);
		TimeManager* getTimeManager();
		void addKeyFrame(KeyFrame3D* keyFrame);
		std::vector<KeyFrame3D>* getKeyFrames();

	private:
		TimeManager* timeManager;
		std::vector<KeyFrame3D> keyFrames;
		unsigned int timeSinceLastKeyFrame = 0;

		glm::vec3 originalPosition;
		glm::vec3 originalRotation;
};