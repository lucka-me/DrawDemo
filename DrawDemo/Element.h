#pragma once
#include "afxwin.h"

/* Element
����
	CObject		�����л��洢
����
	EPoint		��
	ELine		��
	ERectangle	����
	EEllipse	��Բ
*/

typedef enum _ElementType {
	ET_NONE,		// �����ͣ���Ӧ�ó��֣�
	ET_POINT,		// ��
	ET_LINE,		// ��
	ET_POLYLINE,	// ����
	ET_RECTANGLE,	// ����
	ET_ELLIPSE,		// ��Բ
} ElementType;

class Element: public CObject {
	DECLARE_SERIAL(Element)

	// ����
public:
	Element();
	virtual ~Element();
	virtual void Serialize(CArchive & ar);							// ���л�
	virtual ElementType GetElementType();							// ������������
	virtual void Draw(CDC & dc);									// ���ƣ����ڲ���ɫ��
	virtual void Draw(CDC & dc, COLORREF coverColor);				// ��ָ����ɫ���Ƶ����ı��ڲ���ɫ
	virtual Element * Copy();										// ����
	virtual bool Select(CPoint point, unsigned short buffer = 2);	// ��ѡ
	virtual bool Select(CRect rangeRect);							// ѡȡ��Χ
	virtual void Move(int deltaX, int deltaY);						// �ƶ�
	virtual void Zoom(CPoint center, double level);					// ����
	virtual void SymmetryH(int axisX);								// ˮƽ�Գ�
	virtual void SymmetryV(int axisY);								// ��ֱ�Գ�
	virtual void RotateCW(CPoint center);							// ˳ʱ����ת
	virtual void RotateCCW(CPoint center);							// ��ʱ����ת

	// ��Ա����
public:
	COLORREF color;
};

