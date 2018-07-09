#ifndef UICONTAINER_HPP
#define UICONTAINER_HPP

#include <UIElement.hpp>


namespace drawApp {
	class UIContainerGrid : public UIElement {
		public:
			static UIContainerGrid* getInstance(int _rowCols, bool orderByCols, int _childSize);
			static UIContainerGrid* getInstance(const SDL_Rect& bounds, int _rowCols, bool orderByCols, int _childSize);
			virtual ~UIContainerGrid();

			virtual void drawMe(SDL_Renderer* ren) {
				SDL_SetRenderDrawColor(ren, 0,255,255,255);
				SDL_RenderFillRect(ren, &bounds);
				SDL_SetRenderDrawColor(ren, 0,0,0,255);
			}
			void updateChildSizes();

			void orderChildren();
			void setAmtOfRowsOrColumns(int rowCols);
			void setOrderByCols(bool byCols);
			void setXSpacing(int xSpace);
			void setYSpacing(int ySpace);
			void setChildSize(int size);

		protected:
			UIContainerGrid(const SDL_Rect& bounds, int _rowCols, bool orderByCols, int _childSize);

		private:
			int rowCols;
			int xSpacing, ySpacing;
			bool orderByCols;
			int childSize;
	};
}
#endif // UICONTAINER_HPP
