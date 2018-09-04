#include <cmath>
#include <iostream>

#include "Canvas.hpp"

namespace drawApp {

	Canvas* Canvas::getInstance(const SDL_Rect& bounds, int width, int height, SDL_Renderer* ren) {
		return new Canvas(bounds, width, height, ren);
	}

	Canvas::Canvas(const SDL_Rect& bounds, int width, int height, SDL_Renderer* _ren): UIElement(bounds), ren(_ren) {
		content = Texture::createBlank(width, height, ren, SDL_TEXTUREACCESS_TARGET);
		contentDest = bounds;
		zoomCenter = {width / 2, height / 2};
		zoomFactor = 1;
		clearCanvas();
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
		content->setAsRenderTarget(ren);

		SDL_Point p = convertGlobalToLocal(pos);
		brush->mouseDown(btn, p);

		SDL_SetRenderTarget(ren, NULL);
	}

	void Canvas::mouseDragged(Uint32 btn, SDL_Point& pos) {
		content->setAsRenderTarget(ren);

		SDL_Point p = convertGlobalToLocal(pos);
		brush->mouseDragged(btn, p);

		SDL_SetRenderTarget(ren, NULL);
	}

	void Canvas::mouseUp(SDL_Point& pos) {
		content->setAsRenderTarget(ren);

		SDL_Point p = convertGlobalToLocal(pos);
		brush->mouseUp(p);

		SDL_SetRenderTarget(ren, NULL);
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

	void Canvas::clearCanvas() {
		content->setAsRenderTarget(ren);
		SDL_SetRenderDrawColor(ren, 255,255,255,255);
		SDL_RenderClear(ren);
		SDL_SetRenderDrawColor(ren, 0,0,0,255);
		SDL_SetRenderTarget(ren, NULL);
	}

	void Canvas::setBrush(Brush* _brush) {
		brush = _brush;
	}


	SDL_Point Canvas::convertGlobalToLocal(SDL_Point p) {
		p.x -= contentDest.x;
		p.y -= contentDest.y;
		p.x = ((content->getWidth() * 1.0) / contentDest.w) * p.x;
		p.y = ((content->getHeight() * 1.0) / contentDest.h) * p.y;
		return p;
	}

}
