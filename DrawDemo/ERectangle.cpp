#include "stdafx.h"
#include "ERectangle.h"
#include "ELine.h"

IMPLEMENT_SERIAL(ERectangle, CObject, 1)
ERectangle::ERectangle() {
	lineWidth = unsigned short(1);
	fillColor = RGB(255, 255, 255);
}

ERectangle::~ERectangle() {
}

void ERectangle::Serialize(CArchive & ar) {
	Element::Serialize(ar);
	pointA.Serialize(ar);
	pointB.Serialize(ar);
	if (ar.IsStoring()) {
		ar << lineWidth << fillColor;
	} else {
		ar >> lineWidth >> fillColor;
	}
}

ElementType ERectangle::GetElementType() {
	return ET_RECTANGLE;
}

void ERectangle::Set(CPoint A, CPoint B) {
	pointA.Set(A);
	pointB.Set(B);
}

void ERectangle::SetLineColor(COLORREF newLineColor) {
	color = newLineColor;
}

void ERectangle::SetFillColor(COLORREF newFillColor) {
	fillColor = newFillColor;
}

void ERectangle::SetLineWidth(unsigned short newLineWidth) {
	lineWidth = newLineWidth;
}

void ERectangle::Draw(CDC & dc) {
	Draw(dc, color);
}

void ERectangle::Draw(CDC & dc, COLORREF  coverColor) {
	CBrush brush;
	brush.CreateSolidBrush(fillColor);
	CGdiObject *pOldBrush = dc.SelectObject(&brush);
	CPen pen;
	pen.CreatePen(PS_SOLID, lineWidth, coverColor);
	CPen *pOldPen = dc.SelectObject(&pen);

	dc.Rectangle(pointA.x, pointA.y, pointB.x, pointB.y);
	
	dc.SelectObject(pOldPen);
	dc.SelectObject(pOldBrush);
}

Element * ERectangle::Copy() {
	ERectangle * newRectangle = new ERectangle;
	newRectangle->color = this->color;
	newRectangle->pointA.x = this->pointA.x;
	newRectangle->pointA.y = this->pointA.y;
	newRectangle->pointB.x = this->pointB.x;
	newRectangle->pointB.y = this->pointB.y;
	newRectangle->lineWidth = this->lineWidth;
	newRectangle->fillColor = this->fillColor;
	return newRectangle;
}

bool ERectangle::Select(CPoint point, unsigned short buffer) {
	ELine edgeLine;
	CPoint vertex;
	vertex.x = pointA.x;
	vertex.y = pointB.y;
	edgeLine.Set(pointA.GetCPoint(), vertex);
	if (edgeLine.Select(point, buffer)) return true;

	edgeLine.Set(vertex, pointB.GetCPoint());
	if (edgeLine.Select(point, buffer)) return true;

	vertex.x = pointB.x;
	vertex.y = pointA.y;
	edgeLine.Set(pointB.GetCPoint(), vertex);
	if (edgeLine.Select(point, buffer)) return true;

	edgeLine.Set(vertex, pointA.GetCPoint());
	if (edgeLine.Select(point, buffer)) return true;

	return false;
}

bool ERectangle::Select(CRect rangeRect) {
	int left   = pointA.x > pointB.x ? pointB.x : pointA.x;
	int top    = pointA.y > pointB.y ? pointB.y : pointA.y;
	int right  = pointA.x > pointB.x ? pointA.x : pointB.x;
	int bottom = pointA.y > pointB.y ? pointA.y : pointB.y;
	CRect thisRect(left, top, right, bottom);
	CRect testRect;
	return testRect.IntersectRect(thisRect, rangeRect);
}

void ERectangle::Move(int deltaX, int deltaY) {
	pointA.Move(deltaX, deltaY);
	pointB.Move(deltaX, deltaY);
}

void ERectangle::Zoom(CPoint center, double level) {
	pointA.Zoom(center, level);
	pointB.Zoom(center, level);
}

void ERectangle::SymmetryH(int axisX) {
	pointA.SymmetryH(axisX);
	pointB.SymmetryH(axisX);
}

void ERectangle::SymmetryV(int axisY) {
	pointA.SymmetryV(axisY);
	pointB.SymmetryV(axisY);
}

void ERectangle::RotateCW(CPoint center) {
	pointA.RotateCW(center);
	pointB.RotateCW(center);
}

void ERectangle::RotateCCW(CPoint center) {
	pointA.RotateCCW(center);
	pointB.RotateCCW(center);
}