#ifndef UIELEMENT_HPP
#define UIELEMENT_HPP

#include <vector>
#include <SDL.h>

namespace drawApp {
	class UIElement {
		public:
			virtual ~UIElement();
			void addChild(UIElement* child);

			// called from Window
			virtual void mouseDown(Uint32 button, SDL_Point& pos) {};
			virtual void mouseDragged(Uint32 button, SDL_Point& pos) {};
			virtual void mouseUp(SDL_Point& pos) {};
			virtual void mouseHoverOn(SDL_Point& pos) {};
			virtual void mouseHoverOff() {};
			virtual void mouseScroll(SDL_Point& mPos, int x, int y) {}

			UIElement* findChildAt(SDL_Point& pos);
			void draw(SDL_Renderer* ren);
			virtual void updateChildSizes() = 0;

			int getX() const;
			int getY() const;
			virtual void setX(int x);
			virtual void setY(int y);
			int getWidth() const;
			int getHeight() const;
			virtual void setWidth(int w);
			virtual void setHeight(int h);

		protected:
			UIElement(SDL_Rect bounds);
			virtual void drawMe(SDL_Renderer* ren) = 0;

			SDL_Rect bounds;
			std::vector<UIElement*> children;

		private:

	};
}

#endif // UIELEMENT_HPP
