#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CPenWidthDialog 对话框

class CPenWidthDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CPenWidthDialog)

public:
	CPenWidthDialog(unsigned short penWidthInit, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPenWidthDialog();

// 变量
private:
	unsigned short penWidth;
	CSliderCtrl slider;
	CStatic staticText;
	CStatic preview;
	CRect previewRect;
	CDC *pPreviewDC;

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PEN_WIDTH_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	unsigned short GetPenWidth();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};
