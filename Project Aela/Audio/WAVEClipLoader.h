#pragma once

#include "../Resource Management/ResourceLoader.h"

#define CHUNK_HEADER_SIZE 12
#define SUBCHUNK1_HEADER_SIZE 8
#define SUBCHUNK2_HEADER_SIZE 8

#define RIFF_CHUNK_ID "RIFF"
#define SUBCHUNK1_ID "fmt "
#define SUBCHUNK2_ID "data"

#define WAVE_FORMAT "WAVE"

#define PCM_SUBCHUNK1_SIZE 16
#define PCM_FORMAT_CODE 1

namespace Aela {
	class WAVEClipLoader : public ResourceLoader {
	public:
		static WAVEClipLoader& getInstance() {
			// This is guaranteed to be destroyed, and instantiated on first use.
			static WAVEClipLoader instance;
			return instance;
		}

		WAVEClipLoader(WAVEClipLoader const&) = delete;
		void operator=(WAVEClipLoader const&) = delete;

		virtual Resource* load(std::ifstream& in);
	private:
		WAVEClipLoader() {}
	};
}