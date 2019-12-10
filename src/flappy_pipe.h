#pragma once

#include <Shared/all.h>

namespace FlappyBird
{
	class FlappyPipe : public Scene::Node
	{
	public:
		const static float WindowSize;

	public:
		FlappyPipe(const std::shared_ptr<Renderer::Texture>& texture, const Graphics::TexRegion& top_region, const Graphics::TexRegion& bottom_region, 
			const Graphics::TexRegion& body_region);
		~FlappyPipe();

	public:
		auto isScored() const { return mScored; }
		void setScored(bool value) { mScored = value; }

	private:
		bool mScored = false;
	};
}