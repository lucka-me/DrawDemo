
// DrawDemoView.cpp: CDrawDemoView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "DrawDemo.h"
#endif

#include "DrawDemoDoc.h"
#include "DrawDemoView.h"
#include "MainFrm.h"	// 访问 MainFrame
#include <string>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDrawDemoView

IMPLEMENT_DYNCREATE(CDrawDemoView, CView)

BEGIN_MESSAGE_MAP(CDrawDemoView, CView)
	// 文件
	ON_COMMAND(ID_FILE_SAVE, &CDrawDemoView::OnFileSave)

	// 绘图
	ON_COMMAND(ID_DRAW_POINT, &CDrawDemoView::OnDrawPoint)
	ON_COMMAND(ID_DRAW_LINE, &CDrawDemoView::OnDrawLine)
	ON_COMMAND(ID_DRAW_POLYLINE, &CDrawDemoView::OnDrawPolyline)
	ON_COMMAND(ID_DRAW_RECTANGLE, &CDrawDemoView::OnDrawRectangle)
	ON_COMMAND(ID_DRAW_POLYGON, &CDrawDemoView::OnDrawPolygon)
	ON_COMMAND(ID_DRAW_ELLIPSE, &CDrawDemoView::OnDrawEllipse)

	// 编辑
	ON_COMMAND(ID_EDIT_COPY, &CDrawDemoView::OnEditCopy)
	ON_COMMAND(ID_EDIT_PASTE, &CDrawDemoView::OnEditPaste)

	ON_COMMAND(ID_EDIT_SELECT, &CDrawDemoView::OnEditSelect)
	ON_COMMAND(ID_EDIT_SELECT_RANGE, &CDrawDemoView::OnEditSelectRange)
	ON_COMMAND(ID_EDIT_SELECT_ALL, &CDrawDemoView::OnEditSelectAll)
	ON_COMMAND(ID_EDIT_MOVE, &CDrawDemoView::OnEditMove)
	ON_COMMAND(ID_EDIT_ZOOM, &CDrawDemoView::OnEditZoom)
	ON_COMMAND(ID_EDIT_DELETE, &CDrawDemoView::OnEditDelete)

	ON_COMMAND(ID_EDIT_SYMMETRY_H, &CDrawDemoView::OnEditSymmetryH)
	ON_COMMAND(ID_EDIT_SYMMETRY_V, &CDrawDemoView::OnEditSymmetryV)
	ON_COMMAND(ID_EDIT_ROTATE_CW, &CDrawDemoView::OnEditRotateCW)
	ON_COMMAND(ID_EDIT_ROTATE_CCW, &CDrawDemoView::OnEditRotateCCW)
	
	ON_COMMAND(ID_EDIT_SET_COLOR_AUX, &CDrawDemoView::OnEditSetColorAux)
	ON_COMMAND(ID_EDIT_SET_COLOR_PEN, &CDrawDemoView::OnEditSetColorPen)
	ON_COMMAND(ID_EDIT_SET_COLOR_FILL, &CDrawDemoView::OnEditSetColorFill)
	ON_COMMAND(ID_EDIT_SET_WIDTH_PEN, &CDrawDemoView::OnEditSetWidthPen)
	ON_COMMAND(ID_EDIT_SET_WIDTH_AUX, &CDrawDemoView::OnEditSetWidthAux)

	ON_COMMAND(ID_EDIT_CHANGE_WIDTH, &CDrawDemoView::OnEditChangeWidth)
	ON_COMMAND(ID_EDIT_CHANGE_COLOR, &CDrawDemoView::OnEditChangeColor)
	ON_COMMAND(ID_EDIT_CHANGE_FILL, &CDrawDemoView::OnEditChangeFill)

	ON_COMMAND(ID_CANCEL_OPR, &CDrawDemoView::OnCancelOpr)

	// 鼠标消息
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()

	// 键盘消息
	ON_WM_KEYUP()
	ON_WM_CHAR()

	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CDrawDemoView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()	
	
END_MESSAGE_MAP()

// CDrawDemoView 构造/析构

CDrawDemoView::CDrawDemoView()
{
	// TODO: 在此处添加构造代码
	oprType = OPR_NONE;
	clickCount = 0;
	// 画笔颜色和宽度
	auxPenColor = RGB(0, 0, 255);
	auxPenWidth = 1;
	
	selectedColor = RGB(255, 0, 0);
	penColor = RGB(0, 0, 0);
	fillColor = RGB(255, 255, 255);
	lineWidth = 1;
}

CDrawDemoView::~CDrawDemoView()
{
}

BOOL CDrawDemoView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CDrawDemoView 绘图

void CDrawDemoView::OnDraw(CDC* pDC)
{
	CDrawDemoDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	
	// 获取画图区域大小
	GetClientRect(&clientRect);
	
	// 双缓存
	//	Refrence: http://blog.csdn.net/tiaotiaoyly/article/details/2516235
	CDC memDC;
	CBitmap memBitmap;
	memDC.CreateCompatibleDC(NULL);
	memBitmap.CreateCompatibleBitmap(pDC, clientRect.right, clientRect.bottom);
	CBitmap *pOldBit = memDC.SelectObject(&memBitmap);
	memDC.FillSolidRect(0, 0, clientRect.right, clientRect.bottom, RGB(255, 255, 255));

	// 取出数据并重绘图形
	for (int i = 0; i < pDoc->dataList.GetSize(); i++) {
		pDoc->dataList[i]->Draw(memDC);
	}
	for (int i = 0; i < editList.GetSize(); i++) {
		editList[i]->Draw(memDC, selectedColor);
	}

	pDC->BitBlt(0, 0, clientRect.right, clientRect.bottom, &memDC, 0, 0, SRCCOPY);
	memBitmap.DeleteObject();
	memDC.DeleteDC();
}

// CDrawDemoView 打印

void CDrawDemoView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CDrawDemoView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CDrawDemoView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CDrawDemoView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}

void CDrawDemoView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, true);
#endif
}


// CDrawDemoView 诊断

#ifdef _DEBUG
void CDrawDemoView::AssertValid() const
{
	CView::AssertValid();
}

void CDrawDemoView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CDrawDemoDoc* CDrawDemoView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDrawDemoDoc)));
	return (CDrawDemoDoc*)m_pDocument;
}
#endif //_DEBUG


/* MARK: - CDrawDemoView 消息处理程序 */
// MARK: - 菜单消息处理程序

// MARK: - 文件
void CDrawDemoView::OnFileSave() {
	// 文件-保存
	CancelOpr();
	GetDocument()->DoFileSave();
}

// END: - 文件

// MARK: - 编辑
void CDrawDemoView::OnEditCopy() {
	// 菜单-编辑-复制
	if (CheckEditListNotEmpty()) {
		clipboard.RemoveAll();
		for (int i = 0; i < editList.GetSize(); i++) {
			clipboard.Add(editList[i]->Copy());
		}
	}
}

void CDrawDemoView::OnEditPaste() {
	// 菜单-编辑-粘贴
	for (int i = clipboard.GetSize() - 1; i >= 0 ; i--) {
		clipboard[i]->Move(10, 10);
		GetDocument()->dataList.Add(clipboard[i]);
		clipboard.Add(clipboard[i]->Copy());
		clipboard.RemoveAt(i);
	}
	Invalidate();
}

void CDrawDemoView::OnEditSelect() {
	// 菜单-编辑-点选
	CancelOpr();
	HCURSOR cursor = LoadCursor(NULL, IDC_CROSS);
	SetCursor(cursor);
	oprType = OPR_EDIT_SELECT;
}

void CDrawDemoView::OnEditSelectRange() {
	// 菜单-编辑-选取范围
	CancelOpr();
	HCURSOR cursor = LoadCursor(NULL, IDC_CROSS);
	SetCursor(cursor);
	clickCount = 0;
	oprType = OPR_EDIT_SELECT_RANGE;

}

void CDrawDemoView::OnEditSelectAll() {
	// 菜单-编辑-全选
	CancelOpr();
	CClientDC dc(this);
	int addCount = 0;
	for (int i = 0; i < GetDocument()->dataList.GetSize(); i++) {
		if (GetDocument()->dataList[i]->Select(clientRect)) {
			if (!editList.HasElement(GetDocument()->dataList[i])) {
				GetDocument()->dataList[i]->Draw(dc, selectedColor);
				editList.Add(GetDocument()->dataList[i]);
				addCount++;
			}
		}
	}
	CString message;
	message.Format(_T("新选取 %d 个图形，共选取 %d 个"), addCount, editList.GetSize());
}

void CDrawDemoView::OnEditMove() {
	// 菜单-编辑-移动
	if (CheckEditListNotEmpty()) {
		HCURSOR cursor = LoadCursor(NULL, IDC_CROSS);
		SetCursor(cursor);
		clickCount = 0;
		oprType = OPR_EDIT_MOVE;
	}
}

void CDrawDemoView::OnEditZoom() {
	// 菜单-编辑-缩放
	if (CheckEditListNotEmpty()) {
		HCURSOR cursor = LoadCursor(NULL, IDC_CROSS);
		SetCursor(cursor);
		clickCount = 0;
		oprType = OPR_EDIT_ZOOM;
	}
}

// MARK: - 旋转
void CDrawDemoView::OnEditRotateCW() {
	// 菜单-编辑-旋转-顺时针旋转
	if (CheckEditListNotEmpty()) {
		HCURSOR cursor = LoadCursor(NULL, IDC_CROSS);
		SetCursor(cursor);
		clickCount = 0;
		oprType = OPR_EDIT_ROTATE_CW;
	}
}

void CDrawDemoView::OnEditRotateCCW() {
	// 菜单-编辑-旋转-逆时针旋转
	if (CheckEditListNotEmpty()) {
		HCURSOR cursor = LoadCursor(NULL, IDC_CROSS);
		SetCursor(cursor);
		clickCount = 0;
		oprType = OPR_EDIT_ROTATE_CCW;
	}
}

void CDrawDemoView::OnEditSymmetryH() {
	// 菜单-编辑-旋转-水平对称
	if (CheckEditListNotEmpty()) {
		// 绘制新橡皮筋
		CClientDC dc(this);
		CGdiObject *pOldBrush = dc.SelectStockObject(NULL_BRUSH);
		CPen pen;
		pen.CreatePen(PS_SOLID, auxPenWidth, auxPenColor);
		CPen *pOldPen = dc.SelectObject(&pen);
		int drawMode = dc.SetROP2(R2_NOTXORPEN);

		dc.MoveTo(pointLastMouse.x, 0);
		dc.LineTo(pointLastMouse.x, clientRect.bottom);

		dc.SetROP2(drawMode);
		dc.SelectObject(pOldPen);
		dc.SelectObject(pOldBrush);

		clickCount = 0;
		oprType = OPR_EDIT_SYMMETRY_H;
	}
}


void CDrawDemoView::OnEditSymmetryV() {
	// 菜单-编辑-旋转-垂直对称
	if (CheckEditListNotEmpty()) {
		// 绘制新橡皮筋
		CClientDC dc(this);
		CGdiObject *pOldBrush = dc.SelectStockObject(NULL_BRUSH);
		CPen pen;
		pen.CreatePen(PS_SOLID, auxPenWidth, auxPenColor);
		CPen *pOldPen = dc.SelectObject(&pen);
		int drawMode = dc.SetROP2(R2_NOTXORPEN);

		dc.MoveTo(0, pointLastMouse.y);
		dc.LineTo(clientRect.right, pointLastMouse.y);
		
		dc.SetROP2(drawMode);
		dc.SelectObject(pOldPen);
		dc.SelectObject(pOldBrush);

		clickCount = 0;
		oprType = OPR_EDIT_SYMMETRY_V;
	}
}

// END: - 旋转

void CDrawDemoView::OnEditDelete() {
	// 菜单-编辑-删除
	if (CheckEditListNotEmpty()) {
		if (MessageBox(_T("确定要删除吗？"), _T("提示"), MB_OKCANCEL | MB_ICONWARNING) == IDOK) {
			CClientDC dc(this);
			int count = 0;
			for (int i = editList.GetSize() - 1; i >= 0; i--) {
				Element *pElement = editList[i];
				editList.Remove(pElement);
				GetDocument()->dataList.Remove(pElement);
				Invalidate();
				count++;
				delete(pElement);
			}
			CString message;
			message.Format(_T("已删除 %d 个图形"), count);
			UpdateStatusBar(_T("已删除"));
		}
	}
}

// MARK: - 更改
void CDrawDemoView::OnEditChangeWidth() {
	// 菜单-编辑-更改-线宽
	if (CheckEditListNotEmpty()) {
		int count = 0;
		CPenWidthDialog penWidthDlg(lineWidth, this);
		if (penWidthDlg.DoModal() == IDOK) {
			unsigned short newLineWidth = penWidthDlg.GetPenWidth();

			for (int i = 0; i < editList.GetSize(); i++) {
				Element *pElement = editList[i];
				switch (pElement->GetElementType()) {
				case ET_LINE: {
					ELine * pLine = (ELine *)pElement;
					pLine->SetLineWidth(newLineWidth);
					count++;
					break;
				}
				case ET_POLYLINE: {
					EPolyline * pPolyline = (EPolyline *)pElement;
					pPolyline->SetLineWidth(newLineWidth);
					break;
				}
				case ET_RECTANGLE: {
					ERectangle * pRect = (ERectangle *)pElement;
					pRect->SetLineWidth(newLineWidth);
					count++;
					break;
				}
				case ET_POLYGON: {
					EPolygon * pPolygon = (EPolygon *)pElement;
					pPolygon->SetLineWidth(newLineWidth);
					break;
				}
				case ET_ELLIPSE: {
					EEllipse * pEllipse = (EEllipse *)pElement;
					pEllipse->SetLineWidth(newLineWidth);
					count++;
					break;
				}
				default:
					break;
				}
			}
		}
		CString message;
		if (count == 0) {
			message.Format(_T("没有可以修改线宽的图形！\n可以修改线宽的图形包括：线、矩形、椭圆。"));
			MessageBox(message, _T("提示"), MB_OK | MB_ICONWARNING);
		} else {
			Invalidate();
			message.Format(_T("共修改 %d 个图形"), count);
			UpdateStatusBar(message);
		}
	}
}

void CDrawDemoView::OnEditChangeColor() {
	// 菜单-编辑-更改-颜色
	if (CheckEditListNotEmpty()) {
		CColorDialog colorDlg(penColor, CC_FULLOPEN, this);
		if (colorDlg.DoModal() == IDOK) {
			COLORREF newColor = colorDlg.GetColor();
			int count = 0;
			for (int i = 0; i < editList.GetSize(); i++) {
				Element *pElement = editList[i];
				switch (pElement->GetElementType()) {
				case ET_POINT: {
					EPoint * pPoint = (EPoint *)pElement;
					pPoint->SetColor(newColor);
					break;
				}
				case ET_LINE: {
					ELine * pLine = (ELine *)pElement;
					pLine->SetLineColor(newColor);
					break;
				}
				case ET_POLYLINE: {
					EPolyline * pPolyline = (EPolyline *)pElement;
					pPolyline->SetLineColor(newColor);
					break;
				}
				case ET_RECTANGLE: {
					ERectangle * pRect = (ERectangle *)pElement;
					pRect->SetLineColor(newColor);
					break;
				}
				case ET_POLYGON: {
					EPolygon * pPolygon = (EPolygon *)pElement;
					pPolygon->SetLineColor(newColor);
					break;
				}
				case ET_ELLIPSE: {
					EEllipse * pEllipse = (EEllipse *)pElement;
					pEllipse->SetLineColor(newColor);
					break;
				}
				default:
					break;
				}
			}
			Invalidate();
			CString message;
			message.Format(_T("共修改 %d 个图形"), count);
			UpdateStatusBar(message);
		}
	}
}


void CDrawDemoView::OnEditChangeFill() {
	// 菜单-编辑-更改-填充
	if (CheckEditListNotEmpty()) {
		CColorDialog colorDlg(fillColor, CC_FULLOPEN, this);
		if (colorDlg.DoModal() == IDOK) {
			COLORREF newFill = colorDlg.GetColor();
			int count = 0;
			for (int i = 0; i < editList.GetSize(); i++) {
				Element *pElement = editList[i];
				switch (pElement->GetElementType()) {
				case ET_RECTANGLE: {
					ERectangle * pRect = (ERectangle *)pElement;
					pRect->SetFillColor(newFill);
					count++;
					break;
				}
				case ET_POLYGON: {
					EPolygon * pPolygon = (EPolygon *)pElement;
					pPolygon->SetFillColor(newFill);
					count++;
					break;
				}
				case ET_ELLIPSE: {
					EEllipse * pEllipse = (EEllipse *)pElement;
					pEllipse->SetFillColor(newFill);
					count++;
					break;
				}
				default:
					break;
				}
			}
			CString message;
			if (count == 0) {
				message.Format(_T("没有可以修改填充的图形！\n可以修改填充的图形包括：矩形、椭圆。"));
				MessageBox(message, _T("提示"), MB_OK | MB_ICONWARNING);
			}
			else {
				Invalidate();
				message.Format(_T("共修改 %d 个图形"), count);
				UpdateStatusBar(message);
			}
		}
	}
}

// END: - 更改

// MARK: - 颜色
void CDrawDemoView::OnEditSetColorPen() {
	// 菜单-编辑-颜色-画笔颜色
	CancelOpr();
	CColorDialog colorDlg(penColor, CC_FULLOPEN, this);
	if (colorDlg.DoModal() == IDOK) {
		penColor = colorDlg.GetColor();
	}
}

void CDrawDemoView::OnEditSetColorFill() {
	// 菜单-编辑-颜色-填充颜色
	CancelOpr();
	CColorDialog colorDlg(fillColor, CC_FULLOPEN, this);
	if (colorDlg.DoModal() == IDOK) {
		fillColor = colorDlg.GetColor();
	}
}

void CDrawDemoView::OnEditSetColorAux() {
	// 菜单-编辑-颜色-橡皮筋颜色
	CancelOpr();
	CColorDialog colorDlg(auxPenColor, CC_FULLOPEN, this);
	if (colorDlg.DoModal() == IDOK) {
		auxPenColor = colorDlg.GetColor();
	}
}

// END: - 颜色

// MARK: - 笔宽
void CDrawDemoView::OnEditSetWidthPen() {
	// 菜单-编辑-笔宽-画笔宽度
	CancelOpr();
	CPenWidthDialog penWidthDlg(lineWidth, this);
	if (penWidthDlg.DoModal() == IDOK) {
		lineWidth = penWidthDlg.GetPenWidth();
	}
}

void CDrawDemoView::OnEditSetWidthAux() {
	// 菜单-编辑-笔宽-橡皮筋宽度
	CancelOpr();
	CPenWidthDialog penWidthDlg(auxPenWidth, this);
	if (penWidthDlg.DoModal() == IDOK) {
		auxPenWidth = penWidthDlg.GetPenWidth();
	}
}

// END: - 笔宽

// END: - 编辑

// MARK: - 绘图
void CDrawDemoView::OnDrawPoint() {
	// 菜单-绘图-点
	CancelOpr();
	HCURSOR cursor = LoadCursor(NULL, IDC_CROSS);
	SetCursor(cursor);
	clickCount = 0;
	oprType = OPR_DRAW_POINT;
}

void CDrawDemoView::OnDrawLine() {
	// 菜单-绘图-线
	CancelOpr();
	HCURSOR cursor = LoadCursor(NULL, IDC_CROSS);
	SetCursor(cursor);
	clickCount = 0;
	oprType = OPR_DRAW_LINE;
}

void CDrawDemoView::OnDrawPolyline() {
	// 菜单-绘图-折线
	CancelOpr();
	HCURSOR cursor = LoadCursor(NULL, IDC_CROSS);
	SetCursor(cursor);
	clickCount = 0;
	oprType = OPR_DRAW_POLYLINE;
}

void CDrawDemoView::OnDrawRectangle() {
	// 菜单-绘图-矩形
	CancelOpr();
	HCURSOR cursor = LoadCursor(NULL, IDC_CROSS);
	SetCursor(cursor);
	clickCount = 0;
	oprType = OPR_DRAW_RECTANGLE;
}

void CDrawDemoView::OnDrawPolygon() {
	// 菜单-绘图-多边形
	CancelOpr();
	HCURSOR cursor = LoadCursor(NULL, IDC_CROSS);
	SetCursor(cursor);
	clickCount = 0;
	oprType = OPR_DRAW_POLYGON;
}

void CDrawDemoView::OnDrawEllipse() {
	// 菜单-绘图-椭圆
	CancelOpr();
	HCURSOR cursor = LoadCursor(NULL, IDC_CROSS);
	SetCursor(cursor);
	clickCount = 0;
	oprType = OPR_DRAW_ELLIPSE;
}

// END: - 绘图

// END: - 菜单消息处理程序

// MARK: - 鼠标消息处理程序
void CDrawDemoView::OnLButtonDown(UINT nFlags, CPoint point) {
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	// 编辑-点选: 左键按下时选择/取消选择
	CClientDC dc(this);

	switch (oprType) {
	case OPR_EDIT_SELECT:
		for (int i = 0; i < GetDocument()->dataList.GetSize(); i++) {
			if (GetDocument()->dataList[i]->Select(point)) {
				if (editList.HasElement(GetDocument()->dataList[i])) {
					// 已选取则取消选取并从编辑列表删除
					if (editList.Remove(GetDocument()->dataList[i])) {
						Invalidate();
						UpdateStatusBar(_T("已取消选取"));
					} else {
						UpdateStatusBar(_T("取消选取失败！"));
					}
				} else {
					// 未选取则选取并加入编辑列表
					GetDocument()->dataList[i]->Draw(dc, selectedColor);
					editList.Add(GetDocument()->dataList[i]);
					UpdateStatusBar(_T("已选取"));
				}
				// 按住 SHIFT / CTRL 键多选
				
				if ((nFlags != MK_SHIFT + 1) && 
					(nFlags != MK_CONTROL + 1)) {
					CancelOpr();
				}
				
				break;
			}
		}
		break;
	default:
		break;
	}

	CView::OnLButtonDown(nFlags, point);
}

void CDrawDemoView::OnLButtonUp(UINT nFlags, CPoint point) {
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	// 绘图:      左键弹起时开始/完成操作
	// 编辑-点选: 左键弹起时确定起/止点
	CClientDC dc(this);

	switch (oprType) {
	case OPR_DRAW_POINT: {	// 点
							// 避免出现“初始化操作由"case"标签跳过”的错误，放入大括号
		EPoint *pNewPoint = new EPoint;
		pNewPoint->Set(point);
		pNewPoint->SetColor(penColor);
		pNewPoint->Draw(dc);
		this->GetDocument()->dataList.Add(pNewPoint);
		break;
	}
	case OPR_DRAW_LINE:		// 线
		if (clickCount == 0) {
			pointStart = point;
			pointLastMouse = point;
			clickCount++;
		} else if (clickCount == 1) {
			CancelOpr();
			ELine *pNewLine = new ELine;
			pNewLine->Set(pointStart, point);
			pNewLine->SetLineColor(penColor);
			pNewLine->SetLineWidth(lineWidth);
			pNewLine->Draw(dc);
			this->GetDocument()->dataList.Add(pNewLine);
			OnDrawLine();
		}
		break;
	case OPR_DRAW_POLYLINE:		// 折线
		if (clickCount == 0) {
			pointStart = point;
			pointLastMouse = point;
			clickCount++;
		} else if (clickCount == 1) {
			EPolyline *pNewPolyline = new EPolyline;
			pNewPolyline->SetLineColor(penColor);
			pNewPolyline->SetLineWidth(lineWidth);
			pNewPolyline->Set(pointStart, point);
			pNewPolyline->Draw(dc);
			this->GetDocument()->dataList.Add(pNewPolyline);
			pointStart = point;
			clickCount++;
		} else {
			if (GetDocument()->dataList[GetDocument()->dataList.GetSize() - 1]->GetElementType() != ET_POLYLINE) {
				MessageBox(_T("图形错误。"), _T("提示"), MB_OK | MB_ICONWARNING);
				CancelOpr();
			} else {
				EPolyline * polyline = (EPolyline *)GetDocument()->dataList[GetDocument()->dataList.GetSize() - 1];
				polyline->Add(point);
				polyline->DrawLast(dc);
				clickCount++;
				pointStart = point;
			}
		}
		break;
	case OPR_DRAW_RECTANGLE:	// 矩形
		if (clickCount == 0) {
			pointStart = point;
			pointLastMouse = point;
			clickCount++;
		} else if (clickCount == 1) {
			CancelOpr();
			ERectangle *pNewRectangle = new ERectangle;
			pNewRectangle->Set(pointStart, point);
			pNewRectangle->SetLineColor(penColor);
			pNewRectangle->SetFillColor(fillColor);
			pNewRectangle->SetLineWidth(lineWidth);
			pNewRectangle->Draw(dc);
			this->GetDocument()->dataList.Add(pNewRectangle);
			OnDrawRectangle();
		}
		break;
	case OPR_DRAW_POLYGON:		// 多边形
		if (clickCount == 0) {
			pointStart = point;
			pointLastMouse = point;
			clickCount++;
		}
		else if (clickCount == 1) {
			EPolygon *pNewPolygon = new EPolygon;
			pNewPolygon->SetLineColor(penColor);
			
			pNewPolygon->SetLineWidth(lineWidth);
			pNewPolygon->Set(pointStart, point);
			pNewPolygon->DrawLast(dc);
			this->GetDocument()->dataList.Add(pNewPolygon);
			pointStart = point;
			clickCount++;
		}
		else {
			if (GetDocument()->dataList[GetDocument()->dataList.GetSize() - 1]->GetElementType() != ET_POLYGON) {
				MessageBox(_T("图形错误。"), _T("提示"), MB_OK | MB_ICONWARNING);
				CancelOpr();
			}
			else {
				EPolygon * polygon = (EPolygon *)GetDocument()->dataList[GetDocument()->dataList.GetSize() - 1];
				polygon->Add(point);
				polygon->DrawLast(dc);
				clickCount++;
				pointStart = point;
			}
		}
		break;
	case OPR_DRAW_ELLIPSE:		// 椭圆
		if (clickCount == 0) {
			pointStart = point;
			pointLastMouse = point;
			clickCount++;
		}
		else if (clickCount == 1) {
			CancelOpr();
			EEllipse *pNewEllipse = new EEllipse;
			pNewEllipse->Set(pointStart, point);
			pNewEllipse->SetLineColor(penColor);
			pNewEllipse->SetFillColor(fillColor);
			pNewEllipse->SetLineWidth(lineWidth);
			pNewEllipse->Draw(dc);
			this->GetDocument()->dataList.Add(pNewEllipse);
			OnDrawEllipse();
		}
		break;
	case OPR_EDIT_SELECT_RANGE:	// 选取范围
		if (clickCount == 0) {
			pointStart = point;
			pointLastMouse = point;
			clickCount++;
		}
		else if (clickCount == 1) {
			CancelOpr();
			// 注意必须取左上和右下
			int left =   pointStart.x > point.x ? point.x : pointStart.x;
			int top =    pointStart.y > point.y ? point.y : pointStart.y;
			int right =  pointStart.x > point.x ? pointStart.x : point.x;
			int bottom = pointStart.y > point.y ? pointStart.y : point.y;
			int addCount = 0;
			CRect rangeRect(left, top, right, bottom);
			for (int i = 0; i < GetDocument()->dataList.GetSize(); i++) {
				if (GetDocument()->dataList[i]->Select(rangeRect)) {
					if (!editList.HasElement(GetDocument()->dataList[i])) {
						// 未选取则选取并加入编辑列表
						GetDocument()->dataList[i]->Draw(dc, selectedColor);
						editList.Add(GetDocument()->dataList[i]);
						addCount++;
					}
				}
			}
			CString message;
			message.Format(_T("新选取 %d 个图形，共选取 %d 个"), addCount, editList.GetSize());
			OnEditSelectRange();
		}
		break;
	case OPR_EDIT_MOVE:			// 移动
		if (clickCount == 0) {
			pointStart = point;
			pointLastMouse = point;
			clickCount++;
		} else if (clickCount == 1) {
			CancelOpr();
			for (int i = 0; i < editList.GetSize(); i++) {
				editList[i]->Move(point.x - pointStart.x, point.y - pointStart.y);
			}
			Invalidate();		// 刷新视图
			OnEditMove();
		}
		break;
	case OPR_EDIT_ROTATE_CW:	// 顺时针旋转
		for (int i = 0; i < editList.GetSize(); i++) {
			editList[i]->RotateCW(point);
		}
		Invalidate();
		OnEditRotateCW();
		break;
	case OPR_EDIT_ROTATE_CCW:	// 顺时针旋转
		for (int i = 0; i < editList.GetSize(); i++) {
			editList[i]->RotateCCW(point);
		}
		Invalidate();
		OnEditRotateCW();
		break;
	case OPR_EDIT_SYMMETRY_H:	// 水平对称
		CancelOpr();
		for (int i = 0; i < editList.GetSize(); i++) {
			editList[i]->SymmetryH(point.x);
		}
		Invalidate();
		//pointLastMouse = point;
		OnEditSymmetryH();
		break;
	case OPR_EDIT_SYMMETRY_V:	// 垂直对称
		CancelOpr();
		for (int i = 0; i < editList.GetSize(); i++) {
			editList[i]->SymmetryV(point.y);
		}
		Invalidate();
		//pointLastMouse = point;
		OnEditSymmetryV();
		break;
	default:
		break;
	}

	//CView::OnLButtonUp(nFlags, point);
}

void CDrawDemoView::OnRButtonDown(UINT nFlags, CPoint point) {
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	//CView::OnRButtonDown(nFlags, point);
}

void CDrawDemoView::OnRButtonUp(UINT nFlags, CPoint point) {
	// 弹起右键取消当前的绘制操作
	CancelOpr();

	//CView::OnRButtonUp(nFlags, point);
}
 
void CDrawDemoView::OnMouseMove(UINT nFlags, CPoint point) {
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CDrawDemoDoc *pDoc = GetDocument();

	// 创建新画笔并备份旧画笔、笔刷
	CClientDC dc(this);
	CGdiObject *pOldBrush = dc.SelectStockObject(NULL_BRUSH);
	CPen pen;
	pen.CreatePen(PS_SOLID, auxPenWidth, auxPenColor);
	CPen *pOldPen = dc.SelectObject(&pen);

	switch (oprType) {
	case OPR_DRAW_POINT: {	// 点
		HCURSOR cursor = LoadCursor(NULL, IDC_CROSS);
		SetCursor(cursor);
		UpdateStatusBar(point, _T("画点"));
		break;
	}
	case OPR_DRAW_LINE: {	// 线
		HCURSOR cursor = LoadCursor(NULL, IDC_CROSS);
		SetCursor(cursor);
		if (clickCount == 1) {
			CString message;
			message.Format(_T("画线 | (%d, %d) + (%d ,%d)"), pointStart.x, pointStart.y, point.x - pointStart.x, point.y - pointStart.y);
			UpdateStatusBar(point, message);
			if (pointLastMouse != point) {
				// 删除旧橡皮筋
				// 利用颜色模式删除橡皮筋
				//	Refrence: http://blog.csdn.net/p40614021/article/details/8217328
				int drawMode = dc.SetROP2(R2_NOTXORPEN);
				dc.MoveTo(pointStart);
				dc.LineTo(pointLastMouse);
				// 绘制新橡皮筋
				dc.MoveTo(pointStart);
				dc.LineTo(point);
				dc.SetROP2(drawMode);
				pointLastMouse = point;
			}
		} else {
			UpdateStatusBar(point, _T("画线"));
		}
		break;
	}
	case OPR_DRAW_POLYLINE: {	// 折线
		HCURSOR cursor = LoadCursor(NULL, IDC_CROSS);
		SetCursor(cursor);
		if (clickCount > 0) {
			CString message;
			message.Format(_T("画折线 | (%d, %d) + (%d ,%d)"), pointStart.x, pointStart.y, point.x - pointStart.x, point.y - pointStart.y);
			UpdateStatusBar(point, message);
			if (pointLastMouse != point) {
				// 删除旧橡皮筋
				int drawMode = dc.SetROP2(R2_NOTXORPEN);
				dc.MoveTo(pointStart);
				dc.LineTo(pointLastMouse);
				// 绘制新橡皮筋
				dc.MoveTo(pointStart);
				dc.LineTo(point);
				dc.SetROP2(drawMode);
				pointLastMouse = point;
			}
		}
		else {
			UpdateStatusBar(point, _T("画折线"));
		}
		break;
	}
	case OPR_DRAW_RECTANGLE: {	// 矩形
		HCURSOR cursor = LoadCursor(NULL, IDC_CROSS);
		SetCursor(cursor);
		if (clickCount == 1) {
			CString message;
			message.Format(_T("画矩形 | (%d, %d) + (%d ,%d)"), pointStart.x, pointStart.y, point.x - pointStart.x, point.y - pointStart.y);
			UpdateStatusBar(point, message);
			if (pointLastMouse != point) {
				// 删除旧橡皮筋
				int drawMode = dc.SetROP2(R2_NOTXORPEN);
				dc.Rectangle(pointStart.x, pointStart.y, pointLastMouse.x, pointLastMouse.y);
				// 绘制新橡皮筋
				dc.Rectangle(pointStart.x, pointStart.y, point.x, point.y);
				dc.SetROP2(drawMode);
				pointLastMouse = point;
			}
		} else {
			UpdateStatusBar(point, _T("画矩形"));
		}
		break;
	}
	case OPR_DRAW_POLYGON: {	// 多边形
		HCURSOR cursor = LoadCursor(NULL, IDC_CROSS);
		SetCursor(cursor);
		if (clickCount > 0) {
			CString message;
			message.Format(_T("画多边形 | (%d, %d) + (%d ,%d)"), pointStart.x, pointStart.y, point.x - pointStart.x, point.y - pointStart.y);
			UpdateStatusBar(point, message);
			if (pointLastMouse != point) {
				// 删除旧橡皮筋
				int drawMode = dc.SetROP2(R2_NOTXORPEN);
				dc.MoveTo(pointStart);
				dc.LineTo(pointLastMouse);
				// 绘制新橡皮筋
				dc.MoveTo(pointStart);
				dc.LineTo(point);
				dc.SetROP2(drawMode);
				pointLastMouse = point;
			}
		}
		else {
			UpdateStatusBar(point, _T("画多边形"));
		}
		break;
	}
	case OPR_DRAW_ELLIPSE: {	// 椭圆
		HCURSOR cursor = LoadCursor(NULL, IDC_CROSS);
		SetCursor(cursor);
		if (clickCount == 1) {
			CString message;
			message.Format(_T("画椭圆 | (%d, %d) + (%d ,%d)"), pointStart.x, pointStart.y, point.x - pointStart.x, point.y - pointStart.y);
			UpdateStatusBar(point, message);
			if (pointLastMouse != point) {
				// 删除旧橡皮筋
				int drawMode = dc.SetROP2(R2_NOTXORPEN);
				dc.Ellipse(pointStart.x, pointStart.y, pointLastMouse.x, pointLastMouse.y);
				// 绘制新橡皮筋
				dc.Ellipse(pointStart.x, pointStart.y, point.x, point.y);
				dc.SetROP2(drawMode);
				pointLastMouse = point;
			}
		} else {
			UpdateStatusBar(point, _T("画椭圆"));
		}
		break;
	}
	case OPR_EDIT_SELECT: {			// 点选
		HCURSOR cursor = LoadCursor(NULL, IDC_CROSS);
		SetCursor(cursor);
		UpdateStatusBar(point, _T("点选"));
		break;
	}
	case OPR_EDIT_SELECT_RANGE: {	// 选取范围
		HCURSOR cursor = LoadCursor(NULL, IDC_CROSS);
		SetCursor(cursor);
		if (clickCount == 1) {
			CString message;
			message.Format(_T("选取范围 | (%d, %d) + (%d ,%d)"), pointStart.x, pointStart.y, point.x - pointStart.x, point.y - pointStart.y);
			UpdateStatusBar(point, message);
			if (pointLastMouse != point) {
				// 删除旧橡皮筋
				int drawMode = dc.SetROP2(R2_NOTXORPEN);
				dc.Rectangle(pointStart.x, pointStart.y, pointLastMouse.x, pointLastMouse.y);
				// 绘制新橡皮筋
				dc.Rectangle(pointStart.x, pointStart.y, point.x, point.y);
				dc.SetROP2(drawMode);
				pointLastMouse = point;
			}
		} else {
			UpdateStatusBar(point, _T("选取范围"));
		}
		break;
	}
	case OPR_EDIT_MOVE: {		// 移动
		HCURSOR cursor = LoadCursor(NULL, IDC_CROSS);
		SetCursor(cursor);
		if (clickCount == 1) {
			CString message;
			message.Format(_T("移动 | (%d, %d) + (%d ,%d)"), pointStart.x, pointStart.y, point.x - pointStart.x, point.y - pointStart.y);
			UpdateStatusBar(point, message);
			if (pointLastMouse != point) {
				// 删除旧橡皮筋
				int drawMode = dc.SetROP2(R2_NOTXORPEN);
				dc.MoveTo(pointStart);
				dc.LineTo(pointLastMouse);
				// 绘制新橡皮筋
				dc.MoveTo(pointStart);
				dc.LineTo(point);
				dc.SetROP2(drawMode);
				pointLastMouse = point;
			}
		} else {
			UpdateStatusBar(point, _T("移动"));
		}
		break;
	}
	case OPR_EDIT_ZOOM: {		// 缩放
		HCURSOR cursor = LoadCursor(NULL, IDC_CROSS);
		SetCursor(cursor);
		UpdateStatusBar(point, _T("缩放"));
		break;
	}
	case OPR_EDIT_ROTATE_CW: {	// 顺时针旋转
		HCURSOR cursor = LoadCursor(NULL, IDC_CROSS);
		SetCursor(cursor);
		UpdateStatusBar(point, _T("顺时针旋转"));
		break;
	}
	case OPR_EDIT_ROTATE_CCW: {	// 逆时针旋转
		HCURSOR cursor = LoadCursor(NULL, IDC_CROSS);
		SetCursor(cursor);
		UpdateStatusBar(point, _T("逆时针旋转"));
		break;
	}
	case OPR_EDIT_SYMMETRY_H: {	// 水平对称
		CString message;
		message.Format(_T("水平对称 | X = %d"), point.x);
		UpdateStatusBar(message);
		if (pointLastMouse != point) {
			int drawMode = dc.SetROP2(R2_NOTXORPEN);
			if (clickCount == 1) {
				// 删除旧橡皮筋
				dc.MoveTo(pointLastMouse.x, 0);
				dc.LineTo(pointLastMouse.x, clientRect.bottom);
			} else {
				clickCount = 1;
			}
			// 绘制新橡皮筋
			dc.MoveTo(point.x, 0);
			dc.LineTo(point.x, clientRect.bottom);
			dc.SetROP2(drawMode);
			pointLastMouse = point;
		}
		break;
	}
	case OPR_EDIT_SYMMETRY_V: {	// 垂直对称
		CString message;
		message.Format(_T("垂直对称 | X = %d"), point.x);
		UpdateStatusBar(message);
		if (pointLastMouse != point) {
			int drawMode = dc.SetROP2(R2_NOTXORPEN);
			if (clickCount == 1) {
				// 删除旧橡皮筋
				dc.MoveTo(0, pointLastMouse.y);
				dc.LineTo(clientRect.right, pointLastMouse.y);
			} else {
				clickCount = 1;
			}
			
			// 绘制新橡皮筋
			dc.MoveTo(0, point.y);
			dc.LineTo(clientRect.right, point.y);
			dc.SetROP2(drawMode);
			pointLastMouse = point;
		}
		break;
	}
	default:
		break;
	}
	
	// 恢复旧画笔、笔刷
	dc.SelectObject(pOldPen);
	dc.SelectObject(pOldBrush);

	CView::OnMouseMove(nFlags, point);
}

BOOL CDrawDemoView::OnMouseWheel(UINT nFlags, short zDelta, CPoint point) {
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CClientDC dc(this);

	switch (oprType) {
	case OPR_EDIT_ZOOM:
		for (int i = 0; i < editList.GetSize(); i++) {
			double level = 1.0;
			if (zDelta > 0) {
				level = 0.9;
			} else if (zDelta < 0) {
				level = 1.1;
			}
			editList[i]->Zoom(point, level);
			Invalidate();	// 刷新视图
		}
		break;
	default:
		break;
	}

	return 1;//CView::OnMouseWheel(nFlags, zDelta, point);
}

// END: - 鼠标消息处理程序

// MARK: - 键盘消息处理程序
/* 键盘快捷键
	ESC		取消当前操作
	BACK	删除
	DELETE	删除
	M		选取范围
	V		移动
	Z		缩放
*/

// END: - 键盘消息处理程序

// MARK: - 自定义消息处理程序

void CDrawDemoView::OnCancelOpr() {
	// 取消当前操作
	CancelOpr(true);
}

// END: - 自定义消息处理程序

/* END: - CDrawDemoView 消息处理程序 */

/* MARK: - 实现 */
bool CDrawDemoView::UpdateStatusBar(CString message, int position) {
	/* 更新状态栏
		参数
			CString	message		要显示的信息
			int		<position>	状态栏的栏位，默认为最左侧
		返回
			bool				更新成功 -> true，更新失败 -> false 
	*/
	// 访问 MainFrame
	//	Refrence: http://forums.codeguru.com/showthread.php?45132-How-to-Access-MFC-Status-Bar-from-View-class
	//	Refrence: http://forums.codeguru.com/showthread.php?167027-How-do-I-get-a-pointer-to-the-Main-Frame-object-(MFC-SDI)
	CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();

	return pFrame->m_wndStatusBar.SetPaneText(position, message);
}

void CDrawDemoView::UpdateStatusBar(CPoint point, CString headMsg) {
	/* 在状态栏上显示坐标
		显示结果为
			<headMsg | >位置 (X, Y)
		参数
			CPoint	point		要显示坐标的点
			CString	<headMsg>	前缀信息，默认为空
	*/
	CString positionString;
	if (headMsg.IsEmpty()) {
		positionString.Format(_T("位置 (%d, %d)"), headMsg, point.x, point.y);
	} else {
		positionString.Format(_T("%s | 位置 (%d, %d)"), headMsg, point.x, point.y);
	}
	UpdateStatusBar(positionString);
}

void CDrawDemoView::ResetStatusBar(int position) {
	/* 重置状态栏消息
		参数
			int		<position>	状态栏的栏位，默认为最左侧
	*/
	CString defaultMsg;
	switch (position) {
	case 0:
		// 调用 String Table 中的字符串
		//	Refrence: https://www.codeproject.com/Articles/10542/Easily-Load-and-Format-Strings-from-the-String-Tab
		defaultMsg.LoadStringW(AFX_IDS_IDLEMESSAGE);
		break;
	default:
		break;
	}
	UpdateStatusBar(defaultMsg, position);
}

// 取消操作
void CDrawDemoView::CancelOpr(bool isClearSelected) {
	oprType = OPR_NONE;
	if (isClearSelected) {
		editList.RemoveAll();
	}
	Invalidate();	// 刷新屏幕
	ResetStatusBar();
}

// 检查 EditList 是否为空并弹出提示
bool CDrawDemoView::CheckEditListNotEmpty() {
	bool result = true;
	Operation  tempOpr = oprType;
	CancelOpr();
	if (editList.GetSize() == 0) {
		if (tempOpr == OPR_EDIT_SELECT || tempOpr == OPR_EDIT_SELECT_RANGE) {
			if (MessageBox(_T("未选择图形，是否继续选择？"), _T("提示"), MB_OKCANCEL | MB_ICONWARNING) == IDCANCEL) {
				CancelOpr();
			}
		}
		else {
			MessageBox(_T("未选择图形。"), _T("提示"), MB_OK | MB_ICONWARNING);
		}
		result = false;
	}
	return result;
}

/* END: - 实现 */
