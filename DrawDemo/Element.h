#pragma once
#include "afxwin.h"

/* Element
基类
	CObject		可序列化存储
子类
	EPoint		点
	ELine		线
	ERectangle	矩形
	EEllipse	椭圆
*/

typedef enum _ElementType {
	ET_NONE,		// 无类型（不应该出现）
	ET_POINT,		// 点
	ET_LINE,		// 线
	ET_POLYLINE,	// 折线
	ET_RECTANGLE,	// 矩形
	ET_ELLIPSE,		// 椭圆
} ElementType;

class Element: public CObject {
	DECLARE_SERIAL(Element)

	// 方法
public:
	Element();
	virtual ~Element();
	virtual void Serialize(CArchive & ar);							// 序列化
	virtual ElementType GetElementType();							// 返回子类类型
	virtual void Draw(CDC & dc);									// 绘制（以内部颜色）
	virtual void Draw(CDC & dc, COLORREF coverColor);				// 用指定颜色绘制但不改变内部颜色
	virtual Element * Copy();										// 复制
	virtual bool Select(CPoint point, unsigned short buffer = 2);	// 点选
	virtual bool Select(CRect rangeRect);							// 选取范围
	virtual void Move(int deltaX, int deltaY);						// 移动
	virtual void Zoom(CPoint center, double level);					// 缩放
	virtual void SymmetryH(int axisX);								// 水平对称
	virtual void SymmetryV(int axisY);								// 垂直对称
	virtual void RotateCW(CPoint center);							// 顺时针旋转
	virtual void RotateCCW(CPoint center);							// 逆时针旋转

	// 成员变量
public:
	COLORREF color;
};

