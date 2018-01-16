#include "stdafx.h"
#include "EPolygon.h"

IMPLEMENT_SERIAL(EPolygon, CObject, 1)
EPolygon::EPolygon() {
	lineWidth = 1;
	fillColor = RGB(255, 255, 255);
}


EPolygon::~EPolygon() {
}

void EPolygon::Serialize(CArchive & ar) {
	Element::Serialize(ar);

	nodeList.Serialize(ar);
	if (ar.IsStoring()) {
		ar << lineWidth;
	}
	else {
		ar >> lineWidth;

	}
}

ElementType EPolygon::GetElementType() {
	return ET_POLYGON;
}

void EPolygon::Set(CPoint startNode, CPoint secondNode) {
	nodeList.RemoveAll();
	Add(startNode);
	Add(secondNode);
}

void EPolygon::Add(CPoint newNode) {
	EPoint *pNewNode = new EPoint;
	pNewNode->Set(newNode);
	pNewNode->SetColor(color);
	nodeList.Add(pNewNode);
}

void EPolygon::SetLineColor(COLORREF newColor) {
	color = newColor;
}

void EPolygon::SetLineWidth(unsigned short newWidth) {
	lineWidth = newWidth;
}

void EPolygon::SetFillColor(COLORREF newFillColor) {
	fillColor = newFillColor;
}

CPoint * EPolygon::GetCPointList() {
	CPoint * pList = new CPoint[nodeList.GetSize()];
	for (int i = 0; i < nodeList.GetSize(); i++) {
		EPoint * point = (EPoint *)nodeList[i];
		pList[i] = point->GetCPoint();
	}
	return pList;
}

void EPolygon::Draw(CDC &dc) {
	Draw(dc, color);
}

void EPolygon::Draw(CDC &dc, COLORREF coverColor) {
	CBrush brush;
	brush.CreateSolidBrush(fillColor);
	CGdiObject *pOldBrush = dc.SelectObject(&brush);
	CPen pen;
	pen.CreatePen(PS_SOLID, lineWidth, coverColor);
	CPen *pOldPen = dc.SelectObject(&pen);

	CPoint * pList = GetCPointList();
	dc.Polygon(pList, nodeList.GetSize());

	dc.SelectObject(pOldPen);
	dc.SelectObject(pOldBrush);
}

void EPolygon::DrawLast(CDC & dc) {
	CGdiObject *pOldBrush = dc.SelectStockObject(NULL_BRUSH);
	CPen pen;
	pen.CreatePen(PS_SOLID, lineWidth, color);
	CPen *pOldPen = dc.SelectObject(&pen);

	EPoint * pStart = (EPoint *)nodeList[nodeList.GetSize() - 2];
	EPoint * pEnd = (EPoint *)nodeList[nodeList.GetSize() - 1];
	dc.MoveTo(pStart->x, pStart->y);
	dc.LineTo(pEnd->x, pEnd->y);

	dc.SelectObject(pOldPen);
	dc.SelectObject(pOldBrush);
}

Element * EPolygon::Copy() {
	EPolygon * newPolygon = new EPolygon;
	newPolygon->color = this->color;
	newPolygon->lineWidth = this->lineWidth;
	for (int i = 0; i < nodeList.GetSize(); i++) {
		EPoint *pNode = (EPoint *)nodeList[i];
		newPolygon->Add(pNode->GetCPoint());
	}
	return newPolygon;
}

bool EPolygon::Select(CPoint point, unsigned short buffer) {
	bool result = false;
	buffer += lineWidth / 2;
	EPoint *pStart = (EPoint *)nodeList[0];
	for (int i = 1; i < nodeList.GetSize(); i++) {
		EPoint *pEnd = (EPoint *)nodeList[i];
		ELine line;
		line.Set(pStart->GetCPoint(), pEnd->GetCPoint());
		line.SetLineWidth(lineWidth);
		if (line.Select(point, buffer)) {
			result = true;
			break;
		}
		else {
			pStart = pEnd;
		}
	}
	return result;
}

bool EPolygon::Select(CRect rangeRect) {
	bool result = false;
	EPoint *pStart = (EPoint *)nodeList[0];
	for (int i = 1; i < nodeList.GetSize(); i++) {
		EPoint *pEnd = (EPoint *)nodeList[i];
		ELine line;
		line.Set(pStart->GetCPoint(), pEnd->GetCPoint());
		line.SetLineWidth(lineWidth);
		if (line.Select(rangeRect)) {
			result = true;
			break;
		}
		else {
			pStart = pEnd;
		}
	}
	return result;
}

void EPolygon::Move(int deltaX, int deltaY) {
	for (int i = 0; i < nodeList.GetSize(); i++) {
		nodeList[i]->Move(deltaX, deltaY);
	}
}

void EPolygon::Zoom(CPoint center, double level) {
	for (int i = 0; i < nodeList.GetSize(); i++) {
		nodeList[i]->Zoom(center, level);
	}
}

void EPolygon::SymmetryH(int axisX) {
	for (int i = 0; i < nodeList.GetSize(); i++) {
		nodeList[i]->SymmetryH(axisX);
	}
}

void EPolygon::SymmetryV(int axisY) {
	for (int i = 0; i < nodeList.GetSize(); i++) {
		nodeList[i]->SymmetryV(axisY);
	}
}

void EPolygon::RotateCW(CPoint center) {
	for (int i = 0; i < nodeList.GetSize(); i++) {
		nodeList[i]->RotateCW(center);
	}
}

void EPolygon::RotateCCW(CPoint center) {
	for (int i = 0; i < nodeList.GetSize(); i++) {
		nodeList[i]->RotateCCW(center);
	}
}