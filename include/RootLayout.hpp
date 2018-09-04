#ifndef ROOTLAYOUT_HPP
#define ROOTLAYOUT_HPP

#include "UIElement.hpp"
#include "Window.hpp"
#include "Canvas.hpp"
#include "UIContainerGrid.hpp"

namespace drawApp {
	class RootLayout : public UIElement {
		public:
			static RootLayout* getInstance(Window* win);
			virtual ~RootLayout();

			void updateChildSizes();
			void drawMe(SDL_Renderer* ren) {}

		protected:
			RootLayout(Window* win);

		private:
			Canvas* canvas;
			UIContainerGrid* tools;

			void tryMe();
	};
}

#endif // ROOTLAYOUT_HPP
