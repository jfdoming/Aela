#include "AnimationScript.h"
#include "ScriptObjects.h"
#include "../../../Project Aela/3D/Animation/AnimationTrackMaterial.h"
#include "../../../Project Aela/Resource Management/ResourcePaths.h"

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
	                                framesToPauseOn, framePauseTimes);

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

bool Scripts::animateElevator() {
	std::vector<int> framesToPauseOn;
	framesToPauseOn.push_back(9);

	std::vector<long long> framePauseTimes = {500};
	bool returnValue = loopSpriteSheetAnimation("animations_3", "walls_1/2/2.png_mtl", 8, 0, 2, 1, 3,
	                                framesToPauseOn, framePauseTimes);

	if (!returnValue) {
		return false;
	}

	return loopSpriteSheetAnimation("animations_3", "walls_1/3/2.png_mtl", 8, 0, 4, 1, 5,
	                                framesToPauseOn, framePauseTimes);
}

void Scripts::stopAnimatingElevator() {
	AnimationLooper* looper = engine->getAnimationLooper();
	looper->stopLoopingMaterialTrack("walls_1/2/2.png_mtl");
	looper->stopLoopingMaterialTrack("walls_1/3/2.png_mtl");
}

bool Scripts::loopSpriteSheetAnimation(std::string spriteSheet, std::string materialToAnimate, int spriteSheetWidth,
		int startX, int startY,
		int endX, int endY, std::vector<int> framesToPauseOn, std::vector<long long> framePauseTimes) {
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

		std::cout << textures[i] << " " << material << "\n";

		bool pause = false;
		size_t j;

		for (j = 0; j < framesToPauseOn.size(); j++) {
			if (framesToPauseOn[j] == i) {
				pause = true;
				break;
			}
		}

		if (pause) {
			track.addKeyFrameUsingMillis(16, &frames[i]);
			track.addKeyFrameUsingMillis(framePauseTimes[j], &frames[i]);
		} else {
			track.addKeyFrameUsingMillis(16, &frames[i]);
		}
	}

	animationLooper->loopAnimation(&track);

	return true;
}
