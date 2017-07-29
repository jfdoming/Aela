#include <vector>
#include <stdio.h>
#include <string>
#include <cstring>
#include <fstream>
#include <iostream>

#include <glm/glm.hpp>

#include "WAVEClipLoader.h"
#include "AudioClip.h"
#include "../Error Handler/ErrorHandler.h"

using namespace Aela;

Resource* Aela::WAVEClipLoader::load(std::ifstream& in) {
	// using char[] for baseSpeed
	char header[CHUNK_HEADER_SIZE];

	// read the file header
	in.read(header, CHUNK_HEADER_SIZE);

	// make sure we are reading a RIFF file
	if (strncmp((char*) header, RIFF_CHUNK_ID, 4) != 0) {
		errorMessage = "File not in RIFF format!";
		return NULL;
	}

	char* format = header + 8;

	// make sure we are reading a WAVE file
	if (strncmp(format, WAVE_FORMAT, 4) != 0) {
		errorMessage = "File not in WAVE format!";
		return NULL;
	}

	// using char[] for baseSpeed
	char subchunk1Header[SUBCHUNK1_HEADER_SIZE];

	// read in the subchunk 1 header
	in.read(subchunk1Header, SUBCHUNK1_HEADER_SIZE);

	// make sure we are reading the format subchunk header
	if (strncmp((char*) subchunk1Header, SUBCHUNK1_ID, 4) != 0) {
		errorMessage = "Format subchunk header invalid!";
		return NULL;
	}

	unsigned int subchunk1Size = *((unsigned int*) &(subchunk1Header[4])) & 0xFFFFFFFF;

	// the subchunk 1 size is always 16 for PCM data
	if (subchunk1Size != PCM_SUBCHUNK1_SIZE) {
		errorMessage = "Compressed data is not supported!";
		return NULL;
	}

	// using char[] for baseSpeed
	char* subchunk1 = new char[subchunk1Size];

	// read in subchunk 1
	in.read(subchunk1, subchunk1Size);

	unsigned int audioFormatCode = *((unsigned int*) subchunk1) & 0xFFFF;

	// the audio format code is 1 for PCM data
	if (audioFormatCode != PCM_FORMAT_CODE) {
		errorMessage = "Compressed data is not supported!";
		return NULL;
	}

	unsigned int numberOfChannels = *((unsigned int*) &(subchunk1[2])) & 0xFFFF;
	unsigned int sampleRate = *((unsigned int*) &(subchunk1[4])) & 0xFFFFFFFF;
	unsigned int averageByteRate = *((unsigned int*) &(subchunk1[8])) & 0xFFFFFFFF;
	unsigned int dataBlockSizeInBytes = *((unsigned int*) &(subchunk1[12])) & 0xFFFF;
	unsigned int bitsPerSample = *((unsigned int*) &(subchunk1[14])) & 0xFFFF;

	// free memory used to store subchunk 1
	delete[] subchunk1;

	// using char[] for baseSpeed
	char* subchunk2Header = new char[SUBCHUNK2_HEADER_SIZE];

	// read in the subchunk 2 header
	in.read(subchunk2Header, SUBCHUNK2_HEADER_SIZE);

	// make sure we are reading the data subchunk header
	if (strncmp((char*) subchunk2Header, SUBCHUNK2_ID, 4) != 0) {
		errorMessage = "Data subchunk header invalid!";
		return NULL;
	}

	unsigned int subchunk2Size = *((unsigned int*) &(subchunk2Header[4])) & 0xFFFFFFFF;
	char* data = new char[subchunk2Size];

	// read in subchunk 2
	in.read(data, subchunk2Size);

	AudioClip* clip = new AudioClip(data);
	clip->setSize(subchunk2Size);
	clip->setFormat(numberOfChannels, bitsPerSample);
	clip->setSampleRate(sampleRate);

	std::cout << "s" << data[0] << std::endl;
	std::cout << bitsPerSample * sampleRate << std::endl;

	return clip;
}
