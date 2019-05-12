/*
* Class: Personality
* Author: Robert Ciborowski
* Date: 27/09/2018
* Description: A class used to represent a character personality.
*/

#pragma once
#include <string>

namespace Game {
	enum class PersonalityTrait {
		INTROVERTED, EXTROVERTED, SENSING, INTUITION, THINKING, FEELING, JUDGING, PROSPECTING
	};

	class Personality {
		public:
			Personality();
			Personality(std::string personalityAsString);

			void setTrait(PersonalityTrait trait);
			bool hasTrait(PersonalityTrait trait);
			std::string getPersonalityAsString();

		private:
			std::string personalityAsString;

			bool checkPersonalityValidity(std::string personalityAsString);
	};
}
