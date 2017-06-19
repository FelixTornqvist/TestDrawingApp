#include <iostream>

#include "RootLayout.hpp"
#include "UIButton.hpp"

namespace drawApp {

	RootLayout* RootLayout::getInstance(Window* win) {
		return new RootLayout(win);
	}

	RootLayout::~RootLayout() {

	}

	RootLayout::RootLayout(Window* win): UIElement( {
		0,0,0,0
	}) {
		canvas = Canvas::getInstance({0,0,0,0}, 640, 480, win->getRenderer());

		Texture* brush = Texture::createFromFile("res/brush.png", win->getRenderer());
		brush->setBlendMode(SDL_BLENDMODE_BLEND);
		canvas->setBrush(brush);

		tools = UIContainerGrid::getInstance(2, true, 32);

		addChild(canvas);
		addChild(tools);

		tools->setXSpacing(5);
		tools->setYSpacing(5);
		tools->addChild(UIButton::getInstance("B1", std::bind(&RootLayout::tryMe, this)));
	}

	void RootLayout::updateChildSizes() {
		tools->setX(getX());
		tools->setY(getY());
		tools->setHeight(getHeight());
		tools->setWidth(64 + 15);
		tools->updateChildSizes();

		canvas->setX(getX() + tools->getWidth());
		canvas->setY(getY());
		canvas->setWidth(getWidth() - tools->getWidth());
		canvas->setHeight(getHeight());
		canvas->updateChildSizes();
	}

	void RootLayout::tryMe() {
		std::cout << "Rootlayout test" << std::endl;
	}


}
