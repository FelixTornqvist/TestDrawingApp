#ifndef CANVAS_HPP
#define CANVAS_HPP

#include "SDL.h"

#include "UIElement.hpp"
#include "Texture.hpp"

namespace drawApp {
	class Canvas: public UIElement {
		public:
			static Canvas* getInstance(const SDL_Rect& bounds, int width, int height, SDL_Renderer* ren);
			virtual ~Canvas();

			void mouseDown(Uint32 btn, SDL_Point& pos);
			void mouseDragged(Uint32 btn, SDL_Point& pos);
			void mouseUp(SDL_Point& pos);

			void drawLine(SDL_Point from, SDL_Point to, int skippedPixels, SDL_Renderer* ren);
			virtual void drawMe(SDL_Renderer* ren) override;

			void setBrush(Texture* tex);
			void setBrushSize(int radius);
			void setBrushSkippedPixels(int skippedPixels);

			SDL_Point convertGlobalToLocal(SDL_Point p);

		protected:
			Canvas(const SDL_Rect& bounds, int width, int height, SDL_Renderer* ren);

		private:
			Texture *content, *brush = nullptr;
			SDL_Rect brushDest;
			SDL_Rect contentSrc;

			int brushSkippedPixels = 1;
			bool brushDown = false;
			SDL_Point lastPos, currPos;
	};
}

#endif // CANVAS_HPP
