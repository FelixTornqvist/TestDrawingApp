#include <stdexcept>
#include <iostream>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#ifndef __EMSCRIPTEN__
#include <SDL_mixer.h>
#endif

#include "Window.hpp"
#include "UIButton.hpp"

namespace drawApp {
	Window* Window::getInstance(std::string title, SDL_Rect possize) {
		Window* ret = new Window(title, possize);

		UIButton::loadDefaulTexture(ret->getRenderer());
		return ret;
	}

	Window::Window(std::string title, SDL_Rect possize) {
		if ( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
			throw std::runtime_error(SDL_GetError());
		}

		win = SDL_CreateWindow(title.c_str(), possize.x, possize.y, possize.w, possize.h, SDL_WINDOW_RESIZABLE );
		if (win == nullptr) {
			throw std::runtime_error(SDL_GetError());
		}
		ren = SDL_CreateRenderer(win, -1, 0);
		if (ren == nullptr) {
			throw std::runtime_error(SDL_GetError());
		}
	}

	Window::~Window() {
		SDL_DestroyRenderer(ren);
		SDL_DestroyWindow(win);
#ifndef __EMSCRIPTEN__
		Mix_Quit();
#endif
		TTF_Quit();
		IMG_Quit();
		SDL_Quit();
	}

	void Window::update() {
		updateMouseEvents();

		SDL_RenderClear(ren);
		rootElement->draw(ren);
		SDL_RenderPresent(ren);
	}

	void Window::notifyResize(int width, int height) {
		width /= getScaling();
		height /= getScaling();

		rootElement->setX(0);
		rootElement->setY(0);
		rootElement->setWidth(width);
		rootElement->setHeight(height);
		rootElement->updateChildSizes();
	}

	void Window::notifyScroll(int x, int y) {
		SDL_Point mPos;
		Uint32 button = SDL_GetMouseState(&(mPos.x), &(mPos.y));
		mPos.x /= getScaling();
		mPos.y /= getScaling();
		UIElement* hoverOver = rootElement->findChildAt(mPos);
		hoverOver->mouseScroll(mPos, x, y);
	}

	void Window::notifyMultigesture(SDL_MultiGestureEvent& mgEve) {
		SDL_Point mgPos = {static_cast<int>(mgEve.x), static_cast<int>(mgEve.y)};
		UIElement* gestureEle = rootElement->findChildAt(mgPos);
		gestureEle->multigesture(mgPos, mgEve.dTheta, mgEve.dDist, mgEve.numFingers);
	}

	void Window::notifyFingerDown(SDL_TouchFingerEvent& fEve) {
		notifyFinger(fEve, &UIElement::fingerDown);
	}

	void Window::notifyFingerMotion(SDL_TouchFingerEvent& fEve) {
		notifyFinger(fEve, &UIElement::fingerDragged);
	}

	void Window::notifyFingerUp(SDL_TouchFingerEvent& fEve) {
		notifyFinger(fEve, &UIElement::fingerUp);
	}

	void Window::notifyFinger(SDL_TouchFingerEvent& fEve, void(UIElement::* notifyFunc)(SDL_Point&, SDL_FingerID, float)) {
#ifndef __EMSCRIPTEN__
		SDL_Point fPos = {static_cast<int>(fEve.x), static_cast<int>(fEve.y)};
#else
		int w, h;
		getSize(&w, &h);
		SDL_Point fPos = {static_cast<int>(w * fEve.x), static_cast<int>(h * fEve.y)};
#endif

		std::cout << "fpos x" << fEve.x << " y"<< fEve.y << std::endl;

		UIElement* touchedEle = rootElement->findChildAt(fPos);
		(touchedEle->*notifyFunc)(fPos, fEve.fingerId, fEve.pressure);
	}


	void Window::updateMouseEvents() {
		SDL_Point mPos;
		Uint32 button = SDL_GetMouseState(&(mPos.x), &(mPos.y));
		mPos.x /= getScaling();
		mPos.y /= getScaling();

		if (button != 0) {
			if (clickedElement == nullptr) {
				clickedElement = rootElement->findChildAt(mPos);
				clickedElement->mouseDown(button, mPos);
			} else {
				clickedElement->mouseDragged(button, mPos);
			}

		} else {
			if (clickedElement != nullptr) {
				clickedElement->mouseUp(mPos);
				clickedElement = nullptr;
			}

			UIElement* currHover = rootElement->findChildAt(mPos);
			if (hoveredElement != currHover) {
				hoveredElement->mouseHoverOff();
				hoveredElement = currHover;
				hoveredElement->mouseHoverOn(mPos);
			}

		}
	}


	void Window::setRootElement(UIElement* ele) {
		rootElement = ele;
		hoveredElement = rootElement;
	}

	void Window::setScaling(float scale) {
		SDL_RenderSetScale(ren, scale, scale);

		int w, h;
		SDL_GetWindowSize(win, &w, &h);
		notifyResize(w, h);
	}

	float Window::getScaling() {
		float ret = 0, no;
		SDL_RenderGetScale(ren, &ret, &no);
		return ret;
	}

	SDL_Renderer* Window::getRenderer() {
		return ren;
	}

	void Window::getSize(int* w, int* h) {
		SDL_GetWindowSize(win, w, h);
	}


}
