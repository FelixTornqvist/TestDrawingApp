#include <stdexcept>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include "Window.hpp"

namespace drawApp {
	Window* Window::getInstance(std::string title, SDL_Rect possize) {
		return new Window(title, possize);
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
		Mix_Quit();
		TTF_Quit();
		IMG_Quit();
		SDL_Quit();
	}

	void Window::update() {
		updateMouseEvents();

		SDL_RenderClear(ren);
		rootElement->draw(ren);
		SDL_RenderPresent(ren);

//		SDL_Delay(1000/60);
	}

	void Window::notifyResize(int width, int height) {
		rootElement->setWidth(width - 20);
		rootElement->setHeight(height - 20);
		rootElement->updateChildSizes();
	}

	void Window::updateMouseEvents() {
		SDL_Point mPos;
		Uint32 button = SDL_GetMouseState(&(mPos.x), &(mPos.y));

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
	}

	float Window::getScaling() {
		float ret = 0, no;
		SDL_RenderGetScale(ren, &ret, &no);
		return ret;
	}

	SDL_Renderer* Window::getRenderer() {
		return ren;
	}

}
