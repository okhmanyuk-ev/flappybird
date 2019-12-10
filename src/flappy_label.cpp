#include "flappy_label.h"

using namespace FlappyBird;

FlappyLabel::FlappyLabel(const std::string& text)
{
	setFont(FONT("label"));
	setFontSize(50.0f);
	setText(text);
	setPivot({ 0.5f, 0.5f });
	setAnchor({ -0.5f, 0.25f });
	setOutlineColor(Graphics::Color::Black);
	setOutlineThickness(0.75f);
}

FlappyLabel::~FlappyLabel()
{
	//
}

void FlappyLabel::hide(std::function<void()> finishCallback, float speed, float delay)
{
	auto seq = Shared::ActionHelpers::MakeSequence();

	seq->add(Shared::ActionHelpers::Wait(delay));
	seq->add(std::make_unique<Common::Actions::Interpolate>(1.0f, 0.0f, Clock::FromSeconds(0.4f / speed), Common::Easing::BackIn, [this](auto value) {
		setHorizontalAnchor(1.5f - value);
	}));

	if (finishCallback)
		seq->add(Shared::ActionHelpers::Execute(finishCallback));

	runAction(std::move(seq));
}

void FlappyLabel::show(std::function<void()> finishCallback, float speed, float delay)
{
	auto seq = Shared::ActionHelpers::MakeSequence();

	seq->add(Shared::ActionHelpers::Wait(delay));
	seq->add(std::make_unique<Common::Actions::Interpolate>(0.0f, 1.0f, Clock::FromSeconds(0.4f / speed), Common::Easing::BackOut, [this](auto value) {
		setHorizontalAnchor(1.5f - value);
	}));

	if (finishCallback)
		seq->add(Shared::ActionHelpers::Execute(finishCallback));

	runAction(std::move(seq));
}