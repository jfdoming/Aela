// Aela Program.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Aela_Engine.h"

int main(int argc, char *args[]) {
	Aela::Engine engine;

	int error = engine.setupWindow(1280, 720, 50, 50);
	if (error != 0) {
		return error;
	}

	error = engine.setupRenderer();
	if (error != 0) {
		return error;
	}

	error = engine.setupControlManager();
	if (error != 0) {
		return error;
	}

	error = engine.setupLUA();
	if (error != 0) {
		return error;
	}

	error = engine.setupEventHandler();
	if (error != 0) {
		return error;
	}

	error = engine.setupAnimator();
	if (error != 0) {
		return error;
	}

	engine.start();

    return 0;
}