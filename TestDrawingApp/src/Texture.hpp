#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include "string"
#include <SDL.h>

namespace drawApp {

    // Used a lot of inspiration from http://lazyfoo.net/tutorials/SDL/

	class Texture {
		public:
			static Texture* createFromFile(std::string path, SDL_Renderer* ren);
			static Texture* createBlank(int width, int height, SDL_Renderer* ren, SDL_TextureAccess access = SDL_TEXTUREACCESS_STREAMING);

			void render(SDL_Renderer* ren, SDL_Rect* src, SDL_Rect* dest);
			void setAsRenderTarget(SDL_Renderer* ren);

			int getWidth();
			int getHeight();

			bool lockTexture();
			bool unlockTexture();
			void* getPixels();
			int getPitch();

			virtual ~Texture();
		protected:
			Texture();

		private:
			//The actual hardware texture
			SDL_Texture* mTexture;
			void* mPixels;
			int mPitch;

			//Image dimensions
			int mWidth;
			int mHeight;
	};
}

#endif // TEXTURE_HPP
