#pragma once
#include "Element.h"
#include "EPoint.h"

/* ELine
����
	Element
˵��
	���� Element �еı��� color ��Ϊ�ߵ���ɫ
*/

class ELine :
	public Element {
	DECLARE_SERIAL(ELine)

// ����
public:
	ELine();
	~ELine();
	void Serialize(CArchive & ar);
	ElementType GetElementType();
	void Set(CPoint startPoint, CPoint endPoint);
	void SetLineColor(COLORREF newColor);
	void SetLineWidth(unsigned short newWidth);
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
	EPoint start;
	EPoint end;
	unsigned short width;
};

