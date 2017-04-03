/*
* Class: LayoutManager
* Author: Julian Dominguez-Schatz
* Date: 27/03/2017
* Description: Sizes and positions components within a parent container.
*/

#pragma once
#include "Component.h"
#include <vector>

class LayoutManager {
	public:
		virtual void update(std::vector<Component*>& components) = 0;
};
