#include "BattleDisplay.h"
#include "../../Game Object Provider/GameObjectProvider.h"
#include "../../Scripts/Scripts to Move to LUA/SceneIDs.h"
#include "../../../Project Aela/Utilities/enumut.h"
#include "../../Character/CharacterHealthEvent.h"
#include "../../Aela Game/AelaGame.h"
#include "../../Constants/WindowConstants.h"
#include "../../Menus/FontSizes.h"
#include "../../Battle/Battle.h"
#include "../../Scripts/ScriptManager.h"
#include "../../../Project Aela/Resource Management/ResourcePaths.h"

Game::BattleDisplay::BattleDisplay() {
}

void Game::BattleDisplay::setup() {
	renderer = GameObjectProvider::getRenderer();
	animator = GameObjectProvider::getAnimator();
	playerCharacter = GameObjectProvider::getPlayer()->getCharacter();
	game = GameObjectProvider::getGame();
	GameObjectProvider::getEventHandler()->addListener(EventConstants::KEY_PRESSED, bindListener(BattleDisplay::onEvent, this));
	GameObjectProvider::getEventHandler()->addListener(EventConstants::KEY_RELEASED, bindListener(BattleDisplay::onEvent, this));
	GameObjectProvider::getEventHandler()->addListener(CHARACTER_HEALTH, bindListener(BattleDisplay::onEvent, this));

	ResourceManager* resourceManager = GameObjectProvider::getResourceManager();
	GLTexture* texture;
	bool success = resourceManager->obtain<GLTexture>("res/textures/battle/0/0.png", rightReleasedTexture);
	if (success) {
		resourceManager->obtain<GLTexture>("res/textures/battle/1/0.png", rightPressedTexture);
		resourceManager->obtain<GLTexture>("res/textures/battle/2/0.png", upReleasedTexture);
		resourceManager->obtain<GLTexture>("res/textures/battle/3/0.png", upPressedTexture);
		resourceManager->obtain<GLTexture>("res/textures/battle/4/0.png", leftReleasedTexture);
		resourceManager->obtain<GLTexture>("res/textures/battle/5/0.png", leftPressedTexture);
		resourceManager->obtain<GLTexture>("res/textures/battle/6/0.png", downReleasedTexture);
		resourceManager->obtain<GLTexture>("res/textures/battle/7/0.png", downPressedTexture);
		resourceManager->obtain<GLTexture>("res/textures/battle/0/1.png", texture);
		arrowTextures[NoteDirection::RIGHT] = texture;
		resourceManager->obtain<GLTexture>("res/textures/battle/1/1.png", texture);
		arrowTextures[NoteDirection::UP] = texture;
		resourceManager->obtain<GLTexture>("res/textures/battle/2/1.png", texture);
		arrowTextures[NoteDirection::LEFT] = texture;
		resourceManager->obtain<GLTexture>("res/textures/battle/3/1.png", texture);
		arrowTextures[NoteDirection::DOWN] = texture;
		resourceManager->obtain<GLTexture>("res/textures/battle/0/2.png", texture);
		tailTextures[NoteDirection::RIGHT] = texture;
		resourceManager->obtain<GLTexture>("res/textures/battle/1/2.png", texture);
		tailTextures[NoteDirection::UP] = texture;
		resourceManager->obtain<GLTexture>("res/textures/battle/2/2.png", texture);
		tailTextures[NoteDirection::LEFT] = texture;
		resourceManager->obtain<GLTexture>("res/textures/battle/3/2.png", texture);
		tailTextures[NoteDirection::DOWN] = texture;
	} else {
		AelaErrorHandling::consoleWindowError("Battle Display", "Missing all textures from: ../../res/textures/battle.png");
	}

	Rect<int> renderingDimensions(0, 0, DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT);
	distanceToTravelPastArrowKey = renderingDimensions.getHeight() / 24;
	int keyWidthAndHeight = (int) (renderingDimensions.getHeight() / 12);
	defaultArrowEndDimensions[NoteDirection::RIGHT] = Rect<int>((int) (renderingDimensions.getWidth() * 0.53125), (int) (renderingDimensions.getHeight() * 0.625),
			 keyWidthAndHeight, keyWidthAndHeight);
	defaultArrowEndDimensions[NoteDirection::UP] = Rect<int>((int) (renderingDimensions.getWidth() * 0.46875), (int) (renderingDimensions.getHeight() * 0.541666),
			 keyWidthAndHeight, keyWidthAndHeight);
	defaultArrowEndDimensions[NoteDirection::LEFT] = Rect<int>((int) (renderingDimensions.getWidth() * 0.40625), (int) (renderingDimensions.getHeight() * 0.625),
			 keyWidthAndHeight, keyWidthAndHeight);
	defaultArrowEndDimensions[NoteDirection::DOWN] = Rect<int>((int) (renderingDimensions.getWidth() * 0.46875), (int) (renderingDimensions.getHeight() * 0.625),
			 keyWidthAndHeight, keyWidthAndHeight);
	arrowEndDimensions = defaultArrowEndDimensions;
	arrowWidthAndHeight = renderingDimensions.getHeight() / 12;
	spawnDistance = (int) (renderingDimensions.getHeight() * SPAWN_DISTANCE_FACTOR);
	arrowTailOffset = (int) (renderingDimensions.getHeight() * TAIL_OFFSET_FACTOR);
	keyOffset = (int) (renderingDimensions.getHeight() * KEY_OFFSET_FACTOR);
}

void Game::BattleDisplay::update() {
	if (changeToRightPressed) {
		keyImages[NoteDirection::RIGHT]->setTexture(rightPressedTexture);
		changeToRightPressed = false;
	}

	if (changeToRightReleased) {
		keyImages[NoteDirection::RIGHT]->setTexture(rightReleasedTexture);
		changeToRightReleased = false;
	}

	if (changeToUpPressed) {
		keyImages[NoteDirection::UP]->setTexture(upPressedTexture);
		changeToUpPressed = false;
	}

	if (changeToUpReleased) {
		keyImages[NoteDirection::UP]->setTexture(upReleasedTexture);
		changeToUpReleased = false;
	}

	if (changeToLeftPressed) {
		keyImages[NoteDirection::LEFT]->setTexture(leftPressedTexture);
		changeToLeftPressed = false;
	}

	if (changeToLeftReleased) {
		keyImages[NoteDirection::LEFT]->setTexture(leftReleasedTexture);
		changeToLeftReleased = false;
	}

	if (changeToDownPressed) {
		keyImages[NoteDirection::DOWN]->setTexture(downPressedTexture);
		changeToDownPressed = false;
	}

	if (changeToDownReleased) {
		keyImages[NoteDirection::DOWN]->setTexture(downReleasedTexture);
		changeToDownReleased = false;
	}

	if (pressedEnter) {
		if (canPressEnterToEndBattle) {
			hideBattleComponents();
			endTextLabel->hide();
			endTextLabel2->hide();
			endTextLabel3->hide();
			endRect->hide();

			if (fadeOnReturn) {
				game->fadeIn(700);
			}

			game->sceneWasSwitched(WORLD_GAMEPLAY_SCENE);
			GameObjectProvider::getSceneManager()->setCurrentScene(WORLD_GAMEPLAY_SCENE);
			GameObjectProvider::getPlayer()->getCharacter()->allowNewMovements(false);
			canPressEnterToEndBattle = false;

			auto event = [this]() {
				GameObjectProvider::getScriptManager()->runScript(scriptOnReturn);
				GameObjectProvider::getPlayer()->getCharacter()->allowNewMovements(true);
			};
			GameObjectProvider::getTimer()->scheduleEventInMillis(900, event);
		}

		if (canPressEnterToRestartBattle) {
			clear();
			canPressEnterToRestartBattle = false;

			auto event = [this]() {
				GameObjectProvider::getGame()->startBattle(battleSrcToUseOnRestart);
			};

			GameObjectProvider::getTimer()->scheduleEventInMillis(1000, event);
		}

		pressedEnter = false;
	}

	if (playerHealthWasChanged) {
		updateHealthBar();
		playerHealthWasChanged = false;
	}
}

void Game::BattleDisplay::setArrowKeyImages(std::shared_ptr<ImageComponent> rightKeyImage, std::shared_ptr<ImageComponent> upKeyImage,
	std::shared_ptr<ImageComponent> leftKeyImage, std::shared_ptr<ImageComponent> downKeyImage) {
	keyImages[NoteDirection::RIGHT] = rightKeyImage;
	keyImages[NoteDirection::UP] = upKeyImage;
	keyImages[NoteDirection::LEFT] = leftKeyImage;
	keyImages[NoteDirection::DOWN] = downKeyImage;
}

void Game::BattleDisplay::setHealthBarImage(std::shared_ptr<ImageComponent> healthBarImage) {
	this->healthBarImage = healthBarImage;
}

void Game::BattleDisplay::setScoreLabel(std::shared_ptr<Label> scoreLabel) {
	this->scoreLabel = scoreLabel;
}

void Game::BattleDisplay::setTimingLabel(std::shared_ptr<Label> timingLabel) {
	this->timingLabel = timingLabel;
}

void Game::BattleDisplay::setEndMenuItems(std::shared_ptr<RectComponent> endRect, std::shared_ptr<Label> endTextLabel,
	std::shared_ptr<Label> endTextLabel2, std::shared_ptr<Label> endTextLabel3) {
	this->endRect = endRect;
	this->endTextLabel = endTextLabel;
	this->endTextLabel2 = endTextLabel2;
	this->endTextLabel3 = endTextLabel3;
}

void Game::BattleDisplay::setBattleScene(Scene* battleScene) {
	this->battleScene = battleScene;
}

void Game::BattleDisplay::showBackgrounds() {
	battleBackground->show();
	battleBackground2->show();
}

void Game::BattleDisplay::updateHealthBar() {
	healthBarImage->getDimensions()->setWidth((int) (DEFAULT_WINDOW_WIDTH / 4 * playerCharacter->getHealthLeftAsFraction()));
}

void Game::BattleDisplay::removeTail(BattleNote* note) {
	auto pair = tailsOnDisplay.find(note);

	if (pair == tailsOnDisplay.end()) {
		return;
	}

	battleScene->getMenu()->remove(pair->second);
	tailsOnDisplay.erase(pair->first);
}

void Game::BattleDisplay::setBattleBackgroundSrc(std::string src) {
	ResourceManager* resourceManager = GameObjectProvider::getResourceManager();
	bool isUsingRoot = resourceManager->isResourceRootEnabled();
	resourceManager->useResourceRoot(false);
	GLTexture* texture;
	bool success = resourceManager->obtain<GLTexture>(src, texture);

	if (success) {
		battleBackground->setTexture(texture);
		battleBackground->setCropping(texture->getDimensions());
	} else {
		AelaErrorHandling::consoleWindowError(src);
	}

	resourceManager->useResourceRoot(isUsingRoot);
}

void Game::BattleDisplay::setBattleBackground2Src(std::string src) {
	GLTexture* texture;
	bool success = GameObjectProvider::getResourceManager()->obtain<GLTexture>(DEFAULT_TEXTURE_PATH + src, texture);
	if (success) {
		battleBackground2->setTexture(texture);
		battleBackground2->setCropping(texture->getDimensions());
	} else {
		AelaErrorHandling::consoleWindowError(DEFAULT_TEXTURE_PATH + src);
	}
}

void Game::BattleDisplay::resetKeyImageDimensions() {
	Rect<int> renderingDimensions(0, 0, DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT);
	int keyWidthAndHeight = (int) (renderingDimensions.getHeight() / 12);
	keyImages[NoteDirection::RIGHT]->setDimensions(&Rect<int>((int) (renderingDimensions.getWidth() * 0.53125), (int) (renderingDimensions.getHeight() * 0.625),
		 keyWidthAndHeight, keyWidthAndHeight));
	keyImages[NoteDirection::UP]->setDimensions(&Rect<int>((int) (renderingDimensions.getWidth() * 0.46875), (int) (renderingDimensions.getHeight() * 0.541666),
		 keyWidthAndHeight, keyWidthAndHeight));
	keyImages[NoteDirection::LEFT]->setDimensions(&Rect<int>((int) (renderingDimensions.getWidth() * 0.40625), (int) (renderingDimensions.getHeight() * 0.625),
		 keyWidthAndHeight, keyWidthAndHeight));
	keyImages[NoteDirection::DOWN]->setDimensions(&Rect<int>((int) (renderingDimensions.getWidth() * 0.46875), (int) (renderingDimensions.getHeight() * 0.625),
		 keyWidthAndHeight, keyWidthAndHeight));
}

void Game::BattleDisplay::endAllBattleAnimations() {
	animator->delete2DTracksByTag(ANIMATION_TAG);
	animator->delete2DTracksByTag("key_movement_0");
	animator->delete2DTracksByTag("key_movement_1");
	animator->delete2DTracksByTag("key_movement_2");
	animator->delete2DTracksByTag("key_movement_3");
}

void Game::BattleDisplay::onEvent(Event* event) {
	if (GameObjectProvider::getSceneManager()->getCurrentSceneId() == BATTLE_SCENE) {
		if (event->getType() == EventConstants::KEY_PRESSED) {
			auto* keyEvent = dynamic_cast<KeyEvent*>(event);
			switch (keyEvent->getKeycode()) {
				case SDLK_d:
					changeToRightPressed = true;
					break;
				case SDLK_w:
					changeToUpPressed = true;
					break;
				case SDLK_a:
					changeToLeftPressed = true;
					break;
				case SDLK_s:
					changeToDownPressed = true;
					break;
			}
		} else if (event->getType() == EventConstants::KEY_RELEASED) {
			auto* keyEvent = dynamic_cast<KeyEvent*>(event);
			switch (keyEvent->getKeycode()) {
				case SDLK_d:
					changeToRightReleased = true;
					break;
				case SDLK_w:
					changeToUpReleased = true;
					break;
				case SDLK_a:
					changeToLeftReleased = true;
					break;
				case SDLK_s:
					changeToDownReleased = true;
					break;
				case SDLK_RETURN:
					pressedEnter = true;
					break;
			}
		} else if (CharacterHealthEvent* healthEvent = dynamic_cast<CharacterHealthEvent*>(event)) {
			if (healthEvent->getCharacter() == playerCharacter) {
				playerHealthWasChanged = true;
			}
		}
	}
}

void Game::BattleDisplay::addNewNote(BattleNote* note, unsigned long long timeOfEntry, unsigned long long battleStartTime) {
	NoteDirection direction = note->getDirection();
	Rect<int> endDimension;
	AnimationTrack2D noteHeadTrack;
	KeyFrame2D noteHeadFrame;

	// If the key is moving on the screen, we need to figure out what position it will be in when the note hits.
	AnimationTrack2D* keyMovementTrack = animator->get2DTrack("key_movement_" + std::to_string(enumToInteger(direction)));
	if (keyMovementTrack != nullptr) {
		long long timeUntilNoteHit = (long long) note->getTiming() * 1000000;
		long long timeUntilKeyFrame;
		size_t keyFramePosition;
		KeyFrame2D* keyFrame;
		keyMovementTrack->getKeyFrameInfoAtTime(timeUntilNoteHit, &timeUntilKeyFrame, &keyFrame, &keyFramePosition);

		if (keyFrame == nullptr) {
			noteHeadFrame.setDimensions(&arrowEndDimensions[direction]);
			endDimension = arrowEndDimensions[direction];
		} else {
			long long timeUntilPreviousKeyFrame = 0;

			if (keyFramePosition != 0) {
				timeUntilPreviousKeyFrame = keyMovementTrack->getTimeUntilKeyFrame(&keyMovementTrack->getKeyFrames()->at(keyFramePosition - 1).second);
			}

			long long pointInFrame = timeUntilNoteHit - timeUntilPreviousKeyFrame;

			endDimension = *keyFrame->getDimensionsAtTime2(pointInFrame, timeUntilKeyFrame - timeUntilPreviousKeyFrame);
			noteHeadFrame.setDimensions(&endDimension);
		}
	} else {
		noteHeadFrame.setDimensions(&arrowEndDimensions[direction]);
		endDimension = arrowEndDimensions[direction];
	}

	if (note->getTailLength() != 0) {
		auto tailImage = std::make_shared<ImageComponent>();
		Rect<int> startDimensions, endDimensions, endDimensions2;
		int tailLength = (int) ((note->getTailLength()) * spawnDistance / note->getTiming());

		tailImage->setTexture(tailTextures[note->getDirection()]);
		battleScene->getMenu()->add(tailImage);

		AnimationTrack2D noteTailTrack;
		noteTailTrack.setTag(ANIMATION_TAG);
		KeyFrame2D noteTailFrame;
		noteTailFrame.setObject(tailImage);
		KeyFrame2D noteTailFrame2;
		noteTailFrame2.setObject(tailImage);

		auto onEnd = [this, note]() {
			removeTail(note);
		};

		noteTailFrame2.setEndingAction(onEnd);

		switch (direction) {
			case NoteDirection::RIGHT:
				startDimensions = endDimension + Rect<int>(spawnDistance + arrowTailOffset, 0, 0, 0);
				startDimensions.setWidth(tailLength - arrowTailOffset);
				endDimensions = endDimension + Rect<int>(arrowTailOffset, 0, 0, 0);
				endDimensions.setWidth(tailLength - arrowTailOffset);
				endDimensions2 = endDimension;// + Rect<int>(arrowTailOffset, 0, 0, 0);
				endDimensions2.setWidth(0);
				break;
			case NoteDirection::UP:
				startDimensions = endDimension - Rect<int>(0, spawnDistance + tailLength - keyOffset, 0, 0);
				startDimensions.setHeight(tailLength - arrowTailOffset);
				endDimensions = endDimension - Rect<int>(0, tailLength - keyOffset, 0, 0);
				endDimensions.setHeight(tailLength - arrowTailOffset);
				endDimensions2 = endDimension + Rect<int>(0, keyOffset, 0, 0);
				endDimensions2.setHeight(0);
				break;
			case NoteDirection::LEFT:
				startDimensions = endDimension - Rect<int>(spawnDistance + tailLength - keyOffset, 0, 0, 0);
				startDimensions.setWidth(tailLength - arrowTailOffset);
				endDimensions = endDimension - Rect<int>(tailLength - keyOffset, 0, 0, 0);
				endDimensions.setWidth(tailLength - arrowTailOffset);
				endDimensions2 = endDimension + Rect<int>(keyOffset, 0, 0, 0);
				endDimensions2.setWidth(0);
				break;
			case NoteDirection::DOWN:
				startDimensions = endDimension + Rect<int>(0, spawnDistance + arrowTailOffset, 0, 0);
				startDimensions.setHeight(tailLength - arrowTailOffset);
				endDimensions = endDimension + Rect<int>(0, arrowTailOffset, 0, 0);
				endDimensions.setHeight(tailLength - arrowTailOffset);
				endDimensions2 = endDimension;// + Rect<int>(0, arrowTailOffset, 0, 0);
				endDimensions2.setHeight(0);
				break;
			default:
				break;
		}

		tailImage->setDimensions(&startDimensions);
		tailsOnDisplay[note] = tailImage;
		noteTailFrame.setDimensions(&endDimensions);
		noteTailFrame2.setDimensions(&endDimensions2);
		
		noteTailTrack.addKeyFrameUsingMillis(note->getTiming(), &noteTailFrame);
		noteTailTrack.addKeyFrameUsingMillis(note->getTailLength() /*- arrowTailOffset * spawnDistance / note->getTiming()*/, &noteTailFrame2);
		noteTailTrack.updatePositionInTrack(GameObjectProvider::getTime()->getCurrentTimeInNanos() - battleStartTime - timeOfEntry);
		animator->addAnimationTrack2D(&noteTailTrack);
	}

	// This is basically a hack in order to get the arrows to be dislayed below some of the other menu components.
	// The submenu is added back at the end.
	battleScene->getMenu()->remove(battleSubMenu);

	auto noteImage = std::make_shared<ImageComponent>();

	// Note: the random direction will never come from note.getDirection() since the random direction was already replaced
	// with another direction, which was randomly obtained.
	noteImage->setTexture(arrowTextures[note->getDirection()]);

	Rect<int> startDimensions;

	battleScene->getMenu()->add(noteImage);


	noteHeadTrack.setTag(ANIMATION_TAG);
	noteHeadFrame.setObject(noteImage);

	auto onEnd = [this, noteImage, note]() {
		noteImage->hide();
	};

	noteHeadFrame.setEndingAction(onEnd);

	

	switch (direction) {
		case NoteDirection::RIGHT:
			startDimensions = endDimension + Rect<int>(spawnDistance, 0, 0, 0);
			noteImage->setDimensions(&startDimensions);
			break;
		case NoteDirection::UP:
			startDimensions = endDimension - Rect<int>(0, spawnDistance, 0, 0);
			noteImage->setDimensions(&startDimensions);
			break;
		case NoteDirection::LEFT:
			startDimensions = endDimension - Rect<int>(spawnDistance, 0, 0, 0);
			noteImage->setDimensions(&startDimensions);
			break;
		case NoteDirection::DOWN:
			startDimensions = endDimension + Rect<int>(0, spawnDistance, 0, 0);
			noteImage->setDimensions(&startDimensions);
			break;
		default:
			break;
	}

	noteHeadTrack.addKeyFrameUsingMillis(note->getTiming(), &noteHeadFrame);
	noteHeadTrack.updatePositionInTrack(GameObjectProvider::getTime()->getCurrentTimeInNanos() - battleStartTime - timeOfEntry);
	animator->addAnimationTrack2D(&noteHeadTrack);

	notesOnDisplay[note] = noteImage;
	battleScene->getMenu()->add(battleSubMenu);
}

void Game::BattleDisplay::removeNote(BattleNote* note) {
	auto pair = notesOnDisplay.find(note);

	if (pair == notesOnDisplay.end()) {
		return;
	}

	battleScene->getMenu()->remove(pair->second);
	notesOnDisplay.erase(pair->first);
}

void Game::BattleDisplay::noteWasPressed(BattleNote* note) {
	auto pair = notesOnDisplay.find(note);

	if (pair == notesOnDisplay.end()) {
		return;
	}

	pair->second->setTint(&ColourRGBA(0.7f, 0.7f, 0.7f, 1));

	if (note->getTailLength() == 0) {
		return;
	}

	auto pair2 = tailsOnDisplay.find(note);

	if (pair2 == tailsOnDisplay.end()) {
		return;
	}

	pair2->second->setTint(&ColourRGBA(0.7f, 0.7f, 0.7f, 1));
}

void Game::BattleDisplay::tailWasPressed(BattleNote* note) {
	auto pair = tailsOnDisplay.find(note);
	if (pair != tailsOnDisplay.end()) {
		pair->second->setTint(&ColourRGBA(0.7f, 0.7f, 0.7f, 1));
	}
}

void Game::BattleDisplay::tailWasReleased(BattleNote* note) {
	auto pair = tailsOnDisplay.find(note);
	if (pair != tailsOnDisplay.end()) {
		pair->second->setTint(&ColourRGBA(1));
	}
}

void Game::BattleDisplay::moveKey(KeyMovement& movement) {
	Rect<int> renderingDimensions(0, 0, DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT);
	NoteDirection direction = movement.getDirection();
	AnimationTrack2D track;
	KeyFrame2D frame;
	frame.setObject(keyImages[direction]);
	auto* dimensions = keyImages[direction]->getDimensions();
	arrowEndDimensions[direction] = Rect<int>((int) (movement.getX() * renderingDimensions.getWidth()), (int) (movement.getY() * renderingDimensions.getHeight()), dimensions->getWidth(), dimensions->getHeight());
	frame.setDimensions(&arrowEndDimensions[direction]);
	frame.setCurvatureToMovement(movement.getCurvature());
	track.addKeyFrameUsingMillis(movement.getDuration(), &frame);
	track.setTag("key_movement_" + std::to_string(enumToInteger(direction)));
	animator->addAnimationTrack2D(&track);
}

void Game::BattleDisplay::showTimingScore(std::string text) {
	timingLabel->setText(text);
}

void Game::BattleDisplay::showTotalScore(unsigned int amount) {
	scoreLabel->setText("Score: " + std::to_string(amount));
}

void Game::BattleDisplay::battleWasFinished(Battle* battle, unsigned int score) {
	endAllBattleAnimations();

	scriptOnReturn = battle->getScriptOnReturnToGameplay();
	fadeOnReturn = battle->getFadeOnReturn();
	ColourRGBA transparentBlack(0.0f);
	ColourRGBA white(1, 1, 1, 1);

	endRect->setTint(&transparentBlack);
	endRect->setColour(&ColourRGBA(0, 0, 0, 0.95f));
	endRect->show();

	AnimationTrack2D track;
	KeyFrame2D frame;
	frame.setObject(endRect);
	frame.setTint(&white);
	track.addKeyFrameUsingMillis(1000, &frame);
	animator->addAnimationTrack2D(&track);

	endTextLabel->setTint(&transparentBlack);
	endTextLabel->show();
	endTextLabel->setSize(FontSizes::MEDIUM_FONT_SIZE);

	AnimationTrack2D track2;
	KeyFrame2D frame2;
	frame2.setObject(endTextLabel);
	frame2.setTint(&white);
	track2.addKeyFrameUsingMillis(1000, &frame2);
	animator->addAnimationTrack2D(&track2);

	// endTextLabel2->setTint(&transparentBlack);
	endTextLabel2->show();

	/*AnimationTrack2D track3;
	KeyFrame2D frame3;
	frame3.setObject(endTextLabel2);
	frame3.setTint(&white);
	track3.addKeyFrameUsingMillis(1000, &frame3);
	animator->addAnimationTrack2D(&track3);*/

	endTextLabel->setText(battle->getTextOnEnd());
	endTextLabel2->setText("");
	endTextLabel3->setText("Score: " + std::to_string(score));

	auto event = [this]() {
		endTextLabel2->setText("Press ENTER to continue.");
		canPressEnterToEndBattle = true;
	};

	GameObjectProvider::getTimer()->scheduleEventInMillis(2500, event);

	endTextLabel3->setTint(&transparentBlack);
	endTextLabel2->show();

	endTextLabel3->setTint(&transparentBlack);
	endTextLabel3->show();

	AnimationTrack2D track4;
	KeyFrame2D frame4;
	frame4.setObject(endTextLabel3);
	frame4.setTint(&white);
	track4.addKeyFrameUsingMillis(1000, &frame4);
	animator->addAnimationTrack2D(&track4);
}

void Game::BattleDisplay::diedDuringBattle(std::string& battleSrcToUseOnRestart, unsigned int score, std::string deathText) {
	this->battleSrcToUseOnRestart = battleSrcToUseOnRestart;
	endAllBattleAnimations();

	ColourRGBA transparentBlack(0.0f);
	ColourRGBA white(1, 1, 1, 1);

	endRect->setTint(&transparentBlack);
	endRect->setColour(&ColourRGBA(0.225f, 0.15f, 0.15f, 0.95f));
	endTextLabel->setSize(FontSizes::LARGE_FONT_SIZE);
	endRect->show();

	AnimationTrack2D track;
	KeyFrame2D frame;
	frame.setObject(endRect);
	frame.setTint(&white);
	track.addKeyFrameUsingMillis(1000, &frame);
	animator->addAnimationTrack2D(&track);

	endTextLabel->setTint(&transparentBlack);
	endTextLabel->show();

	AnimationTrack2D track2;
	KeyFrame2D frame2;
	frame2.setObject(endTextLabel);
	frame2.setTint(&white);
	track2.addKeyFrameUsingMillis(1000, &frame2);
	animator->addAnimationTrack2D(&track2);

	// endTextLabel2->setTint(&transparentBlack);
	endTextLabel2->show();

	/*AnimationTrack2D track3;
	KeyFrame2D frame3;
	frame3.setObject(endTextLabel2);
	frame3.setTint(&white);
	track3.addKeyFrameUsingMillis(1000, &frame3);
	animator->addAnimationTrack2D(&track3);*/

	endTextLabel->setText(deathText);
	endTextLabel2->setText("");
	endTextLabel3->setText("Score: " + std::to_string(score));

	auto event = [this]() {
		endTextLabel2->setText("Press ENTER to restart the battle.");
		canPressEnterToRestartBattle = true;
	};

	GameObjectProvider::getTimer()->scheduleEventInMillis(2500, event);

	endTextLabel3->setTint(&transparentBlack);
	endTextLabel3->show();

	AnimationTrack2D track4;
	KeyFrame2D frame4;
	frame4.setObject(endTextLabel3);
	frame4.setTint(&white);
	track4.addKeyFrameUsingMillis(1000, &frame4);
	animator->addAnimationTrack2D(&track4);
}

void Game::BattleDisplay::changeBackground(BackgroundChange& backgroundChange) {
	std::string oldSrc = battleBackground2->getTexture()->getSrc();
	setBattleBackground2Src(backgroundChange.src);
	setBattleBackgroundSrc(oldSrc);
	battleBackground2->setTint(&ColourRGBA(1, 1, 1, 0));

	AnimationTrack2D track;
	KeyFrame2D frame;
	frame.setObject(battleBackground2);
	frame.setTint(&ColourRGBA(1, 1, 1, 1));
	track.addKeyFrameUsingMillis(backgroundChange.duration, &frame);
	animator->addAnimationTrack2D(&track);
}

void Game::BattleDisplay::prepareForBattle(std::string& backgroundSrc) {
	setBattleBackground2Src(backgroundSrc);
	battleSubMenu->hide();
	endTextLabel->hide();
	endTextLabel2->hide();
	endTextLabel3->hide();
	battleBackground->hide();
	battleBackground2->hide();
	timingLabel->setText("");
	battleScene->getMenu()->clear();
	updateHealthBar();
	arrowEndDimensions = defaultArrowEndDimensions;
	resetKeyImageDimensions();

	battleScene->getMenu()->add(battleBackground);
	battleScene->getMenu()->add(battleBackground2);
	battleScene->getMenu()->add(battleSubMenu);
}

void Game::BattleDisplay::clear() {
	battleSubMenu->hide();
	endTextLabel->hide();
	endTextLabel2->hide();
	endTextLabel3->hide();
	battleBackground->hide();
	battleBackground2->hide();
	timingLabel->setText("");
	battleScene->getMenu()->clear();
	arrowEndDimensions = defaultArrowEndDimensions;

	battleScene->getMenu()->add(battleBackground);
	battleScene->getMenu()->add(battleBackground2);
	battleScene->getMenu()->add(battleSubMenu);
}

void Game::BattleDisplay::startShowingBattle() {
	battleSubMenu->show();
	showBackgrounds();

	ColourRGBA black(0.0f);
	ColourRGBA white(1);

	endRect->show();
	endRect->setTint(&white);
	endRect->setColour(&ColourRGBA(0, 0, 0, 1));

	AnimationTrack2D rectTrack2D;
	KeyFrame2D rectFrame;
	rectFrame.setObject(endRect);
	rectFrame.setTint(&black);
	rectTrack2D.addKeyFrameUsingMillis(700, &rectFrame);
	animator->addAnimationTrack2D(&rectTrack2D);
}

void Game::BattleDisplay::hideBattleComponents() {
	battleSubMenu->hide();
}

void Game::BattleDisplay::setupKeyMovementAnimations(Battle* battle) {
	auto movements = battle->getKeyMovements();
	Animator* animator = GameObjectProvider::getAnimator();

	std::unordered_map<NoteDirection, AnimationTrack2D> tracks;
	tracks[NoteDirection::RIGHT] = AnimationTrack2D();
	tracks[NoteDirection::UP] = AnimationTrack2D();
	tracks[NoteDirection::LEFT] = AnimationTrack2D();
	tracks[NoteDirection::DOWN] = AnimationTrack2D();
	tracks[NoteDirection::RIGHT].setTag("key_movement_0");
	tracks[NoteDirection::UP].setTag("key_movement_1");
	tracks[NoteDirection::LEFT].setTag("key_movement_2");
	tracks[NoteDirection::DOWN].setTag("key_movement_3");

	std::unordered_map<NoteDirection, Rect<int>> previousDimensions;
	previousDimensions[NoteDirection::RIGHT] = *keyImages[NoteDirection::RIGHT]->getDimensions();
	previousDimensions[NoteDirection::UP] = *keyImages[NoteDirection::UP]->getDimensions();
	previousDimensions[NoteDirection::LEFT] = *keyImages[NoteDirection::LEFT]->getDimensions();
	previousDimensions[NoteDirection::DOWN] = *keyImages[NoteDirection::DOWN]->getDimensions();

	std::unordered_map<NoteDirection, unsigned long long> previousTimes;
	previousTimes[NoteDirection::RIGHT] = 0;
	previousTimes[NoteDirection::UP] = 0;
	previousTimes[NoteDirection::LEFT] = 0;
	previousTimes[NoteDirection::DOWN] = 0;

	int width = keyImages[NoteDirection::RIGHT]->getDimensions()->getWidth();
	int height = keyImages[NoteDirection::RIGHT]->getDimensions()->getHeight();

	for (auto pair : *movements) {
		KeyFrame2D frame;
		NoteDirection direction = pair.second.getDirection();
		unsigned long long duration = pair.second.getDuration() * 1000000;
		frame.setObject(keyImages[direction]);
		frame.setDimensions(&previousDimensions[direction]);
		tracks[direction].addKeyFrame(pair.first - previousTimes[direction], &frame);
		previousTimes[direction] = pair.first + duration;

		// I could optimize this by reusing frame2 during the next movement of the key.
		KeyFrame2D frame2;
		frame2.setObject(keyImages[direction]);
		Rect<int> newRect(pair.second.getX() * DEFAULT_WINDOW_WIDTH, pair.second.getY() * DEFAULT_WINDOW_HEIGHT, width, height);
		frame2.setDimensions(&newRect);
		frame2.setCurvatureToMovement(pair.second.getCurvature());
		previousDimensions[direction] = newRect;
		tracks[direction].addKeyFrame(duration, &frame2);
	}

	// This adds keyframes to the ends of tracks.
	KeyFrame2D frameRight;
	frameRight.setObject(keyImages[NoteDirection::RIGHT]);
	frameRight.setDimensions(&previousDimensions[NoteDirection::RIGHT]);
	tracks[NoteDirection::RIGHT].addKeyFrame(battle->getDuration(), &frameRight);

	KeyFrame2D frameUp;
	frameUp.setObject(keyImages[NoteDirection::UP]);
	frameUp.setDimensions(&previousDimensions[NoteDirection::UP]);
	tracks[NoteDirection::UP].addKeyFrame(battle->getDuration(), &frameUp);

	KeyFrame2D frameLeft;
	frameLeft.setObject(keyImages[NoteDirection::LEFT]);
	frameLeft.setDimensions(&previousDimensions[NoteDirection::LEFT]);
	tracks[NoteDirection::LEFT].addKeyFrame(battle->getDuration(), &frameLeft);

	KeyFrame2D frameDown;
	frameDown.setObject(keyImages[NoteDirection::DOWN]);
	frameDown.setDimensions(&previousDimensions[NoteDirection::DOWN]);
	tracks[NoteDirection::DOWN].addKeyFrame(battle->getDuration(), &frameDown);

	for (auto& pair : tracks) {
		animator->addAnimationTrack2D(&pair.second);
	}
}

void Game::BattleDisplay::setBattleSubMenu(std::shared_ptr<SubMenu> battleSubMenu) {
	this->battleSubMenu = battleSubMenu;
	battleSubMenu->hide();
}

void Game::BattleDisplay::setBattleBackgrounds(std::shared_ptr<ImageComponent> battleBackground, std::shared_ptr<ImageComponent> battleBackground2) {
	this->battleBackground = battleBackground;
	this->battleBackground2 = battleBackground2;
}
