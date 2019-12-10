#pragma once

#include <Shared/all.h>

#include "flappy_label.h"
#include "flappy_button.h"
#include "flappy_pipe.h"

namespace FlappyBird
{
	class Application : public Shared::RichApplication, 
		public Common::EventSystem::Listenable<Platform::Keyboard::Event>,
		public Common::EventSystem::Listenable<Platform::Mouse::Event>,
		public Common::EventSystem::Listenable<Platform::Touch::Event>
	{
	public:
		Application();
		~Application();
		
	private:
		void initialize() override;
		void frame() override;
		void update();
		void tap();
		void jump();
		void collide();
		
	private:
		void event(const Platform::Keyboard::Event& e) override;
		void event(const Platform::Mouse::Event& e) override;
		void event(const Platform::Touch::Event& e) override;

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
			float backgroundHorzPosition = 0.0f;
			float groundHorzPosition = 0.0f;
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

	public:
		/*Graphics::Atlas mAtlas = Graphics::Atlas({
			{ "bird", Platform::Asset("textures/bird.png") },
			{ "background", Platform::Asset("textures/background.png") },
			{ "ground", Platform::Asset("textures/ground.png") },
			{ "pipe_body", Platform::Asset("textures/pipe_body.png") },
			{ "pipe_bottom", Platform::Asset("textures/pipe_bottom.png") },
			{ "pipe_top", Platform::Asset("textures/pipe_top.png") },
			{ "glasses", Platform::Asset("textures/glasses.png") }
		});*/

		std::optional<Graphics::Atlas> mAtlas;
		std::shared_ptr<Renderer::Texture> mAtlasTexture;

	private: // labels
		std::shared_ptr<FlappyLabel> mFlappyBirdLabel;
		std::shared_ptr<FlappyLabel> mReadyLabel;
		std::shared_ptr<FlappyLabel> mScoreLabel;
		std::shared_ptr<FlappyLabel> mGameOverLabel;
		std::shared_ptr<FlappyLabel> mGameOverScoreLabel;

	private: // buttons
		std::shared_ptr<FlappyButton> mPlayButton;
		std::shared_ptr<FlappyButton> mHighScoresButton;
		std::shared_ptr<FlappyButton> mQuitButton;

		std::shared_ptr<FlappyButton> mRetryButton;
		std::shared_ptr<FlappyButton> mMainMenuButton;

	private: // scene
		Scene::Scene mScene;
		
		std::shared_ptr<Scene::Node> mBackgroundHolder = std::make_shared<Scene::Node>();
		std::shared_ptr<Scene::Node> mPipeHolder = std::make_shared<Scene::Node>();
		std::shared_ptr<Scene::Node> mGroundHolder = std::make_shared<Scene::Node>();
		std::shared_ptr<Scene::Sprite> mBirdSprite = std::make_shared<Scene::Sprite>();
		std::shared_ptr<Scene::Sprite> mGlassesSprite = std::make_shared<Scene::Sprite>();
	};
}
