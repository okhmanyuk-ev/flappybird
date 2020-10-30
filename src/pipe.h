#pragma once

#include <Shared/all.h>

namespace FlappyBird
{
	class Pipe : public Scene::Node
	{
	public:
		const static float WindowSize;

	public:
		Pipe();
		~Pipe();

	public:
		auto isScored() const { return mScored; }
		void setScored(bool value) { mScored = value; }

	private:
		bool mScored = false;
	};
}