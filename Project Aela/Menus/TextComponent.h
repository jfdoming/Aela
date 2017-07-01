/*
* Class: TextComponent
* Author: Julian Dominguez-Schatz
* Date: 20/03/2017
* Description: Represents a text component in our game. It allows the user to enter text.
*/

#pragma once
#include "Component.h"

class TextComponent : public Component {
	public:
		TextComponent();
		~TextComponent();

		virtual void update();
		virtual void render();
};
