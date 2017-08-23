/*
* Class: Simple 2D Framebuffer
* Author : Robert Ciborowski
* Date : 04 / 07 / 2017
* Description : A class used by Aela's 2D Renderer to represent a 2D frame buffer.
*/

#include "Simple2DFramebuffer.h"

GLuint* Simple2DFramebuffer::getFramebuffer() {
	return &framebuffer;
}

void Simple2DFramebuffer::setFramebuffer(GLuint* framebuffer) {
	this->framebuffer = *framebuffer;
}

GLuint* Simple2DFramebuffer::getMultisampledFramebuffer() {
	return &multisampledFramebuffer;
}

void Simple2DFramebuffer::setMultisampledFramebuffer(GLuint* framebuffer) {
	multisampledFramebuffer = *framebuffer;
}

Image* Simple2DFramebuffer::getFramebufferImage() {
	return &framebufferTexture;
}

Image* Simple2DFramebuffer::getMultisampledFramebufferImage() {
	return &multisampledFramebufferTexture;
}

unsigned int Simple2DFramebuffer::getMultisampling() {
	return multisampling;
}

void Simple2DFramebuffer::setMultisampling(unsigned int multisampling) {
	this->multisampling = multisampling;
}
