#pragma once

#include <Shared/all.h>

namespace FlappyBird
{
	class FlappyButton : public Scene::Actionable<Scene::Clickable<Scene::Node>>, public std::enable_shared_from_this<FlappyButton>
	{
	public:
		typedef std::function<void()> ClickCallback;

	public:
		FlappyButton(const std::string& text, int order);
		~FlappyButton();

	public:
		void hide(std::function<void()> finishCallback = nullptr);
		void show(std::function<void()> finishCallback = nullptr);

	private:
		std::shared_ptr<Scene::Rectangle> mBlackRectangle = std::make_shared<Scene::Rectangle>();
		std::shared_ptr<Scene::Rectangle> mWhiteRectangle = std::make_shared<Scene::Rectangle>();
		std::shared_ptr<Scene::Rectangle> mOrangeRectangle = std::make_shared<Scene::Rectangle>();
		std::shared_ptr<Scene::Rectangle> mGreenRectangle = std::make_shared<Scene::Rectangle>();
		std::shared_ptr<Scene::Label> mLabel = std::make_shared<Scene::Label>();
		
		int mOrder;
	};
}