#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#ifndef __EMSCRIPTEN__
#include <SDL_mixer.h>
#endif


#include "Window.hpp"
#include "UIElement.hpp"
#include "UIButton.hpp"
#include "UIContainerGrid.hpp"
#include "RootLayout.hpp"
#include "Canvas.hpp"

#define WIDTH 640
#define HEIGHT 480

using namespace drawApp;
using namespace std;

Window* window;
UIElement* rootElement;

class TestUIElement: public UIElement {
	public:
		TestUIElement(): UIElement( {
			0,0,0,0
		}) {}
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

		void updateChildSizes() {
			if (children.size() == 1) {
				UIElement* child1 = children[0];
				child1->setX(getX());
				child1->setY(getY());
				child1->setHeight(getHeight());
				child1->setWidth(64 + 15);
				child1->updateChildSizes();
			}
		}

	protected:
		void drawMe(SDL_Renderer* ren) {
			if (highlight) {
				SDL_SetRenderDrawColor(ren, 255,255,255,255);
				SDL_RenderFillRect(ren, &bounds);
				SDL_SetRenderDrawColor(ren, 0,0,0,255);
			} else {
				SDL_SetRenderDrawColor(ren, 0,100,200,255);
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

void tryMe() {
	cout << "hello, button" << endl;
}

int main ( int argc, char** argv ) {
	atexit(Quit);

	window = Window::getInstance("Hello World!", {100,100,WIDTH,HEIGHT});
	rootElement = RootLayout::getInstance(window);
	window->setRootElement(rootElement);

	window->notifyResize(WIDTH, HEIGHT);

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
						case SDLK_RETURN:
						case SDLK_ESCAPE:
							done = true;
							break;
						case SDLK_UP:
							window->setScaling(window->getScaling() + 0.25f);
							break;
						case SDLK_DOWN:
							window->setScaling(window->getScaling() - 0.25f);
							break;

					}
					break;
				case SDL_MOUSEWHEEL:
					window->notifyScroll(event.wheel.x, event.wheel.y);
					break;
				case SDL_WINDOWEVENT:
					switch (event.window.event) {
						case SDL_WINDOWEVENT_RESIZED:
							window->notifyResize(event.window.data1, event.window.data2);
							break;
					}
					break;
				case SDL_FINGERDOWN:
					window->notifyFingerDown(event.tfinger);
					break;
				case SDL_FINGERMOTION:
					window->notifyFingerMotion(event.tfinger);
					break;
				case SDL_FINGERUP:
					window->notifyFingerUp(event.tfinger);
					break;
				case SDL_MULTIGESTURE:
					window->notifyMultigesture(event.mgesture);
					break;


			}
		}

		window->update();

	}

	return 0;
}
