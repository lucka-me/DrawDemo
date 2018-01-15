#include "stdafx.h"
#include "ELine.h"
#include <math.h>	// ʹ�� fabs()��sqrt()

IMPLEMENT_SERIAL(ELine, CObject, 1)
ELine::ELine() {
	width = 1;
}


ELine::~ELine() {
}

void ELine::Serialize(CArchive & ar) {
	Element::Serialize(ar);
	start.Serialize(ar);
	end.Serialize(ar);
	if (ar.IsStoring()) {
		ar << width;
	}
	else {
		ar >> width;

	}
}

ElementType ELine::GetElementType() {
	return ET_LINE;
}

void ELine::Set(CPoint startPoint, CPoint endPoint) {
	start.Set(startPoint);
	end.Set(endPoint);
}

void ELine::SetLineColor(COLORREF newColor) {
	color = newColor;
}

void ELine::SetLineWidth(unsigned short newWidth) {
	width = newWidth;
}

void ELine::Draw(CDC &dc) {
	Draw(dc, color);
}

void ELine::Draw(CDC &dc, COLORREF coverColor) {
	CGdiObject *pOldBrush = dc.SelectStockObject(NULL_BRUSH);
	CPen pen;
	pen.CreatePen(PS_SOLID, width, coverColor);
	CPen *pOldPen = dc.SelectObject(&pen);

	dc.MoveTo(start.x, start.y);
	dc.LineTo(end.x, end.y);

	dc.SelectObject(pOldPen);
	dc.SelectObject(pOldBrush);
}

Element * ELine::Copy() {
	ELine * newLine = new ELine;
	newLine->color = this->color;
	newLine->start.x = this->start.x;
	newLine->start.y = this->start.y;
	newLine->end.x = this->end.x;
	newLine->end.y = this->end.y;
	newLine->width = this->width;
	return newLine;
}

bool ELine::Select(CPoint point, unsigned short buffer) {
	bool result = false;
	buffer += width / 2;
	// ������ڶԽ��߾����ⲿ�򲻱�ѡȡ
	// �ԽǾ��α߽�
	int right  = start.x > end.x ? start.x : end.x;
	int left   = start.x > end.x ? end.x   : start.x;
	int top    = start.y > end.y ? start.y : end.y;
	int bottom = start.y > end.y ? end.y   : start.y;
	// �ж��Ƿ��ڶԽǾ�����
	if ((point.x > left   - buffer) &&
		(point.y < top    + buffer) &&
		(point.x < right  + buffer) &&
		(point.y > bottom - buffer)) {
		// ����ʽ:   (y - y1) / (x - x1) = (y2 - y1) / (x2 - x1)
		// ֱ�߷���: A         * x + B         * y + C                   = 0
		// ת��:     (y1 - y2) * x + (x2 - x1) * y + (x1 * y2 - x2 * y1) = 0
		int A = start.y - end.y;
		int B = end.x - start.x;
		int C = start.x * end.y - end.x * start.y;
		if (fabs((A * point.x + B * point.y + C) / sqrt(A * A + B * B)) <= buffer) {
			result = true;
		}
	}
	return result;
}

bool ELine::Select(CRect rangeRect) {
	bool result = false;

	// ����Խ��߾�������β��ཻ�򲻱�ѡȡ
	int right  = start.x > end.x ? start.x : end.x;
	int left   = start.x > end.x ? end.x   : start.x;
	int top    = start.y > end.y ? end.y   : start.y;
	int bottom = start.y > end.y ? start.y : end.y;
	CRect diagRect(left, top, right, bottom);
	CRect testRect;
	// ��������
	//	Refrence: https://msdn.microsoft.com/zh-cn/library/h58f4c7y.aspx#crect__intersectrect
	if (testRect.IntersectRect(diagRect, rangeRect)) {
		// ����ж˵��ھ�������ѡȡ
		if (rangeRect.PtInRect(start.GetCPoint()) || rangeRect.PtInRect(end.GetCPoint())) {
			result = true;
		}
		else {
			// �����Ͻǵ���������������һ�����߶�������ѡȡ
			int A = start.y - end.y;
			int B = end.x - start.x;
			int C = start.x * end.y - end.x * start.y;
			if ((A * rangeRect.left + B * rangeRect.top + C) * (A * rangeRect.right + B * rangeRect.top    + C) <= 0 ||
				(A * rangeRect.left + B * rangeRect.top + C) * (A * rangeRect.right + B * rangeRect.bottom + C) <= 0 ||
				(A * rangeRect.left + B * rangeRect.top + C) * (A * rangeRect.left  + B * rangeRect.bottom + C) <= 0) {
				result = true;
			}
		}
	}
	
	return result;
}

void ELine::Move(int deltaX, int deltaY) {
	start.Move(deltaX, deltaY);
	end.Move(deltaX, deltaY);
}

void ELine::Zoom(CPoint center, double level) {
	start.Zoom(center, level);
	end.Zoom(center, level);
}

void ELine::SymmetryH(int axisX) {
	start.SymmetryH(axisX);
	end.SymmetryH(axisX);
}

void ELine::SymmetryV(int axisY) {
	start.SymmetryV(axisY);
	end.SymmetryV(axisY);
}

void ELine::RotateCW(CPoint center) {
	start.RotateCW(center);
	end.RotateCW(center);
}

void ELine::RotateCCW(CPoint center) {
	start.RotateCCW(center);
	end.RotateCCW(center);
}