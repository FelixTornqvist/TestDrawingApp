#include <iostream>
#include <SDL.h>

#include "Texture.hpp"

using namespace drawApp;
using namespace std;

int main ( int argc, char** argv ) {
	if ( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
		printf( "Unable to init SDL: %s\n", SDL_GetError() );
		return 1;
	}

	atexit(SDL_Quit);

	SDL_Window* screen;
	SDL_Renderer* ren;

	if ( SDL_CreateWindowAndRenderer(640, 480, 0, &screen, &ren) == -1) {
		printf("Unable to set 640x480 video: %s\n", SDL_GetError());
		return 1;
	}

	Texture* cbImg = Texture::createFromFile("cb.png", ren);
	SDL_Rect cbDest = {0, 0, cbImg->getWidth(), cbImg->getHeight()};
	cbImg->setBlendMode(SDL_BLENDMODE_BLEND);
	cbImg->setColor(0,255,0);

	Texture* renderTo = Texture::createBlank(2000,1000,ren,SDL_TEXTUREACCESS_TARGET);
	SDL_Rect renderToDst = {100,100,400,300};

	bool bounceUp = false;
	int speed = 10;
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
						case SDLK_w:
							cbDest.y -= speed;
							break;
						case SDLK_a:
							cbDest.x -= speed;
							break;
						case SDLK_s:
							cbDest.y += speed;
							break;
						case SDLK_d:
							cbDest.x += speed;
							break;

					}
					break;

			}
		}
		if (!bounceUp) {
			renderToDst.y++;
			if (renderToDst.y >= 150)
				bounceUp = true;
		} else {
			renderToDst.y--;
			if (renderToDst.y <= 100)
				bounceUp = false;
		}

		SDL_RenderClear(ren);

		renderTo->setAsRenderTarget(ren);
		cbImg->setColor((cbDest.x % 200) + 55, (cbDest.y % 200) + 55, 128);
		cbImg->render(ren, NULL, &cbDest);
		cbImg->setColor(255,255,255);
		SDL_SetRenderTarget(ren, NULL);

		cbImg->render(ren,NULL,NULL);
		renderTo->render(ren,NULL,&renderToDst);

		SDL_RenderPresent(ren);
		SDL_Delay(1000/60);
	}

	delete cbImg;
	printf("Exited cleanly\n");
	return 0;
}
