/*
* Class: AbstractContainer
* Author: Julian Dominguez-Schatz
* Date: 03/20/2017
* Description: Represents a container in our menu system. It lives inside a container,
               and can contain various containers, widgets and other assorted GUI elements.
*/

#pragma once

class AbstractContainer {
public:
	AbstractContainer();
	~AbstractContainer();

	virtual void update() = 0;
	virtual void render() = 0;
};

