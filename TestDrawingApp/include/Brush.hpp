#ifndef BRUSH_HPP
#define BRUSH_HPP

#include "Texture.hpp"

namespace drawApp {
	class Brush {
		public:
			static Brush* getInstance(std::string texPath, SDL_Renderer* ren);
			virtual ~Brush();

			virtual void mouseDown(Uint32 button, SDL_Point& pos);
			virtual void mouseDragged(Uint32 button, SDL_Point& pos);
			virtual void mouseUp(SDL_Point& pos);

			void setSpacing(int _spacing);
			int getSpacing();

		protected:
			Brush(std::string texPath, SDL_Renderer* ren);

		private:
			SDL_Renderer* ren;
			int spacing = 1;
			SDL_Rect brushDest = {0,0,10,10};

			Texture* brushTex;
			bool brushDown = false;
			SDL_Point currPos, lastPos;

			void renderLine(SDL_Point p1, SDL_Point p2);

	};
}
#endif // BRUSH_HPP
