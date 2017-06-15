#ifndef UIBUTTON_HPP
#define UIBUTTON_HPP

#include <string>
#include <functional>

#include "UIElement.hpp"
#include "Texture.hpp"

namespace drawApp {
	class UIButton : public UIElement {
		public:
			static UIButton* getInstance(SDL_Rect& bounds, std::string label, std::string bg, std::function<void()> callback);
			virtual ~UIButton();

			void setText(std::string label);

			virtual void mouseDown(Uint32 btn, SDL_Point& pos) override;
			virtual void mouseUp(SDL_Point& pos) override;

		protected:
			UIButton(SDL_Rect& bounds, std::string label, std::string bg, std::function<void()> callback);

		private:
			bool LMBClicked = false;
			Texture *background, *text;
			std::function<void()> callback;
	};
}
#endif // UIBUTTON_HPP
