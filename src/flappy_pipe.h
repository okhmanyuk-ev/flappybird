#pragma once

#include <Shared/all.h>

namespace FlappyBird
{
	class FlappyPipe : public Scene::Node
	{
	public:
		const static float WindowSize;

	public:
		FlappyPipe();
		~FlappyPipe();

	public:
		auto isScored() const { return mScored; }
		void setScored(bool value) { mScored = value; }

	private:
		bool mScored = false;
	};
}