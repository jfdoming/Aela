#include "SoundScript.h"
#include "ScriptObjects.h"
#include "../../Audio/GameAudioPlayer.h"
#include "../../../Project Aela/Resource Management/ResourcePaths.h"
#include "../../../Project Aela/Utilities/pairut.h"
#include "../../Displays/Dialogue/DialogueDisplay.h"
#include "../../Displays/Battle Dialogue/BattleDialogueDisplay.h"
#include "../../Audio/FootstepAudioPlayer.h"

namespace Scripts {
	std::unordered_map<std::pair<int, int>, std::vector<std::string>, PairHashMapFunctions> levelMusic;
	const std::string MAIN_MENU_MUSIC = "Spacefalls";
}

void Scripts::setupSound() {
	// This sets up levelMusic so that we know which music corresponds to which level.
	levelMusic[{1, 1}].push_back("Eight Sixes");
	levelMusic[{1, 2}].push_back("Japanese Cat Superheroes");
	levelMusic[{1, 2}].push_back("Shadowed Secret");
	levelMusic[{1, 2}].push_back("New Recruit's Theme");
	levelMusic[{1, 2}].push_back("Another Day, Another Pay");
	levelMusic[{1, 2}].push_back("Death of a Laboratory");
	levelMusic[{1, 3}].push_back("Synths at an Exhibition");
	levelMusic[{1, 3}].push_back("Lost Soldier's Theme");
	levelMusic[{1, 3}].push_back("Rest In Peace, WASD");

	// Note: these sound effects were already loaded in the resource script.
	gameAudioPlayer->addAudioAsClip("big ambience", "big ambience.wav");
	gameAudioPlayer->addAudioAsClip("default footstep", "default footstep.wav");
	gameAudioPlayer->addAudioAsClip("elevator ding", "elevator ding.wav");
	gameAudioPlayer->addAudioAsClip("fade", "fade.wav");
	gameAudioPlayer->addAudioAsClip("footstep 6_1", "footstep 6_1.wav");
	gameAudioPlayer->addAudioAsClip("footstep 6_2", "footstep 6_2.wav");
	gameAudioPlayer->addAudioAsClip("footstep 7_1", "footstep 7_1.wav");
	gameAudioPlayer->addAudioAsClip("footstep 7_2", "footstep 7_2.wav");
	gameAudioPlayer->addAudioAsClip("footstep 8_1", "footstep 8_1.wav");
	gameAudioPlayer->addAudioAsClip("footstep 8_2", "footstep 8_2.wav");
	gameAudioPlayer->addAudioAsClip("footstep 9_1", "footstep 9_1.wav");
	gameAudioPlayer->addAudioAsClip("footstep 9_2", "footstep 9_2.wav");
	gameAudioPlayer->addAudioAsClip("lava", "lava.wav");
	gameAudioPlayer->addAudioAsClip("option", "option.wav");
	gameAudioPlayer->addAudioAsClip("select", "select.wav");
	gameAudioPlayer->addAudioAsClip("shoot", "shoot.wav");
	gameAudioPlayer->addAudioAsClip("speech 1", "speech 1.wav");
	gameAudioPlayer->addAudioAsClip("speech 2", "speech 2.wav");
	gameAudioPlayer->addAudioAsClip("speech 3", "speech 3.wav");
	gameAudioPlayer->addAudioAsClip("speech 4", "speech 4.wav");
	gameAudioPlayer->addAudioAsClip("speech 5", "speech 5.wav");
	gameAudioPlayer->addAudioAsClip("speech 6", "speech 6.wav");
	gameAudioPlayer->addAudioAsClip("speech 7", "speech 7.wav");
	gameAudioPlayer->addAudioAsClip("speech 8", "speech 8.wav");
	gameAudioPlayer->addAudioAsClip("speech 9", "speech 9.wav");
	gameAudioPlayer->addAudioAsClip("speech 10", "speech 10.wav");
	gameAudioPlayer->addAudioAsClip("splurt", "splurt.wav");

	footstepAudioPlayer->addSoundToTile(14, "footstep 7_1");
	footstepAudioPlayer->addSoundToTile(15, "footstep 7_1");
	footstepAudioPlayer->addSoundToTile(3, "footstep 6_1");
	footstepAudioPlayer->addSoundToTile(4, "footstep 6_1");
	footstepAudioPlayer->addSoundToTile(5, "footstep 6_1");
	footstepAudioPlayer->addSoundToTile(6, "footstep 6_1");
	footstepAudioPlayer->addSoundToTile(7, "footstep 6_1");
	footstepAudioPlayer->addSoundToTile(8, "footstep 6_1");

	footstepAudioPlayer->addSoundToTile(14, "footstep 7_2");
	footstepAudioPlayer->addSoundToTile(15, "footstep 7_2");
	footstepAudioPlayer->addSoundToTile(3, "footstep 6_2");
	footstepAudioPlayer->addSoundToTile(4, "footstep 6_2");
	footstepAudioPlayer->addSoundToTile(5, "footstep 6_2");
	footstepAudioPlayer->addSoundToTile(6, "footstep 6_2");
	footstepAudioPlayer->addSoundToTile(7, "footstep 6_2");
	footstepAudioPlayer->addSoundToTile(8, "footstep 6_2");

	footstepAudioPlayer->addSoundToTile(357, "footstep 8_1");
	footstepAudioPlayer->addSoundToTile(357, "footstep 8_2");

	footstepAudioPlayer->addSoundToTile(124, "footstep 9_1");
	footstepAudioPlayer->addSoundToTile(125, "footstep 9_1");
	footstepAudioPlayer->addSoundToTile(126, "footstep 9_1");
	footstepAudioPlayer->addSoundToTile(127, "footstep 9_1");
	footstepAudioPlayer->addSoundToTile(128, "footstep 9_1");

	footstepAudioPlayer->addSoundToTile(124, "footstep 9_2");
	footstepAudioPlayer->addSoundToTile(125, "footstep 9_2");
	footstepAudioPlayer->addSoundToTile(126, "footstep 9_2");
	footstepAudioPlayer->addSoundToTile(127, "footstep 9_2");
	footstepAudioPlayer->addSoundToTile(128, "footstep 9_2");

	dialogueDisplay->associateCharacterWithSpeechSound("???", "speech 2");
	dialogueDisplay->associateCharacterWithSpeechSound("Scientist", "speech 3");
	dialogueDisplay->associateCharacterWithSpeechSound("Brobot", "speech 4");
	dialogueDisplay->associateCharacterWithSpeechSound("Aela", "speech 5");
	dialogueDisplay->associateCharacterWithSpeechSound("Kiosk", "speech 7");
	dialogueDisplay->associateCharacterWithSpeechSound("Computer", "speech 6");
	dialogueDisplay->associateCharacterWithSpeechSound("Soldier", "speech 8");
	dialogueDisplay->associateCharacterWithSpeechSound("Drunk", "speech 10");

	battleDialogueDisplay->associateCharacterWithSpeechSound("???", "speech 2");
	battleDialogueDisplay->associateCharacterWithSpeechSound("Scientist", "speech 3");
	battleDialogueDisplay->associateCharacterWithSpeechSound("Brobot", "speech 4");
	battleDialogueDisplay->associateCharacterWithSpeechSound("Aela", "speech 5");
	battleDialogueDisplay->associateCharacterWithSpeechSound("Kiosk", "speech 7");
	battleDialogueDisplay->associateCharacterWithSpeechSound("Computer", "speech 6");
	battleDialogueDisplay->associateCharacterWithSpeechSound("Soldier", "speech 8");
	battleDialogueDisplay->associateCharacterWithSpeechSound("Drunk", "speech 10");
}

void Scripts::setupMusicForLevel(int stage, int level) {
	// This unloads all old music.
	gameAudioPlayer->unloadAllStreams();

	// This sets up the loader.
	WAVEClipLoader waveClipLoader;
	resourceManager->bindLoader(&waveClipLoader);

	// We create a group with a name based on the current time because sometimes
	// we end up loading the same music multiple times, and we don't want to add duplicates
	// to the same group.
	std::string groupName = std::to_string(stage) + "-" + std::to_string(level) + " music" + std::to_string(time->getCurrentTimeInNanos());
	resourceManager->bindGroup(groupName);

	for (auto path : levelMusic[{stage, level}]) {
		AudioClip* clip;
		if (!resourceManager->obtain<AudioClip>(DEFAULT_AUDIO_STREAM_PATH + path + ".wav", clip)) {
			resourceManager->addToGroup(DEFAULT_AUDIO_STREAM_PATH + path + ".wav", false);
		}
	}

	if (resourceManager->loadGroup(groupName) != Aela::ResourceManager::Status::OK) {
		std::cerr << "Failed to load a resource from group \"" + groupName + "\"!" << std::endl;
	}

	// This adds the audio to the GameAudioPlayer.
	for (auto path : levelMusic[{stage, level}]) {
		gameAudioPlayer->addAudioAsStream(path, path + ".wav");
	}
}

void Scripts::startPlayingMainMenuMusic() {
	// This sets up the loader.
	WAVEClipLoader waveClipLoader;
	resourceManager->bindLoader(&waveClipLoader);
	resourceManager->bindGroup("main menu music");

	AudioClip* clip;
	if (!resourceManager->obtain<AudioClip>(DEFAULT_AUDIO_STREAM_PATH + MAIN_MENU_MUSIC + ".wav", clip)) {
		resourceManager->addToGroup(DEFAULT_AUDIO_STREAM_PATH + MAIN_MENU_MUSIC + ".wav", false);
	}

	if (resourceManager->loadGroup("main menu music") != Aela::ResourceManager::Status::OK) {
		std::cerr << "Failed to load a resource from group \"main menu music\"!" << std::endl;
	}

	gameAudioPlayer->addAudioAsStream("main menu music", MAIN_MENU_MUSIC + ".wav");

	auto audioEvent = []() {
		gameAudioPlayer->playAudio("main menu music");
	};

	timer->scheduleEventInMillis("main menu music", 2500, audioEvent);
}

void Scripts::stopPlayingMainMenuMusic() {
	timer->removeEvent("main menu music");
	gameAudioPlayer->stopStream("main menu music");
	gameAudioPlayer->unloadAudio("main menu music");
}
