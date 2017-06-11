#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

#include "Window.hpp"
#include "UIElement.hpp"
#include "Canvas.hpp"

using namespace drawApp;
using namespace std;

Window* window;
UIElement* rootElement;

class TestUIElement: public UIElement {
	public:
		TestUIElement(const SDL_Rect& pos): UIElement(pos) {}

		void mouseDown(Uint32 btn, SDL_Point& pos) {
			if (btn & SDL_BUTTON(SDL_BUTTON_LEFT))
				highlight = false;
			else
				highlight = true;
		}

		void mouseUp(SDL_Point& pos) {
//			highlight = false;
		}

		void mouseDragged(Uint32 btn, SDL_Point& pos) {
			cout << this << " dragged: " << pos.x << ":" << pos.y << endl;
		}

		void mouseHoverOn(SDL_Point& pos) {
			highlight = true;
		}

		void mouseHoverOff() {
			highlight = false;
		}

	protected:
		void drawMe(SDL_Renderer* ren) {
			if (highlight) {
				SDL_SetRenderDrawColor(ren, 255,255,255,255);
				SDL_RenderFillRect(ren, &bounds);
				SDL_SetRenderDrawColor(ren, 0,0,0,255);
			} else {
				SDL_SetRenderDrawColor(ren, 0,0,0,255);
				SDL_RenderFillRect(ren, &bounds);
				SDL_SetRenderDrawColor(ren, 255,255,255,255);
				SDL_RenderDrawRect(ren, &bounds);
				SDL_SetRenderDrawColor(ren, 0,0,0,255);
			}
		}

	private:
		bool highlight = false;
};

void Quit() {
	delete window;
	delete rootElement;
	printf("Exited cleanly\n");
}

int main ( int argc, char** argv ) {
	atexit(Quit);

	window = Window::getInstance("Hello World!", {100,100,640,480});
	rootElement = new TestUIElement({10,10,620,460});
	window->setRootElement(rootElement);

	Canvas* canvas = Canvas::getInstance({20,20, 600, 440}, 600,440, window->getRenderer());
	Texture* testBrush = Texture::createFromFile("brush.png", window->getRenderer());
	testBrush->setBlendMode(SDL_BLENDMODE_BLEND);
	canvas->setBrush(testBrush);

	rootElement->addChild(canvas);

	bool done = false;
	while (!done) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {

			switch (event.type) {
				case SDL_QUIT:
					done = true;
					break;
				case SDL_KEYDOWN:
					switch (event.key.keysym.sym) {
						case SDLK_ESCAPE:
							done = true;
							break;

					}
					break;
			}
		}

		window->update();

	}

	delete testBrush;

	return 0;
}
