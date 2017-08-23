/*
* Class: Resource
* Author: Julian Dominguez-Schatz
* Date: 04/07/2017
* Description: Represents an audio clip resource used in the application.
*/

#pragma once

#include "../Resource Management/Resource.h"
#include <al.h>

namespace Aela {
	class AudioClip : public Resource {
		public:
			AudioClip(std::string src, char* data);
			virtual ~AudioClip();

			const char* getData();
			ALenum getFormat();
			ALsizei getSize();
			ALsizei getSampleRate();

			void setFormat(ALenum format);
			void setFormat(int channelCount, int bitsPerSample);
			void setSize(ALsizei size);
			void setSampleRate(ALsizei sampleRate);
		private:
			const char* data;
			ALenum format;
			ALsizei size;
			ALsizei sampleRate;
	};
}