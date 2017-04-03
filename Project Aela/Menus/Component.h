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

		virtual void update() = 0;
		virtual void render() = 0;
	protected:
		bool dirty = true;
};
