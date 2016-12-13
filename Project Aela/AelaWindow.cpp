#include "AelaWindow.h"
#include "AelaError.h"

void AelaWindow::addProperty(AelaWindowFlag flag) {
	bool flagExists = false;
	for (int i = 0; i < flags.size(); i++) {
		if (flags[i] == flag) {
			flagExists = true;
			AelaErrorHandling::windowError("Flag exists.");
			break;
		}
	}
	if (flagExists == false) {
		if (flag == AelaWindowFlag::AELA_WINDOW_RESIZABLE) {
			flags.insert(flags.begin() + flags.size(), AelaWindowFlag::AELA_WINDOW_RESIZABLE);
		} else if (flag == AelaWindowFlag::AELA_WINDOW_SHOWN) {
			flags.insert(flags.begin() + flags.size(), AelaWindowFlag::AELA_WINDOW_SHOWN);
		} else if (flag == AelaWindowFlag::AELA_WINDOW_BORDERLESS) {
			flags.insert(flags.begin() + flags.size(), AelaWindowFlag::AELA_WINDOW_BORDERLESS);
		} else if (flag == AelaWindowFlag::AELA_WINDOW_MINIMIZED) {
			flags.insert(flags.begin() + flags.size(), AelaWindowFlag::AELA_WINDOW_MINIMIZED);
		} else if (flag == AelaWindowFlag::AELA_WINDOW_OPENGL) {
			flags.insert(flags.begin() + flags.size(), AelaWindowFlag::AELA_WINDOW_OPENGL);
		}
	}
}

bool AelaWindow::createWindow(int setWidth, int setHeight, int setXPosition, int setYPosition, std::string setName) {
	// This sets general object properties.
	windowName = setName.c_str();
	windowWidth = setWidth;
	windowHeight = setHeight;

	SDL_Init(SDL_INIT_EVERYTHING);

	// The window's SDL flags. By default, this uses SDL_WINDOW_MOUSE_FOCUS as if it were NULL.
	SDL_WindowFlags resizableFlag = SDL_WINDOW_MOUSE_FOCUS, shownFlag = SDL_WINDOW_MOUSE_FOCUS,
	borderlessFlag = SDL_WINDOW_MOUSE_FOCUS, minimizedFlag = SDL_WINDOW_MOUSE_FOCUS,
	openGLFlag = SDL_WINDOW_MOUSE_FOCUS;

	// This starts looking at the flags inputted as parameters.
	for (int i = 0; i < flags.size(); i++) {
		if (flags[i] == AelaWindowFlag::AELA_WINDOW_RESIZABLE) {
			resizableFlag = SDL_WINDOW_RESIZABLE;
		} else if (flags[i] == AelaWindowFlag::AELA_WINDOW_SHOWN) {
			shownFlag = SDL_WINDOW_SHOWN;
		} else if (flags[i] == AelaWindowFlag::AELA_WINDOW_BORDERLESS) {
			borderlessFlag = SDL_WINDOW_BORDERLESS;
		} else if (flags[i] == AelaWindowFlag::AELA_WINDOW_MINIMIZED) {
			minimizedFlag = SDL_WINDOW_MINIMIZED;
		} else if (flags[i] == AelaWindowFlag::AELA_WINDOW_OPENGL) {
			openGLFlag = SDL_WINDOW_OPENGL;
		}
	}

	// This sets additional flags.
	if (shownFlag != SDL_WINDOW_SHOWN) {
		shownFlag = SDL_WINDOW_HIDDEN;
	}
	if (minimizedFlag != SDL_WINDOW_MINIMIZED) {
		minimizedFlag = SDL_WINDOW_MAXIMIZED;
	}

	// This creates and sets up the SDL window.
	window = SDL_CreateWindow(windowName.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, windowWidth, windowHeight, resizableFlag | shownFlag | borderlessFlag | minimizedFlag | openGLFlag);
	if (window == NULL) {
		return false;
	}
	SDL_SetWindowSize(window, windowWidth, windowHeight);
	SDL_SetWindowPosition(window, setXPosition, setYPosition);
	return true;
}

void AelaWindow::getWindowDimensions(int * widthVariable, int * heightVariable) {
	*widthVariable = windowWidth;
	*heightVariable = windowHeight;
}

void AelaWindow::getWindowPosition(int * xPositionVariable, int * yPositionVariable) {
	SDL_GetWindowPosition(window, xPositionVariable, yPositionVariable);
}

bool AelaWindow::makeWindowOpenGLContext() {
	openGLContext = SDL_GL_CreateContext(window);
	if (openGLContext == NULL) {
		return false;
	}
	return true;
}

void AelaWindow::updateBuffer() {
	SDL_GL_SwapWindow(window);
}

void AelaWindow::updateWindowEvents() {
	keystates = SDL_GetKeyboardState(NULL);
}

void AelaWindow::getCursorPositionInWindow(int * x, int * y) {
	SDL_GetMouseState(x, y);
}

void AelaWindow::getCursorPositionGlobally(int * x, int * y) {
	SDL_GetGlobalMouseState(x, y);
}

void AelaWindow::setCursorPositionInWindow(int x, int y) {
	SDL_WarpMouseInWindow(window, x, y);
}

void AelaWindow::setCursorPositionGlobally(int x, int y) {
	SDL_WarpMouseGlobal(x, y);
}

bool AelaWindow::quitCheck() {
	SDL_Event occur;
	SDL_PollEvent(&occur);
	return occur.type == SDL_QUIT;
}

bool AelaWindow::keyPressed(int ASCII_Code) {
	return keystates[ASCII_Code];
}

void AelaWindow::showCursor() {
	SDL_ShowCursor(SDL_ENABLE);
}

void AelaWindow::hideCursor() {
	SDL_ShowCursor(SDL_DISABLE);
}