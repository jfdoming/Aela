/*
* Name: Integer Utilities
* Author: Robert Ciborowski
* Date: 30/07/2018
* Description: A file used for performing operations on any data types that store integers.
*              Maybe we should change the established naming convention for utility files
*              because "intut" sounds silly.
*/

#pragma once

// This function is used to force a value within a specific range (not clamping).
// For example, if using the range 0 and 2a, 11a / 2 is forced to 3a / 2. This
// function returns how far the value was outside of the range in terms of
// multiples of the range. The previous example will return 4.
// WARNING! Because a templated function for abs cannot exist (cause there
// would be problems with matching every single type, such as complex<double>),
// it is assumed that minimum < maximum.
template <typename T> static T forceWithinRange(T* value, T minimum, T maximum) {
	if (*value < minimum) {
		T multipleOfRange = (T) ceil(abs(((double) minimum - *value) / ((double) maximum - minimum)));
		*value += (T) ((maximum - minimum) * multipleOfRange);
		return multipleOfRange;
	}

	if (*value > maximum) {
		T multipleOfRange = (T) ceil(abs(((double) maximum - *value) / ((double) maximum - minimum)));
		*value -= (T) ((maximum - minimum) * multipleOfRange);
		return multipleOfRange;
	}

	return 0;
}