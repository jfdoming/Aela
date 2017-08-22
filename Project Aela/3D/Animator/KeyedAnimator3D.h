/*
* Class: Project Aela's Keyed 3D Animator
* Author: Robert Ciborowski
* Date: 20/08/2017
* Description: A class used by Aela's 3D Animator to perform transformations on 3D objects.
*              The transformations occur based on the user's key inputs.
*/

#pragma once

#include <vector>
#include <unordered_map>

#include "../../Time Manager/TimeManager.h"
#include "../../Window/Window.h"
#include "../../Events/EventHandler.h"
#include "../Transformable/Transformable3D.h"

using namespace Aela;

class KeyedAnimator3D : public Listener {
	public:
		KeyedAnimator3D() {

		}

		// This is triggered on an event.
		void onEvent(Event* event);

		// This should be called in some srt of animation thread or something.
		void update();

		// These are getters and setters.
		void setTimeManager(TimeManager* timeManager);
		TimeManager* getTimeManager();
		void setWindow(Window* window);
		Window* getWindow();
		bool addTransformable(int key, Transformable3D* transformable);
		int addTransformable(Transformable3D* transformable);
		bool removeTransformable(int key);

	private:
		// These are the objects from Project Aela that the class uses.
		TimeManager* timeManager;
		Window* window;
		std::unordered_map<int, Transformable3D*> transformables;

		// Speed: 0.001f is 1 unit per tick.
		float speed = 0.03f, superSpeed = 0.12f, currentSpeed = 0.0f, mouseSpeed;

		// These store movements.
		bool forward, backward, left, right, up, down;
};