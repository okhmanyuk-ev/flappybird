#include "application.h"

using namespace FlappyBird;

Application::Application() : Shared::Application(PROJECT_NAME, { Shared::Application::Flag::Scene })
{
	PLATFORM->setTitle(PRODUCT_NAME);
	PLATFORM->resize(540, 960);

	PRECACHE_FONT_ALIAS("fonts/04b19.ttf", "label");
	PRECACHE_FONT_ALIAS("fonts/FFFFORWA.TTF", "button");

	CACHE->makeAtlases();

	initialize();
}

Application::~Application()
{
}

void Application::initialize()
{
	getScene()->setScreenAdaption(glm::vec2{ 360.0f, 640.0f });

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
			Actions::Run(Actions::Collection::Delayed(0.5f,
				Actions::Collection::Execute([this] {
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

	Actions::Run(Actions::Collection::Delayed(0.75f,
		Actions::Collection::Execute([this] {
			showMainMenu();
		})
	));

	mBird.vert_position = -BirdSize.y;
	mBird.fall_velocity = MaxFallSpeed;

	mPipeHolder->setStretch(1.0f);

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

	auto root = getScene()->getRoot();

	auto sky = std::make_shared<Scene::Tappable<Scene::Rectangle>>();
	sky->setTapCallback([this] {
		tap();
	});
	sky->setColor(Graphics::Color::ToNormalized(83, 190, 206));
	sky->setStretch(1.0f);
	sky->runAction(Actions::Collection::ExecuteInfinite([this](Clock::Duration delta) {
		update(Clock::ToSeconds(delta));
	}));
	root->attach(sky);

	mBackground = std::make_shared<Scene::Sprite>();
	mBackground->setTexture(TEXTURE("textures/background.png"));
	mBackground->setAnchor({ 0.0f, 1.0f });
	mBackground->setPivot({ 0.0f, 1.0f });
	mBackground->setStretch({ 2.0f, 0.0f });
	mBackground->setTextureAddress(skygfx::TextureAddress::Wrap);
	mBackground->runAction(Actions::Collection::ExecuteInfinite([this] {
		mBackground->setTexRegion({ { 0.0f, 0.0f }, { mBackground->getAbsoluteWidth(), 0.0f } });
	}));
	sky->attach(mBackground);

	sky->attach(mPipeHolder);

	mGround = std::make_shared<Scene::Sprite>();
	mGround->setTexture(TEXTURE("textures/ground.png"));
	mGround->setAnchor({ 0.0f, 1.0f });
	mGround->setPivot({ 0.0f, 1.0f });
	mGround->setStretch({ 2.0f, 0.0f });
	mGround->setTextureAddress(skygfx::TextureAddress::Wrap);
	mGround->runAction(Actions::Collection::ExecuteInfinite([this] {
		mGround->setTexRegion({ { 0.0f, 0.0f }, { mGround->getAbsoluteWidth(), 0.0f } });
	}));
	sky->attach(mGround);

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
}

void Application::update(float dTime)
{
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

	// move background

	mBackground->setY(-mGround->getHeight());
	mBackground->setX(mBackground->getX() - (offset / 2.0f));
	if (auto tex_w = (float)mBackground->getTexture()->getWidth(); mBackground->getX() <= -tex_w)
	{
		mBackground->setX(mBackground->getX() + tex_w);
	}

	// move ground

	mGround->setX(mGround->getX() - offset);
	if (auto tex_w = (float)mGround->getTexture()->getWidth(); mGround->getX() <= -tex_w)
	{
		mGround->setX(mGround->getX() + tex_w);
	}

	for (auto& pipe : mPipeHolder->getNodes())
	{
		pipe->setX(pipe->getX()- offset);
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
			mScoreLabel->setText(std::to_wstring(mScore));
			mScoreLabel->runAction(Actions::Collection::Shake(mScoreLabel, 3.0f, 0.2f));
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
}

void Application::onEvent(const Platform::Input::Keyboard::Event& e)
{
	if (e.type == Platform::Input::Keyboard::Event::Type::Pressed)
	{
		tap();
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
		mScoreLabel->setText(std::to_wstring(mScore));
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
	mGameOverScoreLabel->setText(L"Score: " + std::to_wstring(mScore));
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
		mGlassesInterpolator.setEasingFunction(Easing::CubicOut);
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
		mGlassesInterpolator.setEasingFunction(Easing::CubicIn);
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
	return PLATFORM->getLogicalHeight() - mGround->getHeight();
}

float Application::getBirdHorizontalPosition()
{
	return PLATFORM->getLogicalWidth() * 0.25f;
}