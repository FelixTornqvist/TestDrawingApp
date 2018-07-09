#include <stdexcept>
#include <SDL_image.h>

#include "Texture.hpp"

namespace drawApp {

	Texture::Texture() {
		mTexture = nullptr;
		mPixels = nullptr;
		mPitch = 0;
		mWidth = 0;
		mHeight = 0;
		red = green = blue = alpha = 255;
	}

	Texture::~Texture() {
		SDL_DestroyTexture(mTexture);
	}

	Texture* Texture::createFromFile(std::string path, SDL_Renderer* ren) {
		SDL_Surface* loadedSurface = IMG_Load( path.c_str() );

		if( loadedSurface == NULL ) {
			throw std::runtime_error(IMG_GetError());
		} else {
			SDL_Surface* formattedSurface = SDL_ConvertSurfaceFormat(loadedSurface, SDL_PIXELFORMAT_RGBA8888, 0);
			SDL_FreeSurface(loadedSurface);

			if( formattedSurface == NULL ) {
				throw std::runtime_error(SDL_GetError());
			} else {
				SDL_Texture* newTexture = SDL_CreateTexture( ren, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, formattedSurface->w, formattedSurface->h );

				if( newTexture == NULL ) {
					throw std::runtime_error(SDL_GetError());
				} else {
					Texture* newTex = new Texture();
					SDL_LockTexture(newTexture, NULL, &(newTex->mPixels), &(newTex->mPitch));

					memcpy(newTex->mPixels, formattedSurface->pixels, formattedSurface->pitch * formattedSurface->h);

					SDL_UnlockTexture(newTexture);
					newTex->mPixels = NULL;
					newTex->mWidth = formattedSurface->w;
					newTex->mHeight = formattedSurface->h;
					newTex->mTexture = newTexture;

					SDL_FreeSurface(formattedSurface);
					return newTex;
				}

				SDL_FreeSurface(formattedSurface);
			}
		}

		return nullptr;

	}

	Texture* Texture::createBlank(int width, int height, SDL_Renderer* ren, SDL_TextureAccess access) {
		Texture* newTex = nullptr;

		SDL_Texture* tex = SDL_CreateTexture(ren, SDL_PIXELFORMAT_RGBA8888, access, width, height );
		if( tex != NULL ) {
			newTex = new Texture();
			newTex->mTexture = tex;
			newTex->mWidth = width;
			newTex->mHeight = height;
		} else {
			throw std::runtime_error(SDL_GetError());
		}

		return newTex;
	}

	void Texture::render(SDL_Renderer* ren, const SDL_Rect* src, const SDL_Rect* dest) {
		SDL_RenderCopy(ren, mTexture, src, dest);
	}

	/** \brief Render to this texture. Use SDL_SetRenderTarget(renderer, NULL) to reset render target.
	 */
	void Texture::setAsRenderTarget(SDL_Renderer* ren) {
		SDL_SetRenderTarget(ren, mTexture);
	}

	void Texture::setBlendMode(SDL_BlendMode blending) {
		SDL_SetTextureBlendMode(mTexture, blending);
	}

	void Texture::setColor( Uint8 r, Uint8 g, Uint8 b ) {
		SDL_SetTextureColorMod(mTexture, r, g, b);
		red = r;
		green = g;
		blue = b;
	}

	void Texture::setAlpha(Uint8 a) {
		SDL_SetTextureAlphaMod(mTexture, a);
		alpha = a;
	}

	Uint8 Texture::getRed() const {
		return red;
	}

	Uint8 Texture::getGreen() const {
		return green;
	}

	Uint8 Texture::getBlue() const {
		return blue;
	}

	Uint8 Texture::getAlpha() const {
		return alpha;
	}



	int Texture::getWidth() {
		return mWidth;
	}

	int Texture::getHeight() {
		return mHeight;
	}

	bool Texture::lockTexture() {
		bool success = true;

		if( mPixels != NULL ) {
			printf( "Texture is already locked!\n" );
			success = false;
		} else if( SDL_LockTexture( mTexture, NULL, &mPixels, &mPitch ) != 0 ) {
			printf( "Unable to lock texture! %s\n", SDL_GetError() );
			success = false;
		}
		return success;
	}

	bool Texture::unlockTexture() {
		bool success = true;

		if( mPixels == NULL ) {
			printf( "Texture is not locked!\n" );
			success = false;
		} else {
			SDL_UnlockTexture( mTexture );
			mPixels = NULL;
			mPitch = 0;
		}
		return success;
	}

	void* Texture::getPixels() {
		return mPixels;
	}

	int Texture::getPitch() {
		return mPitch;
	}
}
