#include "AnimationScript.h"
#include "ScriptObjects.h"
#include "../../../Project Aela/3D/Animation/AnimationTrackMaterial.h"
#include "../../../Project Aela/Resource Management/ResourcePaths.h"
#include "../../Audio/GameAudioPlayer.h"

void Scripts::loadMaterialAnimations() {
	AnimationLooper* animationLooper = engine->getAnimationLooper();
	// TODO add material animations here
}

bool Scripts::loadLogoMaterialAnimation(int whichLevel) {
	std::vector<int> framesToPauseOn;
	framesToPauseOn.push_back(0);
	framesToPauseOn.push_back(8);

	std::vector<long long> framePauseTimes = {2000, 2000};

	return loopSpriteSheetAnimation("animations_2", "walls_1/3/0.png_mtl", 8, 0, whichLevel* 2 - 2, 7, whichLevel* 2 - 1,
	                                &framesToPauseOn, &framePauseTimes, 16);

	/*AnimationLooper* animationLooper = engine->getAnimationLooper();

	std::vector<KeyFrameMaterial> levelLogoFrames(16);
	std::vector<Texture*> levelLogoTextures(16);
	Material* levelLogoMaterial;
	AnimationTrackMaterial levelLogoTrack;

	for (size_t i = 0; i < levelLogoTextures.size(); i++) {
		std::string src;
		if (i < 8) {
			src = "animations_2/" + std::to_string(i) + "/" + std::to_string(whichLevel * 2 - 2) + ".png";
		} else {
			src = "animations_2/" + std::to_string(i - 8) + "/" + std::to_string(whichLevel * 2 - 1) + ".png";
		}
		if (!resourceManager->obtain<Texture>((std::string) DEFAULT_TEXTURE_PATH + src, levelLogoTextures[i])) {
			AelaErrorHandling::consoleWindowError("World Manager", "Could not load a texture for some tile animations: " + src);
			return false;
		}
		std::cout << src << "\n";
	}

	if (!resourceManager->obtain<Material>((std::string) DEFAULT_MATERIAL_PATH + "walls_1/3/0.png_mtl", levelLogoMaterial)) {
		AelaErrorHandling::consoleWindowError("World Manager", "Could not load a material for some tile animations: "
			+ (std::string) "walls_1/3/0.png_mtl");
		return false;
	}

	std::cout << (std::string) DEFAULT_MATERIAL_PATH + "walls_1/3/0.png_mtl" << "\n";

	for (size_t i = 0; i < levelLogoFrames.size(); i++) {
		levelLogoFrames[i].setTexture(levelLogoTextures[i]);
		levelLogoFrames[i].setMaterial(levelLogoMaterial);

		if (i == 0) {
			levelLogoTrack.addKeyFrameUsingMillis(16, &levelLogoFrames[i]);
			levelLogoTrack.addKeyFrameUsingSeconds(2, &levelLogoFrames[i]);
		} else if (i == 8) {
			levelLogoTrack.addKeyFrameUsingMillis(16, &levelLogoFrames[i]);
			levelLogoTrack.addKeyFrameUsingSeconds(2, &levelLogoFrames[i]);
		} else {
			levelLogoTrack.addKeyFrameUsingMillis(16, &levelLogoFrames[i]);
		}
	}

	animationLooper->loopAnimation(&levelLogoTrack);

	return true;*/
}

bool Scripts::animateTelevision() {
	return loopSpriteSheetAnimation("animations_1", "walls_2/4/2.png_mtl", 8, 2, 3, 5, 3, 256);
}

bool Scripts::animateElevator() {
	std::vector<int> framesToPauseOn;
	framesToPauseOn.push_back(9);

	std::vector<long long> framePauseTimes = {500};
	bool returnValue = loopSpriteSheetAnimation("animations_3", "walls_1/2/2.png_mtl", 8, 0, 2, 1, 3,
	                                &framesToPauseOn, &framePauseTimes, 32);

	if (!returnValue) {
		return false;
	}

	return loopSpriteSheetAnimation("animations_3", "walls_1/3/2.png_mtl", 8, 0, 4, 1, 5,
	                                &framesToPauseOn, &framePauseTimes, 32);
}

void Scripts::stopAnimatingElevator() {
	AnimationLooper* looper = engine->getAnimationLooper();
	looper->stopLoopingMaterialTrack("walls_1/2/2.png_mtl");
	looper->stopLoopingMaterialTrack("walls_1/3/2.png_mtl");
	gameAudioPlayer->playAudio("elevator ding");
}

void Scripts::loadAelaWatchAvatarAnimations() {

}

void Scripts::setupMainAnimations() {
	loopSpriteSheetAnimation("animations_3", "walls_1/3/3.png_mtl", 8, 0, 0, 1, 0,
	                                nullptr, nullptr, 500);
	loopSpriteSheetAnimation("animations_3", "walls_1/4/3.png_mtl", 8, 2, 0, 6, 0,
	                                nullptr, nullptr, 128);
	loopSpriteSheetAnimation("animations_3", "walls_2/5/4.png_mtl", 8, 0, 1, 1, 1,
	                                nullptr, nullptr, 500);

	animateTelevision();
}

bool Scripts::loopSpriteSheetAnimation(std::string spriteSheet, std::string materialToAnimate, int spriteSheetWidth,
		int startX, int startY, int endX, int endY, std::vector<int>* framesToPauseOn,
	std::vector<long long>* framePauseTimes, long long timeBetweenFrames) {
	AnimationLooper* animationLooper = engine->getAnimationLooper();

	int numberOfFrames = spriteSheetWidth - startX + spriteSheetWidth * (endY - startY - 1) + endX + 1;

	std::vector<KeyFrameMaterial> frames(numberOfFrames);
	std::vector<Texture*> textures(numberOfFrames);
	Material* material;
	AnimationTrackMaterial track;
	track.setTag(materialToAnimate);

	int frame = 0;
	for (int y = startY; y <= endY; y++) {
		int x;
		if (y == startY) {
			x = startX;
		} else {
			x = 0;
		}

		for (x; y == endY? x <= endX : x < spriteSheetWidth; x++) {
			std::string src = spriteSheet + "/" + std::to_string(x) + "/" + std::to_string(y) + ".png";
			if (!resourceManager->obtain<Texture>((std::string) DEFAULT_TEXTURE_PATH + src, textures[frame])) {
				AelaErrorHandling::consoleWindowError("World Manager", "Could not load a texture for some tile animations: " + src);
				return false;
			}
			frame++;
		}
	}

	if (!resourceManager->obtain<Material>((std::string) DEFAULT_MATERIAL_PATH + materialToAnimate, material)) {
		AelaErrorHandling::consoleWindowError("World Manager", "Could not load a material for some tile animations: "
		+ materialToAnimate);
		return false;
	}

	for (int i = 0; i < frames.size(); i++) {
		frames[i].setTexture(textures[i]);
		frames[i].setMaterial(material);

		if (framesToPauseOn != nullptr && framePauseTimes != nullptr) {
			bool pause = false;
			size_t j;

			for (j = 0; j < framesToPauseOn->size(); j++) {
				if (framesToPauseOn->at(j) == i) {
					pause = true;
					break;
				}
			}

			if (pause) {
				track.addKeyFrameUsingMillis(timeBetweenFrames, &frames[i]);
				track.addKeyFrameUsingMillis(framePauseTimes->at(j), &frames[i]);
			} else {
				track.addKeyFrameUsingMillis(timeBetweenFrames, &frames[i]);
			}
		} else {
			track.addKeyFrameUsingMillis(timeBetweenFrames, &frames[i]);
		}
	}

	animationLooper->loopAnimation(&track);

	return true;
}

bool Scripts::loopSpriteSheetAnimation(std::string spriteSheet, std::string materialToAnimate, int spriteSheetWidth,
	int startX, int startY, int endX, int endY, long long timeBetweenFrames) {
	return loopSpriteSheetAnimation(spriteSheet, materialToAnimate, spriteSheetWidth, startX, startY, endX, endY, nullptr, nullptr, timeBetweenFrames);
}
