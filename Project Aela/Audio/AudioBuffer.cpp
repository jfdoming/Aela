//
// Created by Robert on 03/09/2018.
//

#include "AudioBuffer.h"

AudioBuffer::AudioBuffer(ALuint buffer, ALuint source) {
	this->buffer = buffer;
	this->source = source;
}

ALuint AudioBuffer::getBuffer() {
	return buffer;
}

ALuint AudioBuffer::getSource() {
	return source;
}

