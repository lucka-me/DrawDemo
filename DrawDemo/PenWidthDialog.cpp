// PenWidthDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DrawDemo.h"
#include "PenWidthDialog.h"
#include "afxdialogex.h"


// CPenWidthDialog �Ի���

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


// CPenWidthDialog ��Ϣ�������


BOOL CPenWidthDialog::OnInitDialog() {
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	slider.SetRange(1, 127);
	slider.SetPos(penWidth);
	CString widthStr;
	widthStr.Format(_T("%d"), penWidth);
	staticText.SetWindowTextW(widthStr);
	
	// ��ʼ�� preview
	preview.GetWindowRect(&previewRect);
	pPreviewDC = preview.GetDC();
	pPreviewDC->SelectStockObject(BLACK_BRUSH);
	pPreviewDC->SelectStockObject(BLACK_PEN);
	pPreviewDC->Ellipse((previewRect.right  - previewRect.left - penWidth) / 2,
						(previewRect.bottom - previewRect.top  - penWidth) / 2,
						(previewRect.right  - previewRect.left + penWidth) / 2,
						(previewRect.bottom - previewRect.top  + penWidth) / 2);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}

void CPenWidthDialog::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) {
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	
	pPreviewDC->SelectStockObject(WHITE_BRUSH);
	pPreviewDC->SelectStockObject(WHITE_PEN);
	// ɾ����Բ
	pPreviewDC->Ellipse((previewRect.right  - previewRect.left - penWidth) / 2,
						(previewRect.bottom - previewRect.top  - penWidth) / 2,
						(previewRect.right  - previewRect.left + penWidth) / 2,
						(previewRect.bottom - previewRect.top  + penWidth) / 2);
	// ������Բ
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

// ����
unsigned short CPenWidthDialog::GetPenWidth() {
	return penWidth;
}