#include "Brush.hpp"

#include <iostream>

namespace drawApp {

	Brush* Brush::getInstance(std::string texPath, SDL_Renderer* ren) {
		return new Brush(texPath, ren);
	}

	Brush::Brush(std::string texPath, SDL_Renderer* _ren): brushTex(Texture::createFromFile(texPath, _ren)), ren(_ren) {
		brushTex->setBlendMode(SDL_BLENDMODE_BLEND);
	}

	Brush::~Brush() {
		delete brushTex;
	}

	void Brush::renderLine(SDL_Point from, SDL_Point to) {
		int xDiff = std::abs(from.x - to.x);
		int yDiff = std::abs(from.y - to.y);

		if (xDiff == 0 && yDiff == 0)
			return;

		if (xDiff > yDiff) {
			if (from.x > to.x) {
				SDL_Point tmp = from;
				from = to;
				to = tmp;
			}
			float k = ((from.y - to.y) * -1.0f) / (xDiff * 1.0f);

			for (int x = 0; x <= xDiff; x += spacing) {
				brushDest.y = x * k + from.y;
				brushDest.x = from.x + x;
				brushTex->render(ren, NULL, &brushDest);
			}

		} else {
			if (from.y > to.y) {
				SDL_Point tmp = from;
				from = to;
				to = tmp;
			}
			float k = ((from.x - to.x) * -1.0f) / (yDiff * 1.0f);

			for (int y = 0; y <= yDiff; y += spacing) {
				brushDest.x = y * k + from.x;
				brushDest.y = from.y + y;
				brushTex->render(ren, NULL, &brushDest);
			}
		}

	}

	void Brush::mouseDown(Uint32 btn, SDL_Point& pos) {
		if (btn & SDL_BUTTON(SDL_BUTTON_LEFT)) {
			brushDown = true;
			currPos = pos;
		}
	}

	void Brush::mouseDragged(Uint32 btn, SDL_Point& pos) {
		if (btn & SDL_BUTTON(SDL_BUTTON_LEFT)) {
			lastPos = currPos;
			currPos = pos;
			renderLine(lastPos, currPos);
		}
	}

	void Brush::mouseUp(SDL_Point& pos) {
		brushDown = false;
		lastPos = currPos;
		currPos = pos;
		renderLine(lastPos, currPos);
	}

	void Brush::setSpacing(int _spacing) {
		spacing = _spacing;
	}

	int Brush::getSpacing() {
		return spacing;
	}


}
