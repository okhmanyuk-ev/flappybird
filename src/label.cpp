#include "label.h"

using namespace FlappyBird;

Label::Label(const std::string& text)
{
	setFont(FONT("label"));
	setFontSize(50.0f);
	setText(text);
	setPivot({ 0.5f, 0.5f });
	setAnchor({ -0.5f, 0.25f });
	setOutlineColor(Graphics::Color::Black);
	setOutlineThickness(0.75f);
}

Label::~Label()
{
	//
}

void Label::hide(std::function<void()> finishCallback, float speed, float delay)
{
	runAction(Actions::Collection::MakeSequence(
		Actions::Collection::Wait(delay),
		Actions::Collection::ChangeHorizontalAnchor(shared_from_this(), 0.5f, 1.5f, 0.4f / speed, Easing::BackIn),
		Actions::Collection::Execute(finishCallback)
	));
}

void Label::show(std::function<void()> finishCallback, float speed, float delay)
{
	runAction(Actions::Collection::MakeSequence(
		Actions::Collection::Wait(delay),
		Actions::Collection::ChangeHorizontalAnchor(shared_from_this(), 1.5f, 0.5f, 0.4f / speed, Easing::BackOut),
		Actions::Collection::Execute(finishCallback)
	));
}