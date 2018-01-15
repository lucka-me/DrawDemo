// PenWidthDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DrawDemo.h"
#include "PenWidthDialog.h"
#include "afxdialogex.h"


// CPenWidthDialog 对话框

IMPLEMENT_DYNAMIC(CPenWidthDialog, CDialogEx)

CPenWidthDialog::CPenWidthDialog(unsigned short penWidthInit, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_PEN_WIDTH_DIALOG, pParent) {
	penWidth = penWidthInit;
}

CPenWidthDialog::~CPenWidthDialog()
{
	
}

void CPenWidthDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PEN_WIDTH_SLIDER, slider);
	DDX_Control(pDX, IDC_PEN_WIDTH_STATIC, staticText);
	DDX_Control(pDX, IDC_PEN_WIDTH_PREVIEW, preview);
}


BEGIN_MESSAGE_MAP(CPenWidthDialog, CDialogEx)
	ON_WM_HSCROLL()
END_MESSAGE_MAP()


// CPenWidthDialog 消息处理程序


BOOL CPenWidthDialog::OnInitDialog() {
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	slider.SetRange(1, 127);
	slider.SetPos(penWidth);
	CString widthStr;
	widthStr.Format(_T("%d"), penWidth);
	staticText.SetWindowTextW(widthStr);
	
	// 初始化 preview
	preview.GetWindowRect(&previewRect);
	pPreviewDC = preview.GetDC();
	pPreviewDC->SelectStockObject(BLACK_BRUSH);
	pPreviewDC->SelectStockObject(BLACK_PEN);
	pPreviewDC->Ellipse((previewRect.right  - previewRect.left - penWidth) / 2,
						(previewRect.bottom - previewRect.top  - penWidth) / 2,
						(previewRect.right  - previewRect.left + penWidth) / 2,
						(previewRect.bottom - previewRect.top  + penWidth) / 2);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

void CPenWidthDialog::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) {
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	
	pPreviewDC->SelectStockObject(WHITE_BRUSH);
	pPreviewDC->SelectStockObject(WHITE_PEN);
	// 删除旧圆
	pPreviewDC->Ellipse((previewRect.right  - previewRect.left - penWidth) / 2,
						(previewRect.bottom - previewRect.top  - penWidth) / 2,
						(previewRect.right  - previewRect.left + penWidth) / 2,
						(previewRect.bottom - previewRect.top  + penWidth) / 2);
	// 绘制新圆
	pPreviewDC->SelectStockObject(BLACK_BRUSH);
	pPreviewDC->SelectStockObject(BLACK_PEN);
	penWidth = slider.GetPos();
	pPreviewDC->Ellipse((previewRect.right  - previewRect.left - penWidth) / 2,
						(previewRect.bottom - previewRect.top  - penWidth) / 2,
						(previewRect.right  - previewRect.left + penWidth) / 2,
						(previewRect.bottom - previewRect.top  + penWidth) / 2);

	CString widthStr;
	widthStr.Format(_T("%d"), penWidth);
	staticText.SetWindowTextW(widthStr);

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}

// 方法
unsigned short CPenWidthDialog::GetPenWidth() {
	return penWidth;
}