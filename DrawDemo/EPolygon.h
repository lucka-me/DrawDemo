#pragma once
#include "Element.h"
#include "ElementArray.h"
#include "EPoint.h"
#include "ELine.h"

class EPolygon :
	public Element {
	DECLARE_SERIAL(EPolygon)
public:
	EPolygon();
	~EPolygon();
	void Serialize(CArchive & ar);
	ElementType GetElementType();
	void Set(CPoint startNode, CPoint secondNode);
	void Add(CPoint newNode);
	void SetLineColor(COLORREF newColor);
	void SetLineWidth(unsigned short newWidth);
	void SetFillColor(COLORREF newFillColor);
	CPoint * GetCPointList();	// 取得 CPoint 数组
	void Draw(CDC & dc);
	void Draw(CDC & dc, COLORREF coverColor);
	void DrawLast(CDC & dc);	// 绘制最新加入的最后一条线
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
public:
	ElementArray nodeList;
	unsigned short lineWidth;
	COLORREF fillColor;
};

