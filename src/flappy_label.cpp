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
	runAction(Shared::ActionHelpers::MakeSequence(
		Shared::ActionHelpers::Wait(delay),
		Shared::ActionHelpers::ChangeHorizontalAnchor(shared_from_this(), 0.5f, 1.5f, 0.4f / speed, Common::Easing::BackIn),
		Shared::ActionHelpers::Execute(finishCallback)
	));
}

void FlappyLabel::show(std::function<void()> finishCallback, float speed, float delay)
{
	runAction(Shared::ActionHelpers::MakeSequence(
		Shared::ActionHelpers::Wait(delay),
		Shared::ActionHelpers::ChangeHorizontalAnchor(shared_from_this(), 1.5f, 0.5f, 0.4f / speed, Common::Easing::BackOut),
		Shared::ActionHelpers::Execute(finishCallback)
	));
}