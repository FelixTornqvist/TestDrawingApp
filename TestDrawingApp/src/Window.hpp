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

		protected:
			Window(std::string title, SDL_Rect possize);

		private:
			SDL_Window* win;
			SDL_Renderer* ren;
			UIElement* rootElement;
			UIElement* clickedElement = nullptr;
			UIElement* hoveredElement = nullptr;

			void updateMouseEvents();
	};
}

#endif // WINDOW_HPP
