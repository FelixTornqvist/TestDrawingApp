#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

#include "Window.hpp"
#include "UIElement.hpp"
#include "UIContainerGrid.hpp"
#include "Canvas.hpp"

using namespace drawApp;
using namespace std;

Window* window;
UIElement* rootElement;

class TestUIElement: public UIElement {
	public:
		TestUIElement(): UIElement({0,0,0,0}) {}
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
				child1->setWidth(50);
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

int main ( int argc, char** argv ) {
	atexit(Quit);

	window = Window::getInstance("Hello World!", {100,100,640,480});
	rootElement = new TestUIElement({10,10,620*2,460*2});
	window->setRootElement(rootElement);

	Canvas* canvas = Canvas::getInstance({20,20, 600, 440}, 600,440, window->getRenderer());
	Texture* testBrush = Texture::createFromFile("brush.png", window->getRenderer());
	testBrush->setBlendMode(SDL_BLENDMODE_BLEND);
	canvas->setBrush(testBrush);

	UIContainerGrid* grid = UIContainerGrid::getInstance({20, 20, 100,100}, 2, true, 20);
	grid->setXSpacing(5);
	grid->setYSpacing(5);
	grid->addChild(new TestUIElement());
	grid->addChild(new TestUIElement());
	grid->addChild(new TestUIElement());
	grid->addChild(new TestUIElement());
	grid->addChild(new TestUIElement());
	grid->orderChildren();

//	rootElement->addChild(canvas);
	rootElement->addChild(grid);

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
						case SDLK_DELETE:
						case SDLK_BACKSPACE:
							canvas->clearCanvas(window->getRenderer());
							break;

					}
					break;
				case SDL_MOUSEWHEEL:
					testBrush->setAlpha(testBrush->getAlpha() + event.wheel.y);
					canvas->setBrushSize(canvas->getBrushSize() + event.wheel.x);
					cout << "Alpha: " << (int) testBrush->getAlpha() << " Size: " << canvas->getBrushSize() << endl;
					break;
				case SDL_WINDOWEVENT:
					switch (event.window.event) {
						case SDL_WINDOWEVENT_RESIZED:
							cout << "resized" << endl;
							window->notifyResize(event.window.data1, event.window.data2);
							break;
					}

			}
		}

		window->update();

	}

	delete testBrush;

	return 0;
}
