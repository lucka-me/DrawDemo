#pragma once
#include "Element.h"
#include "EPoint.h"

/* EEllipse
基类
	Element
说明
	基类 Element 中的变量 color 作为边线的颜色
*/

class EEllipse :
	public Element {
	DECLARE_SERIAL(EEllipse)

// 方法
public:
	EEllipse();
	~EEllipse();
	void Serialize(CArchive & ar);
	ElementType GetElementType();
	void Set(CPoint pointA, CPoint pointB);
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
	

// 变量
private:
	EPoint pointA;
	EPoint pointB;
	unsigned short lineWidth;
	COLORREF fillColor;
};

