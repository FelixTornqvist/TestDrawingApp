#include "UIButton.hpp"

namespace drawApp {

	Texture* UIButton::defaultTexture = nullptr;

	UIButton* UIButton::getInstance(const SDL_Rect& bounds, std::string label, Texture* bg, std::function<void()> callback) {
		return new UIButton(bounds, label, bg, callback);
	}

	UIButton* UIButton::getInstance(std::string label, std::function<void()>callback) {
		return new UIButton({0,0,0,0}, label, defaultTexture, callback);
	}


	UIButton::UIButton(const SDL_Rect& bounds, std::string label, Texture* bg, std::function<void()> _callback):
		UIElement(bounds), callback(_callback), background(bg) {

	}

	UIButton::~UIButton() {
		delete background;
		delete text;
	}

	void UIButton::loadDefaulTexture(SDL_Renderer* ren) {
		if (!defaultTexture) {
			defaultTexture = Texture::createFromFile(DEFAULT_UIBUTTON_TEXTURE, ren);
			defaultTexture->setBlendMode(SDL_BLENDMODE_BLEND);
		}
	}

	void UIButton::drawMe(SDL_Renderer* ren) {
		background->render(ren, NULL, &bounds);
	}

	void UIButton::setText(std::string label) {

	}

	void UIButton::mouseDown(Uint32 btn, SDL_Point& pos) {
		if (btn & SDL_BUTTON(SDL_BUTTON_LEFT)) {
			LMBClicked = true;
		}
	}

	void UIButton::mouseUp(SDL_Point& pos) {
		if (LMBClicked && SDL_PointInRect(&pos, &bounds)) {
			callback();
		}

		LMBClicked = false;
	}



}
