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

	Texture* tex;
	try {
		tex = Texture::createFromFile("cb.bmp", ren);
	} catch (exception& e) {
		cout << "could not create/load texture: " << e.what() << endl;
		return -1;
	}

	bool done = false;
	while (!done) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {

			switch (event.type) {
				case SDL_QUIT:
					done = true;
					break;
				case SDL_KEYDOWN:
					if (event.key.keysym.sym == SDLK_ESCAPE)
						done = true;
					break;

			}
		}

		SDL_RenderClear(ren);

		tex->render(ren, NULL, NULL);

		SDL_RenderPresent(ren);
		SDL_Delay(1000/60);
	}

	delete tex;
	printf("Exited cleanly\n");
	return 0;
}
