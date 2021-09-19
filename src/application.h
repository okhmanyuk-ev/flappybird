#pragma once

#include <Shared/all.h>

#include "label.h"
#include "button.h"
#include "pipe.h"

namespace FlappyBird
{
	class Application : public Shared::Application, 
		public Common::FrameSystem::Frameable
	{
	public:
		Application();
		~Application();
		
	private:
		void initialize();
		void onFrame() override;
		void tap();
		void jump();
		void collide();

	private:
		void showMainMenu();
		void hideMainMenu(std::function<void()> finishCallback = nullptr);

		void showReadyMenu();
		void hideReadyMenu(std::function<void()> finishCallback = nullptr);

		void showGameOverMenu();
		void hideGameOverMenu(std::function<void()> finishCallback = nullptr);

		void showPlayMenu();
		void hidePlayMenu(std::function<void()> finishCallback = nullptr);

	private:
		float getWorkingAreaHeight();
		float getBirdHorizontalPosition();

	private:
		struct 
		{ 
			float vert_position = 0.0f; 
			float fall_velocity = 0.0f; 
			float rotation = 0.0f;
		} mBird;

		struct
		{
			float horz_velocity = 0.0f;
		} mWorld;

		enum class State
		{
			None,
			Main,
			Ready,
			Playing,
			GameOver,
			Quit
		};

		State mState = State::None;

		int mScore = 0;
		Common::Timer mScoreShakeTimer;

		const glm::vec2 BirdSize = { 40.0f, 30.0f };
		const glm::vec2 GlassesSize = { 80.0f, 16.0f };
		const float PipeSpawnDistance = 180.0f;
		const float Gravitation = 0.4f;
		const float MaxFallSpeed = 15.0f;

		bool mClearing = false;

		Common::Interpolator mGlassesInterpolator;

	private: // labels
		std::shared_ptr<Label> mFlappyBirdLabel;
		std::shared_ptr<Label> mReadyLabel;
		std::shared_ptr<Label> mScoreLabel;
		std::shared_ptr<Label> mGameOverLabel;
		std::shared_ptr<Label> mGameOverScoreLabel;

	private: // buttons
		std::shared_ptr<Button> mPlayButton;
		std::shared_ptr<Button> mHighScoresButton;
		std::shared_ptr<Button> mQuitButton;

		std::shared_ptr<Button> mRetryButton;
		std::shared_ptr<Button> mMainMenuButton;

	private: // scene
		std::shared_ptr<Scene::Sprite> mBackground;
		std::shared_ptr<Scene::Node> mPipeHolder = std::make_shared<Scene::Node>();
		std::shared_ptr<Scene::Sprite> mGround;
		std::shared_ptr<Scene::Sprite> mBirdSprite = std::make_shared<Scene::Sprite>();
		std::shared_ptr<Scene::Sprite> mGlassesSprite = std::make_shared<Scene::Sprite>();
	};
}
