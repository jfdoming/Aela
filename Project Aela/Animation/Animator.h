/*
* Class: Project Aela's 3D Animator
* Author: Robert Ciborowski
* Date: 10/07/2017
* Description: A class used by Aela's 3D Animator to perform transformations on 3D objects.
*              Note: The position of the point used when rotating a Transformable3D using
*              PointRotation3D is relative to the Transformable3D (as opposed to being relative
*              to (0, 0, 0) in worldspace).
*/

#pragma once

#include <vector>
#include "../2D/Animation/AnimationTrack2D.h"
#include "../3D/Animation/AnimationTrack3D.h"
#include "../3D/Animation/AnimationTrackMaterial.h"
#include "../3D/Animation/AnimationTrackModel.h"
#include "../Time/Time.h"

namespace Aela {
	class Animator {
		public:
			Animator() {

			}

			// This function should be run every loop in the thread specified for animations.
			void update();

			// These are getters and setters.
			void setTime(Time* time);
			void addAnimationTrack3D(AnimationTrack3D* track);
			AnimationTrack3D* get3DTrack(std::string name);
			void addAnimationTrack2D(AnimationTrack2D* track);
			AnimationTrack2D* get2DTrack(std::string name);
			void addAnimationTrackMaterial(AnimationTrackMaterial* track);
			AnimationTrackMaterial* getMaterialTrack(std::string name);
			void addAnimationTrackModel(AnimationTrackModel* track);
			AnimationTrackModel* getModelTrack(std::string name);
			int delete3DTracksByTag(std::string tag), delete2DTracksByTag(std::string tag),
				deleteMaterialTracksByTag(std::string tag), deleteModelTracksByTag(std::string tag),
				deleteTracksByTag(std::string tag);
			bool trackWithTagExists(std::string tag);
			long long tracksWithTag(std::string tag);

			void pause3DAnimations();
			void unpause3DAnimations();
			bool is3DPaused();
			void pause2DAnimations();
			void unpause2DAnimations();
			bool is2DPaused();

		private:
			// These are the Aela objects that this class uses.
			Time* time;

			// These are basically the "tracks" in which the lists of keyframes are layed out. Note that 3D animation
			// uses a seperate track from 2D animation.
			std::vector<AnimationTrack3D> tracks3D;
			std::vector<AnimationTrackMaterial> tracksMaterial;
			std::vector<AnimationTrackModel> tracksModel;
			std::vector<AnimationTrack2D> tracks2D;

			bool paused3D = false, paused2D = false;
	};
}