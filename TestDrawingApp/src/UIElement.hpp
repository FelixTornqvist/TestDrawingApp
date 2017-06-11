#ifndef UIELEMENT_HPP
#define UIELEMENT_HPP

#include <vector>
#include <SDL.h>

namespace drawApp {
	class UIElement {
		public:
			virtual ~UIElement();
			void addChild(UIElement* child);

			// called by a UIElement manager
			virtual void mouseDown(Uint8 button, SDL_Point& pos) {};
			virtual void mouseDragged(Uint8 button, SDL_Point& pos) {};
			virtual void mouseUp(SDL_Point& pos) {};
			virtual void mouseHoverOn(SDL_Point& pos) {};
			virtual void mouseHoverOff() {};

			UIElement* findChildAt(SDL_Point& pos);
			void draw(SDL_Renderer* ren) const;

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
			virtual void drawMe(SDL_Renderer* ren) const = 0;

			SDL_Rect bounds;
			std::vector<UIElement*> children;

		private:

	};
}

#endif // UIELEMENT_HPP
