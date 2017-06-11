#include "UIElement.hpp"

namespace drawApp {
	UIElement::UIElement(SDL_Rect bound) {
		bounds = bound;
	}

	UIElement::~UIElement() {
		//dtor
	}

	UIElement* UIElement::findChildAt(SDL_Point& pos) {
		for (UIElement* child : children) {
			if (SDL_PointInRect(&pos, &(child->bounds)))
				return child->findChildAt(pos);
		}
		return this;
	}

	void UIElement::draw(SDL_Renderer* ren) const {
		drawMe(ren);
		for (UIElement* child : children) {
			child->draw(ren);
		}
	}

	void UIElement::addChild(UIElement* child) {
		children.push_back(child);
	}


	int UIElement::getX() const {
		return bounds.x;
	}

	int UIElement::getY() const {
		return bounds.y;
	}

	void UIElement::setX(int x) {
		bounds.x = x;
	}

	void UIElement::setY(int y) {
		bounds.y = y;
	}

	int UIElement::getWidth() const {
		return bounds.w;
	}

	int UIElement::getHeight() const {
		return bounds.h;
	}

	void UIElement::setWidth(int w) {
		bounds.w = w;
	}

	void UIElement::setHeight(int h) {
		bounds.h = h;
	}

}
