#include "AnimationScript.h"
#include "ScriptObjects.h"
#include "../../../Project Aela/3D/Animation/AnimationTrackMaterial.h"
#include "../../../Project Aela/Resource Management/ResourcePaths.h"

void Scripts::loadMaterialAnimations() {
	AnimationLooper* animationLooper = engine->getAnimationLooper();
	// TODO add material animations here
}

bool Scripts::loadLogoMaterialAnimation(int whichLevel) {
	AnimationLooper* animationLooper = engine->getAnimationLooper();

	// The following code is an example for a simple water animation!
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
	}

	if (!resourceManager->obtain<Material>((std::string) DEFAULT_MATERIAL_PATH + "walls_1/3/0.png_mtl", levelLogoMaterial)) {
		AelaErrorHandling::consoleWindowError("World Manager", "Could not load a material for some tile animations: "
			+ (std::string) "walls_1/3/0.png_mtl");
		return false;
	}

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

	return true;
}
