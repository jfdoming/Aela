/*
* Name: Project Aela's Window Class
* Author: Robert Ciborowski
* Date: October 2016
* Description: A class that uses SDL to form a window.
*/

#pragma once
#include <string>
#include <vector>
#include "../Utilities/Rect/Rect.h"
#include "SDL.h"

namespace Aela {
	enum class WindowFlag {
		AELA_WINDOW_RESIZABLE, AELA_WINDOW_NON_RESIZABLE,
		AELA_WINDOW_SHOWN, AELA_WINDOW_HIDDEN,
		AELA_WINDOW_BORDERLESS,
		AELA_WINDOW_MINIMIZED, AELA_WINDOW_MAXIMIZED,
		AELA_WINDOW_OPENGL
	};

	class Window {
		private:
		// These are typical window properties.
		int windowWidth, windowHeight;
		std::string windowName;
		bool resizable, visibility, borderless, maximized, taskbarVisibility, hasFocus, shouldQuit;

		// This is the SDL_Window used by this class.
		SDL_Window* window;

		// These are the enumerators for the window flags.
		std::vector<WindowFlag> flags;

		// This is used to bind SDL with OpenGL.
		SDL_GLContext openGLContext;

		Rect<unsigned int> windowDimensions;

		public:
		// These are the constructors.
		Window() {
			hasFocus = true;
			windowName = "Aela Window";
		}

		// These are the behaviours.
		void addProperty(WindowFlag flag);
		bool createWindow(int setWidth, int setHeight, int setXPosition, int setYPosition, std::string setName);
		void getWindowDimensions(int* widthVariable, int* heightVariable);
		Rect<unsigned int>* getWindowDimensions();
		void getWindowPosition(int* xPositionVariable, int* yPositionVariable);
		bool makeWindowOpenGLContext();
		void updateBuffer();
		void getCursorPositionInWindow(int* x, int* y);
		void getCursorPositionGlobally(int* x, int* y);
		void setCursorPositionInWindow(int x, int y);
		void setCursorPositionGlobally(int x, int y);
		void setFocus(bool focus);
		std::string getWindowName();
		void quit();
		bool quitCheck();
		bool isFocused();

		void showCursor();
		static void hideCursor();
	};
}