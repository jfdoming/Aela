/*
* Class: AbstractLayoutManager
* Author: Julian Dominguez-Schatz
* Date: 02/04/2017
* Description: Sizes and positions components within a parent container.
*/

#pragma once
#include "Container.h"
#include "LayoutManager.h"

class AbstractLayoutManager : LayoutManager {
public:
	AbstractLayoutManager(Container* parent);
	~AbstractLayoutManager();
protected:
	Container* parent;
};
