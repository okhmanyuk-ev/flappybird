#include "pipe.h"

using namespace FlappyBird;

const float Pipe::WindowSize = 180.0f;

Pipe::Pipe()
{
	const float size = 75.0f;

	setWidth(size);
	setVerticalStretch(2.0f);
	setHorizontalPivot(0.5f);

	auto topSprite = std::make_shared<Scene::Sprite>();
	topSprite->setTexture(TEXTURE("textures/pipe_top.png"));
	topSprite->setAnchor({ 0.5f, 0.0f });
	topSprite->setPivot({ 0.5f, 0.0f });
	topSprite->setSize({ size, size });
	topSprite->setY(WindowSize / 2.0f);
	attach(topSprite);

	auto topBody = std::make_shared<Scene::Sprite>();
	topBody->setTexture(TEXTURE("textures/pipe_body.png"));
	topBody->setAnchor({ 0.5f, 0.0f });
	topBody->setPivot({ 0.5f, 0.0f });
	topBody->setVerticalStretch(1.0f);
	topBody->setWidth(size);
	topBody->setY((WindowSize / 2.0f) + size);
	attach(topBody);
	
	auto bottomSprite = std::make_shared<Scene::Sprite>();
	bottomSprite->setTexture(TEXTURE("textures/pipe_bottom.png"));
	bottomSprite->setAnchor({ 0.5f, 0.0f });
	bottomSprite->setPivot({ 0.5f, 1.0f });
	bottomSprite->setSize({ size, size });
	bottomSprite->setY(-WindowSize / 2.0f);
	attach(bottomSprite);

	auto bottomBody = std::make_shared<Scene::Sprite>();
	bottomBody->setTexture(TEXTURE("textures/pipe_body.png"));
	bottomBody->setAnchor({ 0.5f, 0.0f });
	bottomBody->setPivot({ 0.5f, 1.0f });
	bottomBody->setVerticalStretch(1.0f);
	bottomBody->setWidth(size);
	bottomBody->setY((-WindowSize / 2.0f) - size);
	attach(bottomBody);
}

Pipe::~Pipe()
{
	//
}