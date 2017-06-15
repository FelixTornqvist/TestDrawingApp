#include "UIButton.hpp"

namespace drawApp {

	UIButton* UIButton::getInstance(SDL_Rect& bounds, std::string label, std::string bg, std::function<void()> callback) {

	}

	UIButton::UIButton(SDL_Rect& bounds, std::string label, std::string bg, std::function<void()> callback): UIElement(bounds) {

	}

	UIButton::~UIButton() {
		delete background;
		delete text;
	}

	void UIButton::setText(std::string label) {

	}

	void UIButton::mouseDown(Uint32 btn, SDL_Point& pos) {
		if (btn & SDL_BUTTON(SDL_BUTTON_LEFT)) {
			LMBClicked = true;
		}
	}

	void UIButton::mouseUp(SDL_Point& pos) {
		if (LMBClicked) {
			callback();
		}

		LMBClicked = false;
	}



}
