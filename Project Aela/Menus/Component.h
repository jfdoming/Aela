/*
* Class: Component
* Author: Julian Dominguez-Schatz
* Date: 03/20/2017
* Description: Represents a component in our menu system. It lives inside a container.
*/

#pragma once
#include "AbstractContainer.h"

class Component {
public:
	Component();
	~Component();
protected:
	AbstractContainer* parent;
};

