#pragma once

#include <Shared/all.h>

namespace FlappyBird
{
	class FlappyLabel : public Scene::Actionable<Scene::Label>
	{
	public:
		FlappyLabel(const std::string& text = "");
		~FlappyLabel();

	public:
		void hide(std::function<void()> finishCallback = nullptr, float speed = 1.0f, float delay = 0.0f);
		void show(std::function<void()> finishCallback = nullptr, float speed = 1.0f, float delay = 0.0f);
	};
}