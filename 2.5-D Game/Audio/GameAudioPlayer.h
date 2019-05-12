/*
* Class: Game Audio Player
* Author: Robert Ciborowski
* Date: 28/01/2019
* Description: A class used to play game-related audio.
*/

#pragma once
#include <string>
#include <unordered_map>
#include "../../Project Aela/Audio/AudioClip.h"
#include "../../Project Aela/Resource Management/ResourceManager.h"
#include "../../Project Aela/Audio/AudioPlayer.h"
#include "../../Project Aela/Audio/AudioLooper.h"

namespace Game {
	class GameAudioPlayer {
		public:
			GameAudioPlayer();

			bool addAudioAsClip(std::string tag, std::string src);
			bool addAudioAsStream(std::string tag, std::string src);
			bool playAudio(std::string tag);
			bool playAudioIfNotPlaying(std::string tag);
			bool isPlayingClip(std::string tag);
			bool isPlayingStream(std::string tag);
			bool unloadAudio(std::string tag);
			void unloadAllClips();
			void unloadAllStreams();
			AudioClip* getClip(std::string tag);
			bool setClipVolume(std::string tag, float volume);
			bool setStreamVolume(std::string tag, float volume);
			void setAllClipVolumes(float volume);
			void setAllStreamVolumes(float volume);

			void loopClip(std::string tag, unsigned int numberOfLoops, unsigned long long locationOfLoopEnd, unsigned long long waitTimeBeforeStart);
			void loopStream(std::string tag, unsigned int numberOfLoops, unsigned long long locationOfLoopEnd, unsigned long long waitTimeBeforeStart);
			void loopClipInfinitely(std::string tag, unsigned long long locationOfLoopEnd, unsigned long long waitTimeBeforeStart);
			void loopStreamInfinitely(std::string tag, unsigned long long locationOfLoopEnd, unsigned long long waitTimeBeforeStart);
			void stopClip(std::string tag);
			void stopStream(std::string tag);
			void stopAllLoops();
			void stopEverything();

		private:
			std::unordered_map<std::string, Aela::AudioClip*> clips;

			// Once audio streams are added, this will be a map of streams. For now, we're using clips.
			std::unordered_map<std::string, Aela::AudioClip*> streams;

			ResourceManager* resourceManager;
			AudioPlayer* audioPlayer;
			AudioLooper* audioLooper;
	};
}
