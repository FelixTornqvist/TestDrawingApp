#include <cmath>
#include <iostream>

#include "Canvas.hpp"

namespace drawApp {

	Canvas* Canvas::getInstance(const SDL_Rect& bounds, int width, int height, SDL_Renderer* ren) {
		return new Canvas(bounds, width, height, ren);
	}

	Canvas::Canvas(const SDL_Rect& bounds, int width, int height, SDL_Renderer* _ren): UIElement(bounds), ren(_ren) {
		content = Texture::createBlank(width, height, ren, SDL_TEXTUREACCESS_TARGET);
		lastPos = {0,0};
		currPos = {0,0};
		contentDest = bounds;
		zoomCenter = {width / 2, height / 2};
		zoomFactor = 1;
		clearCanvas(ren);
	}

	Canvas::~Canvas() {
		delete content;
	}

	void Canvas::drawMe(SDL_Renderer* ren) {
		content->render(ren, NULL, &contentDest);
	}

	void Canvas::updateChildSizes() {
		updateZoom();
	}

	void Canvas::updateZoom() {
		SDL_Point scaledZoomCenter = zoomCenter;
		scaledZoomCenter.x *= zoomFactor;
		scaledZoomCenter.y *= zoomFactor;

		contentDest.x = bounds.x + (bounds.w / 2) - scaledZoomCenter.x;
		contentDest.y = bounds.y + (bounds.h / 2) - scaledZoomCenter.y;
		contentDest.w = content->getWidth() * zoomFactor;
		contentDest.h = content->getHeight() * zoomFactor;
	}

	void Canvas::mouseDown(Uint32 btn, SDL_Point& pos) {
		if (btn & SDL_BUTTON(SDL_BUTTON_LEFT)) {
			brushDown = true;
			lastPos = convertGlobalToLocal(pos);
			currPos = lastPos;
		}
	}

	void Canvas::mouseDragged(Uint32 btn, SDL_Point& pos) {
		if (btn & SDL_BUTTON(SDL_BUTTON_LEFT)) {
			lastPos = currPos;
			currPos = convertGlobalToLocal(pos);
			drawLine(lastPos, currPos, brushSkippedPixels, ren);
		}
	}

	void Canvas::mouseUp(SDL_Point& pos) {
		brushDown = false;
	}

	void Canvas::mouseScroll(SDL_Point& mPos, int x, int y) {
		float zoomSpeed = 0.05;
		float scrollSpeed = 10;

		if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_LCTRL]) {
			zoomFactor *= (1 + (y > 0? zoomSpeed : -zoomSpeed));
			mPos = convertGlobalToLocal(mPos);
			if (y > 0) {
				zoomCenter.x += (mPos.x - zoomCenter.x) * zoomSpeed;
				zoomCenter.y += (mPos.y - zoomCenter.y) * zoomSpeed;
			} else if (y < 0) {
				zoomCenter.x -= (mPos.x - zoomCenter.x) * zoomSpeed;
				zoomCenter.y -= (mPos.y - zoomCenter.y) * zoomSpeed;
			}
		} else {
			zoomCenter.x -= x * scrollSpeed;
			zoomCenter.y -= y * scrollSpeed;
		}

		updateZoom();
	}

	void Canvas::multigesture(SDL_Point& fPos, float dRot, float dDist, Uint16 fingers) {
		SDL_Log("multigesture rot:%f dist:%f fingers:%d", dRot, dDist, fingers);
		zoomFactor += dDist * 0.005;
		zoomCenter = fPos;
		updateZoom();
	}


	void Canvas::drawLine(SDL_Point from, SDL_Point to, int skippedPixels, SDL_Renderer* ren) {
		int xDiff = std::abs(from.x - to.x);
		int yDiff = std::abs(from.y - to.y);
		content->setAsRenderTarget(ren);

		if (xDiff > yDiff) {
			if (from.x > to.x) {
				SDL_Point tmp = from;
				from = to;
				to = tmp;
			}
			float k = ((from.y - to.y) * -1.0f) / (xDiff * 1.0f);

			for (int x = 0; x <= xDiff; x += skippedPixels) {
				brushDest.y = x * k + from.y;
				brushDest.x = from.x + x;
				brush->render(ren, NULL, &brushDest);
			}

		} else {
			if (from.y > to.y) {
				SDL_Point tmp = from;
				from = to;
				to = tmp;
			}
			float k = ((from.x - to.x) * -1.0f) / (yDiff * 1.0f);

			for (int y = 0; y <= yDiff; y += skippedPixels) {
				brushDest.x = y * k + from.x;
				brushDest.y = from.y + y;
				brush->render(ren, NULL, &brushDest);
			}
		}

		SDL_SetRenderTarget(ren, NULL);
	}

	void Canvas::clearCanvas(SDL_Renderer* ren) {
		content->setAsRenderTarget(ren);
		SDL_SetRenderDrawColor(ren, 255,255,255,255);
		SDL_RenderClear(ren);
		SDL_SetRenderDrawColor(ren, 0,0,0,255);
		SDL_SetRenderTarget(ren, NULL);
	}


	void Canvas::setBrush(Texture* tex) {
		brush = tex;
		brushDest = {0, 0, 10,10};
	}

	void Canvas::setBrushSize(int radius) {
		brushDest.w = radius;
		brushDest.h = radius;
	}

	void Canvas::setBrushSkippedPixels(int skippedPixels) {
		brushSkippedPixels = skippedPixels > 0 ? skippedPixels : 1;
	}

	int Canvas::getBrushSize() const {
		return brushDest.w;
	}

	SDL_Point Canvas::convertGlobalToLocal(SDL_Point p) {
		p.x -= contentDest.x;
		p.y -= contentDest.y;
		p.x = ((content->getWidth() * 1.0) / contentDest.w) * p.x;
		p.y = ((content->getHeight() * 1.0) / contentDest.h) * p.y;
		return p;
	}

}
