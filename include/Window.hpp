#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <string>
#include <SDL.h>
#include "UIElement.hpp"

namespace drawApp {
	class Window {
		public:
			static Window* getInstance(std::string title, SDL_Rect possize);
			virtual ~Window();

			void update();
			void setRootElement(UIElement* ele);
			void setScaling(float scale);
			float getScaling();

			void notifyResize(int width, int height);
			void notifyScroll(int x, int y);

			void notifyFingerDown(SDL_TouchFingerEvent& fEve);
			void notifyFingerMotion(SDL_TouchFingerEvent& fEve);
			void notifyFingerUp(SDL_TouchFingerEvent& fEve);

			void notifyMultigesture(SDL_MultiGestureEvent& mgEve);

			SDL_Renderer* getRenderer();
			void getSize(int* w, int* h);

		protected:
			Window(std::string title, SDL_Rect possize);

		private:
			SDL_Window* win;
			SDL_Renderer* ren;
			UIElement* rootElement;
			UIElement* clickedElement = nullptr;
			UIElement* hoveredElement = nullptr;

			void updateMouseEvents();
			void notifyFinger(SDL_TouchFingerEvent& fEve, void (UIElement::*notifyFunc)(SDL_Point&, SDL_FingerID, float));
	};
}

#endif // WINDOW_HPP
