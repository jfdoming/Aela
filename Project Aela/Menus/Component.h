/*
* Class: Component
* Author: Julian Dominguez-Schatz
* Date: 20/03/2017
* Description: Represents a component in our menu system. It lives inside a container.
*/

#pragma once

class Component {
	public:
		Component();
		~Component();

		virtual bool isDirty();
		int getWidth();
		int getHeight();


		virtual void update() = 0;
		virtual void render() = 0;
	protected:
		// whether this component needs to be repainted
		bool dirty = true;

		// dimensions
		int width = 0;
		int height = 0;
};
