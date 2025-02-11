#include "button.h"

using namespace FlappyBird;

Button::Button(const std::string& text, int order) : mOrder(order)
{
	const glm::vec2 size = { 150.0f, 35.0f };

	setAnchor({ -0.5f, 0.5f });
	setPivot({ 0.5f, 0.5f });
	setY(order * 50.0f);
	setSize(size);

	mBlackRectangle->setAnchor({ 0.5f, 0.5f });
	mBlackRectangle->setPivot({ 0.5f, 0.5f });
	mBlackRectangle->setSize(size);
	mBlackRectangle->setColor(Graphics::Color::Black);

	mWhiteRectangle->setAnchor({ 0.5f, 0.5f });
	mWhiteRectangle->setPivot({ 0.5f, 0.5f });
	mWhiteRectangle->setSize(size - glm::vec2(4.0f));
	mWhiteRectangle->setColor(Graphics::Color::White);

	mOrangeRectangle->setAnchor({ 0.5f, 0.5f });
	mOrangeRectangle->setPivot({ 0.5f, 0.5f });
	mOrangeRectangle->setSize(size - glm::vec2(8.0f));
	mOrangeRectangle->setColor(Graphics::Color::ToNormalized(231, 97, 0));
	mOrangeRectangle->setEnabled(true);

	mGreenRectangle->setAnchor({ 0.5f, 0.5f });
	mGreenRectangle->setPivot({ 0.5f, 0.5f });
	mGreenRectangle->setSize(size - glm::vec2(8.0f));
	mGreenRectangle->setColor(Graphics::Color::ToNormalized(97, 231, 0));
	mGreenRectangle->setEnabled(false);

	mLabel->setFont(FONT("button"));
	mLabel->setFontSize(20.0f);
	mLabel->setText(sky::to_wstring(text));
	mLabel->setPivot({ 0.5f, 0.5f });
	mLabel->setAnchor({ 0.5f, 0.5f });
	mLabel->setColor(Graphics::Color::White);
	mLabel->getOutlineColor()->setColor(Graphics::Color::White);
	mLabel->setOutlineThickness(0.2f); // fix FFFFORWA artefacts, wtf

	attach(mBlackRectangle);
	attach(mWhiteRectangle);
	attach(mOrangeRectangle);
	attach(mGreenRectangle);
	attach(mLabel);

	setChooseBeginCallback([this] {
		mGreenRectangle->setEnabled(true);
		mOrangeRectangle->setEnabled(false);
	});
	setChooseEndCallback([this] {
		mGreenRectangle->setEnabled(false);
		mOrangeRectangle->setEnabled(true);
	});
}

Button::~Button()
{
}

void Button::hide(std::function<void()> finishCallback)
{
	setClickEnabled(false);
	runAction(Actions::Collection::MakeSequence(
		Actions::Collection::Wait(0.1f * mOrder),
		Actions::Collection::ChangeHorizontalAnchor(shared_from_this(), 0.5f, -0.5f, 0.4f, Easing::BackIn),
		Actions::Collection::Execute(finishCallback)
	));
}

void Button::show(std::function<void()> finishCallback)
{
	setClickEnabled(true);
	runAction(Actions::Collection::MakeSequence(
		Actions::Collection::Wait(0.1f * mOrder),
		Actions::Collection::ChangeHorizontalAnchor(shared_from_this(), -0.5f, 0.5f, 0.4f, Easing::BackOut),
		Actions::Collection::Execute(finishCallback)
	));
}