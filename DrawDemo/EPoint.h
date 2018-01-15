#pragma once
#include "Element.h"

/* EPoint
基类
	Element
说明
	基类 Element 中的变量 color 作为点的颜色
*/

class EPoint :
	public Element {
	DECLARE_SERIAL(EPoint)

// 方法
public:
	EPoint();
	~EPoint();
	void Serialize(CArchive & ar);
	ElementType GetElementType();
	void Set(CPoint point);
	void SetColor(COLORREF newColor);
	CPoint GetCPoint();		// 返回对应的 CPoint
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

// 变量
public:
	int x;
	int y;
};

