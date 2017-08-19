/*
* Class: Aela Engine Functor
* Author: Robert Ciborowski
* Date: 19/08/2017
* Description: A functor that contains a pointer to an Aela Engine. This can be extremely useful in very specific cases
               since this allows an object to be given a functor to call for later with this functor having access to
			   the entire engine.

			   Question: This is a disgusting way of doing things, why would you do this?
			   Answer: Because it makes certain situations easy and simple rather than complicated and messy.

			   Question: Why would you even use a functor in the first place? They're basically the bloatware of C++.
			   Answer: Because they make certain situations easy and simple rather than complicated and messy.
*/

#pragma once

#include "../Aela_Engine.h"

using namespace Aela;

class AelaEngineFunctor {
	public:
		AelaEngineFunctor() {

		}

		AelaEngineFunctor(Engine* engine, void (*action)(Engine* engine)) {
			this->engine = engine;
			this->action = action;
		}

		void operator()() {
			action(engine);
		};

		void setEngine(Engine* engine) {
			this->engine = engine;
		}

		void setAction(void(*action)(Engine* engine)) {
			this->action = action;
		}

	private:
		Engine* engine;
		void (*action)(Engine* engine);
};