#pragma once
#include "Element.h"

/* EPoint
����
	Element
˵��
	���� Element �еı��� color ��Ϊ�����ɫ
*/

class EPoint :
	public Element {
	DECLARE_SERIAL(EPoint)

// ����
public:
	EPoint();
	~EPoint();
	void Serialize(CArchive & ar);
	ElementType GetElementType();
	void Set(CPoint point);
	void SetColor(COLORREF newColor);
	CPoint GetCPoint();		// ���ض�Ӧ�� CPoint
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
public:
	int x;
	int y;
};

