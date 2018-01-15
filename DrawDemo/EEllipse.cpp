#include "stdafx.h"
#include "EEllipse.h"
#include <math.h>

IMPLEMENT_SERIAL(EEllipse, CObject, 1)
EEllipse::EEllipse() {
	lineWidth = 1;
	fillColor = RGB(255, 255, 255);
}

EEllipse::~EEllipse() {
}

void EEllipse::Serialize(CArchive & ar) {
	Element::Serialize(ar);
	pointA.Serialize(ar);
	pointB.Serialize(ar);
	if (ar.IsStoring()) {
		ar << lineWidth << fillColor;
	}
	else {
		ar >> lineWidth >> fillColor;
	}
}

ElementType EEllipse::GetElementType() {
	return ET_ELLIPSE;
}

void EEllipse::Set(CPoint pointA, CPoint pointB) {
	this->pointA.Set(pointA);
	this->pointB.Set(pointB);
}

void EEllipse::SetLineColor(COLORREF newLineColor) {
	color = newLineColor;
}

void EEllipse::SetFillColor(COLORREF newFillColor) {
	fillColor = newFillColor;
}

void EEllipse::SetLineWidth(unsigned short newLineWidth) {
	lineWidth = newLineWidth;
}

void EEllipse::Draw(CDC & dc) {
	Draw(dc, color);
}

void EEllipse::Draw(CDC & dc, COLORREF coverColor) {
	CBrush brush;
	brush.CreateSolidBrush(fillColor);
	CGdiObject *pOldBrush = dc.SelectObject(&brush);
	CPen pen;
	pen.CreatePen(PS_SOLID, lineWidth, coverColor);
	CPen *pOldPen = dc.SelectObject(&pen);

	dc.Ellipse(pointA.x, pointA.y, pointB.x, pointB.y);
	
	dc.SelectObject(pOldPen);
	dc.SelectObject(pOldBrush);
}

Element * EEllipse::Copy() {
	EEllipse * newEllipse = new EEllipse;
	newEllipse->color = this->color;
	newEllipse->pointA.x = this->pointA.x;
	newEllipse->pointA.y = this->pointA.y;
	newEllipse->pointB.x = this->pointB.x;
	newEllipse->pointB.y = this->pointB.y;
	newEllipse->lineWidth = this->lineWidth;
	newEllipse->fillColor = this->fillColor;
	return newEllipse;
}

bool EEllipse::Select(CPoint point, unsigned short buffer) {
	// 方程: ((x - x0) ^ 2 / a ^ 2) + ((y - y0) ^ 2 / b ^ 2) - 1 = 0
	bool result = false;
	buffer += lineWidth / 2;
	double centerX = (pointB.x + pointA.x) / 2;
	double centerY = (pointB.y + pointA.y) / 2;
	double a = fabs((pointA.x - pointB.x) / 2);
	double b = fabs((pointA.y - pointB.y) / 2);
	// 比较内外侧椭圆
	if (((((point.x - centerX) * (point.x - centerX)) / ((a - buffer) * (a - buffer))) + (((point.y - centerY) * (point.y - centerY)) / ((b - buffer) * (b - buffer))) - 1 >= 0) &&
		((((point.x - centerX) * (point.x - centerX)) / ((a + buffer) * (a + buffer))) + (((point.y - centerY) * (point.y - centerY)) / ((b + buffer) * (b + buffer))) - 1 <= 0)) {
		result = true;
	}

	return result;
}

bool EEllipse::Select(CRect rangeRect) {
	bool result = false;
	// 如果外接矩形与矩形不相交则不被选取
	int left   = pointA.x > pointB.x ? pointB.x : pointA.x;
	int top    = pointA.y > pointB.y ? pointB.y : pointA.y;
	int right  = pointA.x > pointB.x ? pointA.x : pointB.x;
	int bottom = pointA.y > pointB.y ? pointA.y : pointB.y;
	CRect diagRect(left, top, right, bottom);
	CRect testRect;
	if (testRect.IntersectRect(diagRect, rangeRect)) {
		// 如果有矩形顶点落在椭圆内部则被选取
		double buffer = lineWidth / 2;
		double centerX = (pointB.x + pointA.x) / 2;
		double centerY = (pointB.y + pointA.y) / 2;
		double a = fabs((pointA.x - pointB.x) / 2);
		double b = fabs((pointA.y - pointB.y) / 2);
		if (((((rangeRect.left  - centerX) * (rangeRect.left  - centerX)) / ((a + buffer) * (a + buffer))) + (((rangeRect.top    - centerY) * (rangeRect.top    - centerY)) / ((b + buffer) * (b + buffer))) - 1 <= 0) ||
			((((rangeRect.right - centerX) * (rangeRect.right - centerX)) / ((a + buffer) * (a + buffer))) + (((rangeRect.top    - centerY) * (rangeRect.top    - centerY)) / ((b + buffer) * (b + buffer))) - 1 <= 0) ||
			((((rangeRect.right - centerX) * (rangeRect.right - centerX)) / ((a + buffer) * (a + buffer))) + (((rangeRect.bottom - centerY) * (rangeRect.bottom - centerY)) / ((b + buffer) * (b + buffer))) - 1 <= 0) ||
			((((rangeRect.left  - centerX) * (rangeRect.left  - centerX)) / ((a + buffer) * (a + buffer))) + (((rangeRect.bottom - centerY) * (rangeRect.bottom - centerY)) / ((b + buffer) * (b + buffer))) - 1 <= 0)) {
			result = true;
		} else {
			// 如果矩形边线与椭圆相交则被选取
			// 点在椭圆内部的情况已经被排除，因此只需对比其是否分别在中线两侧
			if ((rangeRect.left < centerX && rangeRect.right > centerX) || (rangeRect.top < centerY && rangeRect.bottom > centerY)) {
				result = true;
			}
		}
	}

	return result;
}

void EEllipse::Move(int deltaX, int deltaY) {
	pointA.Move(deltaX, deltaY);
	pointB.Move(deltaX, deltaY);
}

void EEllipse::Zoom(CPoint center, double level) {
	this->pointA.Zoom(center, level);
	this->pointB.Zoom(center, level);
}

void EEllipse::SymmetryH(int axisX) {
	pointA.SymmetryH(axisX);
	pointB.SymmetryH(axisX);
}

void EEllipse::SymmetryV(int axisY) {
	pointA.SymmetryV(axisY);
	pointB.SymmetryV(axisY);
}

void EEllipse::RotateCW(CPoint center) {
	pointA.RotateCW(center);
	pointB.RotateCW(center);
}

void EEllipse::RotateCCW(CPoint center) {
	pointA.RotateCCW(center);
	pointB.RotateCCW(center);
}