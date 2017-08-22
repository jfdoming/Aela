/*
* Namespace: Project Aela's Error Handler
* Author: Robert Ciborowski
* Date: October 2016
* Description: A namespace with classes that handles error messages.
*/

#include "ErrorHandler.h"
#include "AelaErrorHandler.h"
#include "AelaSimpleError.h"

namespace AelaErrorHandling {
	AelaErrorHandler errorHandler;
}

void AelaErrorHandling::consoleWindowError(std::string message) {
	// This outputs an error into the seperate window that acts as a console.
	AelaSimpleError simpleError(AELA_ERROR_CONSOLE_WINDOW, message);
	errorHandler.throwError(simpleError);
}

void AelaErrorHandling::consoleWindowError(std::string title, std::string message) {
	// This outputs an error into the seperate window that acts as a console.
	AelaSimpleError simpleError(AELA_ERROR_CONSOLE_WINDOW, title, message);
	errorHandler.throwError(simpleError);
}

void AelaErrorHandling::consoleInternalError(std::string message) {
	// Implementation for an internal console within the engine/game must be added.
	AelaSimpleError simpleError(AELA_ERROR_CONSOLE_INTERNAL, message);
	errorHandler.throwError(simpleError);
}

void AelaErrorHandling::consoleInternalError(std::string title, std::string message) {
	// Implementation for an internal console within the engine/game must be added.
	AelaSimpleError simpleError(AELA_ERROR_CONSOLE_INTERNAL, title, message);
	errorHandler.throwError(simpleError);
}

void AelaErrorHandling::windowError(std::string message) {
	// This outputs an error using the operating system's error message window.
	AelaSimpleError simpleError(AELA_ERROR_WINDOW, message);
	errorHandler.throwError(simpleError);
}

void AelaErrorHandling::windowError(std::string title, std::string message) {
	// This outputs an error using the operating system's error message window.
	AelaSimpleError simpleError(AELA_ERROR_WINDOW, title, message);
	errorHandler.throwError(simpleError);
}

void AelaErrorHandling::windowWarning(std::string message) {
	// This outputs an error using the operating system's error message window.
	AelaSimpleError simpleError(AELA_WARNING_WINDOW, message);
	errorHandler.throwError(simpleError);
}

void AelaErrorHandling::windowWarning(std::string title, std::string message) {
	// This outputs an error using the operating system's error message window.
	AelaSimpleError simpleError(AELA_WARNING_WINDOW, title, message);
	errorHandler.throwError(simpleError);
}

bool AelaErrorHandling::programCloseWasRequested() {
	return errorHandler.programShouldBeClosed;
}
