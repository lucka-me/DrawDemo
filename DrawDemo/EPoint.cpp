#include "stdafx.h"
#include "EPoint.h"

IMPLEMENT_SERIAL(EPoint, CObject, 0)
EPoint::EPoint() {
	x = 0;
	y = 0;
	color = RGB(0, 0, 0);
}

EPoint::~EPoint() {
}

void EPoint::Serialize(CArchive & ar) {
	Element::Serialize(ar);
	if (ar.IsStoring()) {
		ar << x << y;
	} else {
		ar >> x >> y;
	}
}

ElementType EPoint::GetElementType() {
	return ET_POINT;
}

void EPoint::Set(CPoint point) {
	x = point.x;
	y = point.y;
}

void EPoint::SetColor(COLORREF newColor) {
	color = newColor;
}

CPoint EPoint::GetCPoint() {
	CPoint result;
	result.x = x;
	result.y = y;
	return result;
}

void EPoint::Draw(CDC &dc) {
	Draw(dc, color);
}

void EPoint::Draw(CDC &dc, COLORREF coverColor) {
	CGdiObject *pOldBrush = dc.SelectStockObject(NULL_BRUSH);
	CPen pen;
	pen.CreatePen(PS_SOLID, 1, coverColor);
	CPen *pOldPen = dc.SelectObject(&pen);

	dc.Rectangle(x - 2, y - 2, x + 2, y + 2);
	
	dc.SelectObject(pOldPen);
	dc.SelectObject(pOldBrush);
}

Element * EPoint::Copy() {
	EPoint * newPoint = new EPoint;
	newPoint->color = this->color;
	newPoint->x = this->x;
	newPoint->y = this->y;
	return newPoint;
}

bool EPoint::Select(CPoint point, unsigned short buffer) {
	// 返回是否被选择
	// 缓冲区为纵横正负2像素
	return (point.x >= x - buffer) &&
		   (point.x <= x + buffer) &&
		   (point.y >= y - buffer) &&
		   (point.y <= y + buffer);
}

bool EPoint::Select(CRect rangeRect) {
	bool result = false;
	if (rangeRect.PtInRect(GetCPoint())) {
		result = true;
	}
	return result;
}

void EPoint::Move(int deltaX, int deltaY) {
	x += deltaX;
	y += deltaY;
}

void EPoint::Zoom(CPoint center, double level) {
	x = center.x + (x - center.x) * level;
	y = center.y + (y - center.y) * level;
}

void EPoint::SymmetryH(int axisX) {
	x = 2 * axisX - x;
}

void EPoint::SymmetryV(int axisY) {
	y = 2 * axisY - y;
}

void EPoint::RotateCW(CPoint center) {
	int tempX = x;
	int tempY = y;
	x = center.x - (tempY - center.y);
	y = center.y + (tempX - center.x);
}

void EPoint::RotateCCW(CPoint center) {
	int tempX = x;
	int tempY = y;
	x = center.x + (tempY - center.y);
	y = center.y - (tempX - center.x);
}