#pragma once
#include "Element.h"
#include "EPoint.h"

/* ERectangle
����
	Element
˵��
	���� Element �еı��� color ��Ϊ���ߵ���ɫ
*/

class ERectangle :
	public Element {
	DECLARE_SERIAL(ERectangle)

// ����
public:
	ERectangle();
	~ERectangle();
	void Serialize(CArchive & ar);
	ElementType GetElementType();
	void Set(CPoint A, CPoint B);
	void SetLineColor(COLORREF newLineColor);
	void SetFillColor(COLORREF newFillColor);
	void SetLineWidth(unsigned short newLineWidth);
	void Draw(CDC & dc);
	void Draw(CDC & dc, COLORREF coverColor);
	Element * Copy();
	bool Select(CPoint point, unsigned short buffer = 2);
	bool Select(CRect rangeRect);
	void Move(int deltaX, int deltaY);
	void Zoom(CPoint center, double level);
	void SymmetryH(int axisX);
	void SymmetryV(int axisY);
	void RotateCW(CPoint center);
	void RotateCCW(CPoint center);

// ����
private:
	EPoint pointA;
	EPoint pointB;
	unsigned short lineWidth;
	COLORREF fillColor;
};