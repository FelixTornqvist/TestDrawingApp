#ifndef UIBUTTON_HPP
#define UIBUTTON_HPP

#include <string>
#include <functional>

#include "UIElement.hpp"
#include "Texture.hpp"
#include "Window.hpp"

#define DEFAULT_UIBUTTON_TEXTURE "res/defaultButton.png"

namespace drawApp {
	class UIButton : public UIElement {
		public:
			static UIButton* getInstance(const SDL_Rect& bounds, std::string label, Texture* bg, std::function<void()> callback);
			static UIButton* getInstance(std::string label, std::function<void()> callback);
			virtual ~UIButton();

			void setText(std::string label);
			void updateChildSizes() {}
			void drawMe(SDL_Renderer* ren);

			virtual void mouseDown(Uint32 btn, SDL_Point& pos) override;
			virtual void mouseUp(SDL_Point& pos) override;

		protected:
			UIButton(const SDL_Rect& bounds, std::string label, Texture* bg, std::function<void()> callback);

		private:
			bool LMBClicked = false;
			static Texture* defaultTexture;
			Texture *background, *text;
			std::function<void()> callback;

			friend Window* Window::getInstance(std::string, SDL_Rect);
			static void loadDefaulTexture(SDL_Renderer* ren);
	};
}
#endif // UIBUTTON_HPP
