#include "stdafx.h"
#include "Element.h"

IMPLEMENT_SERIAL(Element, CObject, 0)
Element::Element() {
	color = RGB(0, 0, 0);
}

Element::~Element() {
}

void Element::Serialize(CArchive & ar) {
	CObject::Serialize(ar);
	if (ar.IsStoring()) {
		ar << color;
	} else {
		ar >> color;
	}
}

ElementType Element::GetElementType() {
	return ET_NONE;
}

void Element::Draw(CDC &dc) {
}

void Element::Draw(CDC &dc, COLORREF coverColor) {
}

Element * Element::Copy() {
	Element * newElement = new Element;
	newElement->color = this->color;
	return newElement;
}

bool Element::Select(CPoint point, unsigned short buffer) {
	return false;
}

bool Element::Select(CRect rangeRect) {
	return false;
}

void Element::Move(int deltaX, int deltaY) {
}

void Element::Zoom(CPoint center, double level) {
}

void Element::SymmetryH(int axisX) {
}

void Element::SymmetryV(int axisY) {
}

void Element::RotateCW(CPoint center) {
}

void Element::RotateCCW(CPoint center) {
}