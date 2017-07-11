// Aela Program.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Aela_Engine.h"

int main(int argc, char *args[]) {
	using namespace Aela;

	int error = setupWindow(1280, 720, 50, 50);
	if (error != 0) {
		return error;
	}

	error = setupRenderer();
	if (error != 0) {
		return error;
	}

	error = setupControlManager();
	if (error != 0) {
		return error;
	}

	error = setupLUA();
	if (error != 0) {
		return error;
	}

	error = setupEventHandler();
	if (error != 0) {
		return error;
	}

	error = setupAnimator();
	if (error != 0) {
		return error;
	}

	Aela::start();

    return 0;
}