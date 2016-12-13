#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "SDL.h"

// This enum is used for the types of errors:
//		Console Window Errors:
//			Errors that appear in a seperate window .
//			The window looks like the Operating System's terminal.
//			Example: A window that looks like cmd in Windows.
//		Console Internal Errors:
//			Errors that appear in the engine's own console.
//			This console is accessible in-the-window/in-game.
//			Example: Try pressing '~' in Valve Source games.
//		Error Window Errors:
//			This displays a seperate error message
//			Example: A Windows error message with buttons and icons.

enum AelaErrorMessageType {
	AELA_ERROR_CONSOLE_WINDOW, AELA_ERROR_CONSOLE_INTERNAL, AELA_ERROR_WINDOW
};

namespace AelaErrorHandling {
	// These are the namespace's classes used for error handling.
	class AelaSimpleError {
	private:
		AelaErrorMessageType errorMessageType;
		std::string message;
		std::string title;

	public:
		AelaSimpleError() {

		}

		AelaSimpleError(AelaErrorMessageType setType, std::string setMessage) {
			setProperties(setType, setMessage);
		}

		AelaSimpleError(AelaErrorMessageType setType, std::string setTitle, std::string setMessage) {
			setProperties(setType, setTitle, setMessage);
		}

		void setProperties(AelaErrorMessageType setType, std::string setMessage) {
			errorMessageType = setType;
			message = setMessage;
			title = "Aela Error";
		}

		void setProperties(AelaErrorMessageType setType, std::string setTitle, std::string setMessage) {
			errorMessageType = setType;
			message = setMessage;
			title = setTitle;
		}

		AelaErrorMessageType getType() {
			return errorMessageType;
		}

		std::string getMessage() {
			return message;
		}

		std::string getTitle() {
			return title;
		}

	};

	class AelaErrorHandler {
	private:
		const SDL_MessageBoxColorScheme colorScheme = {
			{ /* .colors (.r, .g, .b) */
			  /* [SDL_MESSAGEBOX_COLOR_BACKGROUND] */
				{ 255,   0,   0 },
				/* [SDL_MESSAGEBOX_COLOR_TEXT] */
				{ 0, 255,   0 },
				/* [SDL_MESSAGEBOX_COLOR_BUTTON_BORDER] */
				{ 255, 255,   0 },
				/* [SDL_MESSAGEBOX_COLOR_BUTTON_BACKGROUND] */
				{ 0,   0, 255 },
				/* [SDL_MESSAGEBOX_COLOR_BUTTON_SELECTED] */
				{ 255,   0, 255 }
			}
		};

		const int numberOfButtons = 2;

		const SDL_MessageBoxButtonData buttons[2] = {
			{ 0, 0, "End Program" },
			{ SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 1, "Okay" }
		};


	public:
		bool programShouldBeClosed = false;

		AelaErrorHandler() {

		}

		void throwError(AelaSimpleError error);
		void requestProgramClose();
	};

	// This is the namespace's error handler.
	extern AelaErrorHandler errorHandler;

	// These are the namespace's simple behaviours.
	void consoleWindowError(std::string message);
	void consoleWindowError(std::string setTitle, std::string message);
	void consoleInternalError(std::string message);
	void consoleInternalError(std::string setTitle, std::string message);
	void windowError(std::string message);
	void windowError(std::string setTitle, std::string message);
	bool programCloseWasRequested();
}
