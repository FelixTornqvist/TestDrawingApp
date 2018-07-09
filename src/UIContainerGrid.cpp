#include <iostream>
#include <cmath>

#include "UIContainerGrid.hpp"

namespace drawApp {

	UIContainerGrid* UIContainerGrid::getInstance(int _rowCols, bool _orderByCols, int _childSize) {
		return new UIContainerGrid({0,0,0,0}, _rowCols, _orderByCols, _childSize);
	}

	UIContainerGrid* UIContainerGrid::getInstance(const SDL_Rect& bounds, int _rowCols, bool _orderByCols, int _childSize) {
		return new UIContainerGrid(bounds, _rowCols, _orderByCols, _childSize);
	}

	UIContainerGrid::UIContainerGrid(const SDL_Rect& bounds, int _rowCols, bool _orderByCols, int _childSize):
		UIElement(bounds), rowCols(_rowCols), childSize(_childSize), orderByCols(_orderByCols) {
		xSpacing = 0;
		ySpacing = 0;
	}

	UIContainerGrid::~UIContainerGrid() {
	}

	void UIContainerGrid::updateChildSizes() {
		orderChildren();
		for (UIElement* child : children) {
			child->updateChildSizes();
		}
	}

	void UIContainerGrid::orderChildren() {
		int childrenAmt = children.size();
		int height = ceil(childrenAmt / (rowCols * 1.0f));

		int childWidth = orderByCols ? ((getWidth() / rowCols) - xSpacing - (xSpacing / rowCols) ) : childSize;
		int childHeight = !orderByCols ? ((getHeight() / rowCols) - ySpacing - (ySpacing / rowCols)) : childSize;

		for (int x = 0; x < rowCols; x++) {
			for (int y = 0; y < height; y++) {
				int index = x + rowCols * y;
				if (index >= childrenAmt)
					continue;

				UIElement* child = children[index];
				child->setWidth(childWidth);
				child->setHeight(childHeight);

				if (orderByCols) {
					child->setX(getX() + (x * (childWidth + xSpacing)) + xSpacing );
					child->setY(getY() + (y * (childHeight + ySpacing)) + ySpacing);
				} else {
					child->setX(getX() + (y * (childWidth + xSpacing)) + xSpacing);
					child->setY(getY() + (x * (childHeight + ySpacing)) + ySpacing);
				}
			}
		}
	}

	void UIContainerGrid::setAmtOfRowsOrColumns(int _rowCols) {
		rowCols = _rowCols;
	}

	void UIContainerGrid::setOrderByCols(bool byCols) {
		orderByCols = byCols;
	}

	void UIContainerGrid::setXSpacing(int xSpace) {
		xSpacing = xSpace;
	}

	void UIContainerGrid::setYSpacing(int ySpace) {
		ySpacing = ySpace;
	}

	void UIContainerGrid::setChildSize(int size) {
		childSize = size;
	}

}
