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

		if ( SDL_CreateWindowAndRenderer(possize.w, possize.h, 0, &win, &ren) == -1) {
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

		SDL_Delay(1000/60);
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

}
