#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "SDL.h"

enum class AelaWindowFlag {
	AELA_WINDOW_RESIZABLE, AELA_WINDOW_NON_RESIZABLE,
	AELA_WINDOW_SHOWN, AELA_WINDOW_HIDDEN,
	AELA_WINDOW_BORDERLESS,
	AELA_WINDOW_MINIMIZED, AELA_WINDOW_MAXIMIZED,
	AELA_WINDOW_OPENGL
};

class AelaWindow {
	private:
		// These are typical window properties.
		int windowWidth, windowHeight;
		std::string windowName;
		bool resizable, visibility, borderless, maximized, taskbarVisibility;

		// This is the SDL_Window used by this class.
		SDL_Window * window;

		// These are the enumerators for the window flags.
		std::vector<AelaWindowFlag> flags;

		SDL_GLContext openGLContext;
		const Uint8 * keystates;

	public:
		// These are the constructors.
		AelaWindow() {

		}

		// These are the behaviours.
		void addProperty(AelaWindowFlag flag);
		bool createWindow(int setWidth, int setHeight, int setXPosition, int setYPosition, std::string setName);
		void getWindowDimensions(int * widthVariable, int * heightVariable);
		void getWindowPosition(int * xPositionVariable, int * yPositionVariable);
		bool makeWindowOpenGLContext();
		void updateBuffer();
		void updateWindowEvents();
		void getCursorPositionInWindow(int * x, int * y);
		void getCursorPositionGlobally(int * x, int * y);
		void setCursorPositionInWindow(int x, int y);
		void setCursorPositionGlobally(int x, int y);
		bool quitCheck();
		bool keyPressed(int ASCII_Code);
		static void showCursor();
		static void hideCursor();
};