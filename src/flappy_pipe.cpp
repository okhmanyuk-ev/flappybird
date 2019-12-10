#include "flappy_pipe.h"

using namespace FlappyBird;

const float FlappyPipe::WindowSize = 180.0f;

FlappyPipe::FlappyPipe(const std::shared_ptr<Renderer::Texture>& texture, const Graphics::TexRegion& top_region, const Graphics::TexRegion& bottom_region,
	const Graphics::TexRegion& body_region)
{
	const float size = 75.0f;

	setWidth(size);
	setVerticalStretch(2.0f);
	setHorizontalPivot(0.5f);

	auto topSprite = std::make_shared<Scene::Sprite>();
	topSprite->setTexture(texture);
	topSprite->setTexRegion(top_region);
	topSprite->setAnchor({ 0.5f, 0.0f });
	topSprite->setPivot({ 0.5f, 0.0f });
	topSprite->setSize({ size, size });
	topSprite->setY(WindowSize / 2.0f);
	attach(topSprite);

	auto topBody = std::make_shared<Scene::Sprite>();
	topBody->setTexture(texture);
	topBody->setTexRegion(body_region);
	topBody->setAnchor({ 0.5f, 0.0f });
	topBody->setPivot({ 0.5f, 0.0f });
	topBody->setVerticalStretch(1.0f);
	topBody->setWidth(size);
	topBody->setY((WindowSize / 2.0f) + size);
	attach(topBody);
	
	auto bottomSprite = std::make_shared<Scene::Sprite>();
	bottomSprite->setTexture(texture);
	bottomSprite->setTexRegion(bottom_region);
	bottomSprite->setAnchor({ 0.5f, 0.0f });
	bottomSprite->setPivot({ 0.5f, 1.0f });
	bottomSprite->setSize({ size, size });
	bottomSprite->setY(-WindowSize / 2.0f);
	attach(bottomSprite);

	auto bottomBody = std::make_shared<Scene::Sprite>();
	bottomBody->setTexture(texture);
	bottomBody->setTexRegion(body_region);
	bottomBody->setAnchor({ 0.5f, 0.0f });
	bottomBody->setPivot({ 0.5f, 1.0f });
	bottomBody->setVerticalStretch(1.0f);
	bottomBody->setWidth(size);
	bottomBody->setY((-WindowSize / 2.0f) - size);
	attach(bottomBody);
}

FlappyPipe::~FlappyPipe()
{
	//
}