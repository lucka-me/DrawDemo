#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CPenWidthDialog �Ի���

class CPenWidthDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CPenWidthDialog)

public:
	CPenWidthDialog(unsigned short penWidthInit, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPenWidthDialog();

// ����
private:
	unsigned short penWidth;
	CSliderCtrl slider;
	CStatic staticText;
	CStatic preview;
	CRect previewRect;
	CDC *pPreviewDC;

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PEN_WIDTH_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	unsigned short GetPenWidth();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};
