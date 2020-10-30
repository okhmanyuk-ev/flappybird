#include "application.h"

using namespace FlappyBird;

Application::Application() : Shared::Application(PROJECT_NAME, { Shared::Application::Flag::Scene })
{
	PLATFORM->setTitle(PRODUCT_NAME);
	PLATFORM->resize(360, 640);
	RENDERER->setVsync(true);
#if !defined(PLATFORM_MOBILE)
	PLATFORM->rescale(1.5f);
#endif

	PRECACHE_FONT_ALIAS("fonts/04b19.ttf", "label");
	PRECACHE_FONT_ALIAS("fonts/FFFFORWA.TTF", "button");

#if defined(BUILD_DEVELOPER)
	CONSOLE->execute("hud_show_fps 1");
	CONSOLE->execute("hud_show_drawcalls 1");
#else
	CONSOLE_DEVICE->setEnabled(false);
	STATS->setEnabled(false);
#endif

	RENDERER->setVsync(true);

	CACHE->makeAtlases();

	FRAME->addOne([this] {
		initialize();
	});
}

Application::~Application()
{
	//
}

void Application::initialize()
{	
	// labels
	mFlappyBirdLabel = std::make_shared<Label>("Flappy Bird");
	mReadyLabel = std::make_shared<Label>("Ready!?");
	mScoreLabel = std::make_shared<Label>();
	mGameOverLabel = std::make_shared<Label>("Game Over");
	mGameOverScoreLabel = std::make_shared<Label>();

	// buttons
	mPlayButton = std::make_shared<Button>("PLAY", 0);
	mHighScoresButton = std::make_shared<Button>("HIGH SCORES", 1);
	mQuitButton = std::make_shared<Button>("QUIT", 2);

	mRetryButton = std::make_shared<Button>("RETRY", 1);
	mMainMenuButton = std::make_shared<Button>("MAIN MENY", 2);

	mPlayButton->setClickCallback([this] {
		hideMainMenu([this] {
			mState = State::Ready;
			showReadyMenu();
		});
	});

	mHighScoresButton->setClickCallback([this] {
		hideMainMenu(std::bind(&Application::showMainMenu, this));
	});

	mQuitButton->setClickCallback([this] {
		hideMainMenu([this] {
			Common::Actions::Run(Shared::ActionHelpers::Delayed(0.5f,
				Shared::ActionHelpers::Execute([this] {
					PLATFORM->quit();
				})
			));
		});
		mState = State::Quit;
	});

	mRetryButton->setClickCallback([this] {
		hideGameOverMenu([this] {
			showReadyMenu();
		});
		mState = State::Ready;
	});

	mMainMenuButton->setClickCallback([this] {
		hideGameOverMenu([this] {
			showMainMenu();
		});
		mState = State::Ready;
	});

	Common::Actions::Run(Shared::ActionHelpers::Delayed(0.75f,
		Shared::ActionHelpers::Execute([this] {
			showMainMenu();
		})
	));

	mBird.vert_position = -BirdSize.y;
	mBird.fall_velocity = MaxFallSpeed;

	mBackgroundHolder->setStretch({ 1.0f, 1.0f });
	mPipeHolder->setStretch({ 1.0f, 1.0f });
	mGroundHolder->setStretch({ 1.0f, 1.0f });

	mBirdSprite->setTexture(TEXTURE("textures/bird.png"));
	mBirdSprite->setSize(BirdSize);
	mBirdSprite->setPivot({ 0.5f, 0.5f });

	mGlassesSprite->setTexture(TEXTURE("textures/glasses.png"));
	mGlassesSprite->setSize(GlassesSize);
	mGlassesSprite->setPivot({ 0.5f, 0.5f });

	mReadyLabel->setColor(Graphics::Color::ToNormalized(252, 185, 0));

	mScoreLabel->setVerticalAnchor(0.15f);

	mGameOverLabel->setVerticalAnchor(0.2f);
	mGameOverLabel->setColor(Graphics::Color::ToNormalized(230, 68, 52));

	mGameOverScoreLabel->setVerticalAnchor(0.325f);
	mGameOverScoreLabel->setColor(Graphics::Color::ToNormalized(253, 232, 0));

	auto root = mScene->getRoot();

	auto bloom_layer = std::make_shared<Scene::BloomLayer>();
	bloom_layer->setStretch(1.0f);
	bloom_layer->setBrightThreshold(0.96f);
	bloom_layer->setGlowIntensity(1.125f);
	root->attach(bloom_layer);

	auto sky = std::make_shared<Scene::Rectangle>();
	sky->setColor(Graphics::Color::ToNormalized(83, 190, 206));
	sky->setStretch(1.0f);
	bloom_layer->attach(sky);

	sky->attach(mBackgroundHolder);
	sky->attach(mPipeHolder);
	sky->attach(mGroundHolder);
	sky->attach(mBirdSprite);
	sky->attach(mGlassesSprite);

	sky->attach(mFlappyBirdLabel);
	sky->attach(mReadyLabel);
	sky->attach(mScoreLabel);
	sky->attach(mGameOverLabel);
	sky->attach(mGameOverScoreLabel);

	sky->attach(mPlayButton);
	sky->attach(mHighScoresButton);
	sky->attach(mQuitButton);

	sky->attach(mRetryButton);
	sky->attach(mMainMenuButton);

#if defined(BUILD_SPY)
	auto black_rect = std::make_shared<Scene::Rectangle>();
	black_rect->setStretch(true);
	black_rect->setColor(Graphics::Color::Black);
	black_rect->setAlpha(0.8f);
	root->attach(black_rect);
#endif
}

void Application::frame()
{
	float dTime = Clock::ToSeconds(FRAME->getTimeDelta());

	if ((mState != State::GameOver && mState != State::None) || mClearing)
	{
		if (mState == State::Quit)
		{
			if (mBird.fall_velocity < MaxFallSpeed)
			{
				mBird.fall_velocity = -MaxFallSpeed;
			}
			else
			{
				mBird.fall_velocity -= Gravitation * dTime * 100.0f;
			}
		}
		else
		{
			if (mBird.fall_velocity > MaxFallSpeed)
			{
				mBird.fall_velocity = MaxFallSpeed;
			}
			else
			{
				mBird.fall_velocity += Gravitation * dTime * 100.0f;
			}
		}
		mBird.vert_position += mBird.fall_velocity * dTime * 100.0f / 1.5f;
	}

	// prevent big fall on big freeze

	if (mBird.vert_position > PLATFORM->getLogicalHeight())
	{
		mBird.vert_position = PLATFORM->getLogicalHeight();
	}

	if (mState != State::GameOver || mClearing)
	{
		mBird.rotation = glm::radians(mBird.fall_velocity * 2.5f);
	}

	// world

	if (!mPipeHolder->hasNodes())
	{
		mClearing = false;
	}

	mWorld.horz_velocity = 1.0f;

	if (mState == State::Playing)
	{
		mWorld.horz_velocity *= 3.25f;
	}
	
	if (mClearing)
	{
		mWorld.horz_velocity *= 12.5f * mPipeHolder->getNodes().size();
	}
	
	if (mClearing || mState == State::Main || mState == State::Ready)
	{
		if (mBird.vert_position >= PLATFORM->getLogicalHeight() / 2.0f)
		{
			jump();
		}
	}

	float offset = 0.0f;
	
	if (mState != State::GameOver || mClearing)
		offset = mWorld.horz_velocity * dTime * 100.0f / 1.5f;

	mWorld.backgroundHorzPosition -= offset / 2.0f;
	mWorld.groundHorzPosition -= offset;

	for (auto& pipe : mPipeHolder->getNodes())
	{
		pipe->setX(pipe->getX()- offset);
	}

	auto bg_width = static_cast<float>(TEXTURE("textures/background.png").getWidth());
	while (mWorld.backgroundHorzPosition < -bg_width)
	{
		mWorld.backgroundHorzPosition += bg_width;
	}

	auto g_width = static_cast<float>(TEXTURE("textures/ground.png").getWidth());
	while (mWorld.groundHorzPosition < -g_width)
	{
		mWorld.groundHorzPosition += g_width;
	}

	while (mPipeHolder->hasNodes())
	{
		auto node = mPipeHolder->getNodes().front();
		
		if (node->getX() > -node->getWidth() / 2.0f)
			break;

		mPipeHolder->detach(node);
	}

	if (mState == State::Playing)
	{
		if (!mPipeHolder->hasNodes() || PLATFORM->getLogicalWidth() - mPipeHolder->getNodes().back()->getX() >= PipeSpawnDistance)
		{
			auto pipe = std::make_shared<Pipe>();

			float padding = 25.0f * PLATFORM->getScale();
			float min = 0.0f + (Pipe::WindowSize / 2.0f) + padding;
			float max = getWorkingAreaHeight() - (Pipe::WindowSize / 2.0f) - padding;

			if (max < min)
				max = min;

			pipe->setX(PLATFORM->getLogicalWidth() + (pipe->getWidth() / 2.0f));
			pipe->setY(glm::linearRand(min, max));

			mPipeHolder->attach(pipe);
		}
	}

	// check collision

	if (mState == State::Playing)
	{
		if (mBird.vert_position + (BirdSize.y / 2.0f) >= getWorkingAreaHeight())
			collide();
		
		if (mBird.vert_position - (BirdSize.y / 2.0f) <= 0.0f)
			collide();
		
		for (auto& pipe : mPipeHolder->getNodes())
		{
			if (getBirdHorizontalPosition() + (BirdSize.x / 2.0f) < pipe->getX() - (pipe->getWidth() / 2.0f) ||
				getBirdHorizontalPosition() - (BirdSize.x / 2.0f) > pipe->getX() + (pipe->getWidth() / 2.0f))
			{
				continue;
			}

			if (mBird.vert_position + (BirdSize.y / 2.0f) < pipe->getY() + (Pipe::WindowSize / 2.0f) &&
				mBird.vert_position - (BirdSize.y / 2.0f) > pipe->getY() - (Pipe::WindowSize / 2.0f))
			{
				continue;
			}

			collide();
			break;
		}
	}

	// calculate score

	if (mState == State::Playing)
	{
		for (const auto _pipe : mPipeHolder->getNodes())
		{
			auto pipe = std::static_pointer_cast<Pipe>(_pipe);
			
			if (pipe->isScored())
				continue;

			if (pipe->getX() + (pipe->getWidth() / 2.0f) >= getBirdHorizontalPosition() - (BirdSize.x / 2.0f))
				continue;
			
			mScore += 1;
			mScoreLabel->setText(std::to_string(mScore));
			mScoreLabel->runAction(Shared::ActionHelpers::Shake(mScoreLabel, 3.0f, 0.2f));
			pipe->setScored(true);
		}
	}

	// sky

	// bird sprite

	mBirdSprite->setRotation(mBird.rotation);
	mBirdSprite->setX(getBirdHorizontalPosition());
	mBirdSprite->setY(mBird.vert_position);

	// glasses sprite

	auto glassesDefaultPos = -std::max(GlassesSize.y, GlassesSize.x) * 2.0f;
	mGlassesSprite->setRotation(mBird.rotation * mGlassesInterpolator.getValue());
	mGlassesSprite->setX(getBirdHorizontalPosition());
	mGlassesSprite->setY(glassesDefaultPos + ((-glassesDefaultPos + mBird.vert_position - 5.0f) * mGlassesInterpolator.getValue()));

	// background holder

	//float bg_width = static_cast<float>(mBackgroundTexture.getWidth());
	auto bg_count = static_cast<int>(std::ceil(PLATFORM->getLogicalWidth() / bg_width)) + 1;
	assert(bg_count >= 0);
	auto& bg_nodes = mBackgroundHolder->getNodes();
	while (static_cast<int>(bg_nodes.size()) < bg_count)
	{
		auto node = std::make_shared<Scene::Sprite>();
		node->setTexture(TEXTURE("textures/background.png"));
		node->setPivot({ 0.0f, 1.0f });
		node->setAnchor({ 0.0f, 1.0f }); 
		node->setY(static_cast<float>(-TEXTURE("textures/ground.png").getHeight()));
		mBackgroundHolder->attach(node);
	}
	while (static_cast<int>(bg_nodes.size()) > bg_count)
	{
		auto node = bg_nodes.front();
		mBackgroundHolder->detach(node);
	}
	float bg_offset = 0.0f;
	for (auto node : bg_nodes)
	{
		node->setX(mWorld.backgroundHorzPosition + bg_offset);
		bg_offset += bg_width;
	}

	// ground holder

	//float g_width = static_cast<float>(mGroundTexture.getWidth());
	auto g_count = static_cast<int>(std::ceil(PLATFORM->getLogicalWidth() / g_width)) + 1;
	assert(g_count >= 0);
	auto& g_nodes = mGroundHolder->getNodes();
	while (static_cast<int>(g_nodes.size()) < g_count)
	{
		auto node = std::make_shared<Scene::Sprite>();
		node->setTexture(TEXTURE("textures/ground.png"));
		node->setPivot({ 0.0f, 1.0f });
		node->setAnchor({ 0.0f, 1.0f });
		mGroundHolder->attach(node);
	}
	while (static_cast<int>(g_nodes.size()) > g_count)
	{
		auto node = g_nodes.front();
		mGroundHolder->detach(node);
	}
	float g_offset = 0.0f;
	for (auto node : g_nodes)
	{
		node->setX(mWorld.groundHorzPosition + g_offset);
		g_offset += g_width;
	}
}

void Application::tap()
{
	if (CONSOLE_DEVICE->isOpened())
		return;

	if (mClearing)
		return;

	if (mState == State::Ready)
	{
		mState = State::Playing;
		hideReadyMenu(std::bind(&Application::showPlayMenu, this));
		mScore = 0;
		mScoreLabel->setText(std::to_string(mScore));
	}
	
	if (mState != State::Playing)
		return;
	
	jump();
}

void Application::jump()
{
	mBird.fall_velocity = -10.0f;
}

void Application::collide()
{
	hidePlayMenu();
	showGameOverMenu();
}

void Application::onEvent(const Platform::Input::Keyboard::Event& e)
{
	if (e.type != Platform::Input::Keyboard::Event::Type::Pressed)
		return;

	tap();
}

void Application::onEvent(const Platform::Input::Mouse::Event& e)
{
	if (e.type != Platform::Input::Mouse::Event::Type::ButtonDown)
		return;

	tap();
}

void Application::onEvent(const Platform::Input::Touch::Event& e)
{
	if (e.type != Platform::Input::Touch::Event::Type::Begin)
		return;

	tap();
}

void Application::showMainMenu()
{
	mFlappyBirdLabel->show();
	mPlayButton->show();
	mHighScoresButton->show();
	mQuitButton->show();
	mState = State::Main;
}

void Application::hideMainMenu(std::function<void()> finishCallback)
{
	mFlappyBirdLabel->hide();
	mPlayButton->hide();
	mHighScoresButton->hide();
	mQuitButton->hide(finishCallback);
}

void Application::showReadyMenu()
{
	if (mState != State::Ready)
		return;

	mReadyLabel->show();
}

void Application::hideReadyMenu(std::function<void()> finishCallback)
{
	mReadyLabel->hide(finishCallback);
}

void Application::showGameOverMenu()
{
	mGameOverScoreLabel->setText("Score: " + std::to_string(mScore));
	mGameOverLabel->show();
	mGameOverScoreLabel->show(nullptr, 1.0f, 0.175f);
	mRetryButton->show();
	mMainMenuButton->show();
	mState = State::GameOver;

	if (mScore > 0)
	{
		mGlassesInterpolator.setDuration(Clock::FromSeconds(0.75f));
		mGlassesInterpolator.setStartValue(mGlassesInterpolator.getValue());
		mGlassesInterpolator.setDestinationValue(1.0f);
		mGlassesInterpolator.setEasingFunction(Common::Easing::CubicOut);
		mGlassesInterpolator.setPassed(Clock::Duration::zero());
	}
}

void Application::hideGameOverMenu(std::function<void()> finishCallback)
{
	mClearing = true;
	mGameOverLabel->hide();
	mGameOverScoreLabel->hide(nullptr, 1.0f, 0.175f);
	mRetryButton->hide();
	mMainMenuButton->hide(finishCallback);

	if (mScore > 0)
	{
		mGlassesInterpolator.setDuration(Clock::FromSeconds(0.75f));
		mGlassesInterpolator.setStartValue(mGlassesInterpolator.getValue());
		mGlassesInterpolator.setDestinationValue(0.0f);
		mGlassesInterpolator.setEasingFunction(Common::Easing::CubicIn);
		mGlassesInterpolator.setPassed(Clock::Duration::zero());
	}
}

void Application::showPlayMenu()
{
	if (mState != State::Playing)
		return;

	mScoreLabel->show(nullptr, 2.0f);
}

void Application::hidePlayMenu(std::function<void()> finishCallback)
{
	mScoreLabel->hide(finishCallback, 2.0f);
}

float Application::getWorkingAreaHeight()
{
	return PLATFORM->getLogicalHeight() - TEXTURE("textures/ground.png").getHeight();
}

float Application::getBirdHorizontalPosition()
{
	return PLATFORM->getLogicalWidth() * 0.25f;
}