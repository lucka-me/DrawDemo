#include "stdafx.h"
#include "EPolyline.h"

IMPLEMENT_SERIAL(EPolyline, CObject, 1)
EPolyline::EPolyline() {
}

EPolyline::~EPolyline() {
}

void EPolyline::Serialize(CArchive & ar) {
	Element::Serialize(ar);

	nodeList.Serialize(ar);
	if (ar.IsStoring()) {
		ar << width;
	}
	else {
		ar >> width;

	}
}

ElementType EPolyline::GetElementType() {
	return ET_POLYLINE;
}

void EPolyline::Set(CPoint startNode, CPoint secondNode) {
	nodeList.RemoveAll();
	Add(startNode);
	Add(secondNode);
}

void EPolyline::Add(CPoint newNode) {
	EPoint *pNewNode = new EPoint;
	pNewNode->Set(newNode);
	pNewNode->SetColor(color);
	nodeList.Add(pNewNode);
}

void EPolyline::SetLineColor(COLORREF newColor) {
	color = newColor;
}

void EPolyline::SetLineWidth(unsigned short newWidth) {
	width = newWidth;
}

void EPolyline::Draw(CDC &dc) {
	Draw(dc, color);
}

void EPolyline::Draw(CDC &dc, COLORREF coverColor) {
	CGdiObject *pOldBrush = dc.SelectStockObject(NULL_BRUSH);
	CPen pen;
	pen.CreatePen(PS_SOLID, width, coverColor);
	CPen *pOldPen = dc.SelectObject(&pen);
	
	// 画出折线
	EPoint * pNode = (EPoint *)nodeList[0];
	dc.MoveTo(pNode->x, pNode->y);
	for (int i = 1; i < nodeList.GetSize(); i++) {
		EPoint *pNode = (EPoint *)nodeList[i];
		dc.LineTo(pNode->x, pNode->y);
	}
	// 画出节点
	for (int i = 0; i < nodeList.GetSize(); i++) {
		EPoint *pNode = (EPoint *)nodeList[i];
		pNode->Draw(dc, coverColor);
	}

	dc.SelectObject(pOldPen);
	dc.SelectObject(pOldBrush);
}

void EPolyline::DrawLast(CDC & dc) {
	CGdiObject *pOldBrush = dc.SelectStockObject(NULL_BRUSH);
	CPen pen;
	pen.CreatePen(PS_SOLID, width, color);
	CPen *pOldPen = dc.SelectObject(&pen);

	EPoint * pStart = (EPoint *)nodeList[nodeList.GetSize() - 2];
	EPoint * pEnd = (EPoint *)nodeList[nodeList.GetSize() - 1];
	dc.MoveTo(pStart->x, pStart->y);
	dc.LineTo(pEnd->x, pEnd->y);
	pEnd->Draw(dc, color);

	dc.SelectObject(pOldPen);
	dc.SelectObject(pOldBrush);
}

Element * EPolyline::Copy() {
	EPolyline * newPolyline = new EPolyline;
	newPolyline->color = this->color;
	newPolyline->width = this->width;
	for (int i = 0; i < nodeList.GetSize(); i++) {
		EPoint *pNode = (EPoint *)nodeList[i];
		newPolyline->Add(pNode->GetCPoint());
	}
	return newPolyline;
}

bool EPolyline::Select(CPoint point, unsigned short buffer) {
	bool result = false;
	buffer += width / 2;
	EPoint *pStart = (EPoint *)nodeList[0];
	for (int i = 1; i < nodeList.GetSize(); i++) {
		EPoint *pEnd = (EPoint *)nodeList[i];
		ELine line;
		line.Set(pStart->GetCPoint(), pEnd->GetCPoint());
		line.SetLineWidth(width);
		if (line.Select(point, buffer)) {
			result = true;
			break;
		} else {
			pStart = pEnd;
		}
	}
	return result;
}

bool EPolyline::Select(CRect rangeRect) {
	bool result = false;
	EPoint *pStart = (EPoint *)nodeList[0];
	for (int i = 1; i < nodeList.GetSize(); i++) {
		EPoint *pEnd = (EPoint *)nodeList[i];
		ELine line;
		line.Set(pStart->GetCPoint(), pEnd->GetCPoint());
		line.SetLineWidth(width);
		if (line.Select(rangeRect)) {
			result = true;
			break;
		} else {
			pStart = pEnd;
		}
	}
	return result;
}

void EPolyline::Move(int deltaX, int deltaY) {
	for (int i = 0; i < nodeList.GetSize(); i++) {
		nodeList[i]->Move(deltaX, deltaY);
	}
}

void EPolyline::Zoom(CPoint center, double level) {
	for (int i = 0; i < nodeList.GetSize(); i++) {
		nodeList[i]->Zoom(center, level);
	}
}

void EPolyline::SymmetryH(int axisX) {
	for (int i = 0; i < nodeList.GetSize(); i++) {
		nodeList[i]->SymmetryH(axisX);
	}
}

void EPolyline::SymmetryV(int axisY) {
	for (int i = 0; i < nodeList.GetSize(); i++) {
		nodeList[i]->SymmetryV(axisY);
	}
}

void EPolyline::RotateCW(CPoint center) {
	for (int i = 0; i < nodeList.GetSize(); i++) {
		nodeList[i]->RotateCW(center);
	}
}

void EPolyline::RotateCCW(CPoint center) {
	for (int i = 0; i < nodeList.GetSize(); i++) {
		nodeList[i]->RotateCCW(center);
	}
}