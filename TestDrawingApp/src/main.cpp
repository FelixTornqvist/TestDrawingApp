#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

#include "Window.hpp"
#include "UIElement.hpp"
#include "Texture.hpp"

using namespace drawApp;
using namespace std;

Window* window;
UIElement* rootElement;

class TestUIElement: public UIElement {
	public:
		TestUIElement(const SDL_Rect& pos): UIElement(pos) {}

		void mouseDown(Uint8 btn, SDL_Point& pos) {
			highlight = false;
		}

		void mouseUp(SDL_Point& pos) {
			highlight = false;
		}

		void mouseHoverOn(SDL_Point& pos) {
			highlight = true;
		}

		void mouseHoverOff() {
			highlight = false;
		}

	protected:
		void drawMe(SDL_Renderer* ren) const {
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

	UIElement* ele1 = new TestUIElement({20,20,100,100});
	UIElement* ele1c1 = new TestUIElement({30,20,10,10});
	UIElement* ele1c2 = new TestUIElement({50,20,10,10});
	UIElement* ele1c3 = new TestUIElement({100,20,100,10});
	ele1->addChild(ele1c1);
	ele1->addChild(ele1c2);
	ele1->addChild(ele1c3);

	UIElement* ele2 = new TestUIElement({200,200,100,100});
	rootElement->addChild(ele1);
	rootElement->addChild(ele2);

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

	return 0;
}
