/*
* Class: Battle Display
* Author: Robert Ciborowski
* Date: 07/01/2019
* Description: A class used to display battles.
*/

#pragma once
#include "../../../Project Aela/Menus/Label.h"
#include "../../../Project Aela/Menus/ImageComponent.h"
#include "../../../Project Aela/Scenes/Scene.h"
#include "../../Battle/BattleNote.h"
#include "../../../Project Aela/Menus/SubMenu.h"
#include "../../Character/Character.h"
#include "../../../Project Aela/Menus/RectComponent.h"

namespace Game {
	struct BackgroundChange;
	class KeyMovement;
	class Battle;

	class BattleDisplay {
		public:
			BattleDisplay();

			void setup();
			void update();

			// This is triggered on an event.
			void onEvent(Event* event);

			void addNewNote(BattleNote* note, unsigned long long timeOfEntry, unsigned long long battleStartTime);
			void removeNote(BattleNote* note);
			void noteWasPressed(BattleNote* note);
			void tailWasPressed(BattleNote* note);
			void tailWasReleased(BattleNote* note);
			void moveKey(KeyMovement& movement);
			void showTimingScore(std::string text);
			void showTotalScore(unsigned int amount);
			void battleWasFinished(Battle* battle, unsigned int score);
			void diedDuringBattle(std::string& battleSrcToUseOnRestart, unsigned int score, std::string deathText);
			void changeBackground(BackgroundChange& backgroundChange);
			void prepareForBattle(std::string& backgroundSrc);
			void clear();
			void startShowingBattle();
			void hideBattleComponents();
			void setupKeyMovementAnimations(Battle* battle);

			void setBattleSubMenu(std::shared_ptr<SubMenu> battleSubMenu);
			void setBattleBackgrounds(std::shared_ptr<ImageComponent> battleBackground, std::shared_ptr<ImageComponent> battleBackground2);
			void setArrowKeyImages(std::shared_ptr<ImageComponent> rightKeyImage, std::shared_ptr<ImageComponent> upKeyImage,
				std::shared_ptr<ImageComponent> leftKeyImage, std::shared_ptr<ImageComponent> downKeyImage);
			void setHealthBarImage(std::shared_ptr<ImageComponent> healthBarImage);
			void setScoreLabel(std::shared_ptr<Label> scoreLabel);
			void setTimingLabel(std::shared_ptr<Label> timingLabel);
			void setEndMenuItems(std::shared_ptr<RectComponent> endRect, std::shared_ptr<Label> endTextLabel, std::shared_ptr<Label> endTextLabel2, std::shared_ptr<Label> endTextLabel3);
			void setBattleScene(Scene* battleScene);
			
		private:
			Renderer* renderer;
			Animator* animator;
			Scene* battleScene;
			Character* playerCharacter;
			AelaGame* game;

			// These are a bunch of menu components and textures.
			std::unordered_map<NoteDirection, GLTexture*> arrowTextures;
			std::unordered_map<NoteDirection, GLTexture*> tailTextures;
			std::shared_ptr<SubMenu> battleSubMenu;
			std::shared_ptr<ImageComponent> battleBackground;
			std::shared_ptr<ImageComponent> battleBackground2;
			std::unordered_map<NoteDirection, std::shared_ptr<ImageComponent>> keyImages;
			std::shared_ptr<ImageComponent> healthBarImage;
			std::shared_ptr<ImageComponent> avatarImage;
			GLTexture* rightPressedTexture, *rightReleasedTexture;
			GLTexture* upPressedTexture, *upReleasedTexture;
			GLTexture* leftPressedTexture, *leftReleasedTexture;
			GLTexture* downPressedTexture, *downReleasedTexture;
			std::shared_ptr<Label> scoreLabel;
			std::shared_ptr<Label> timingLabel;
			std::shared_ptr<RectComponent> endRect;
			std::shared_ptr<Label> endTextLabel;
			std::shared_ptr<Label> endTextLabel2;
			std::shared_ptr<Label> endTextLabel3;

			// These are a bunch of dimensions that the notes should end at.
			std::unordered_map<NoteDirection, Rect<int>> arrowEndDimensions;
			std::unordered_map<NoteDirection, Rect<int>> defaultArrowEndDimensions;

			std::unordered_map<BattleNote*, std::shared_ptr<ImageComponent>> notesOnDisplay;
			std::unordered_map<BattleNote*, std::shared_ptr<ImageComponent>> tailsOnDisplay;

			std::atomic<bool> changeToRightPressed = false, changeToRightReleased = false, changeToUpPressed = false, changeToUpReleased = false,
				changeToLeftPressed = false, changeToLeftReleased = false, changeToDownPressed = false, changeToDownReleased = false,
				pressedEnter = false;
			std::atomic<bool> playerHealthWasChanged = false;

			const std::string ANIMATION_TAG = "battle notes";
			const float SPAWN_DISTANCE_FACTOR = 1.0f;
			const float KEY_OFFSET_FACTOR = 0.083333332f;
			const float TAIL_OFFSET_FACTOR = 0.04f;
			int arrowWidthAndHeight;
			int arrowTailOffset;
			int spawnDistance;
			int keyOffset = 0;
			int distanceToTravelPastArrowKey;

			bool canPressEnterToEndBattle = false, canPressEnterToRestartBattle = false, fadeOnReturn = true;
			std::string battleSrcToUseOnRestart = "";
			std::string scriptOnReturn = "";

			void updateHealthBar();
			void removeTail(BattleNote* note);
			void setBattleBackgroundSrc(std::string src);
			void setBattleBackground2Src(std::string src);
			void resetKeyImageDimensions();
			void endAllBattleAnimations();
			void showBackgrounds();
	};
}
