
// Tank2View.cpp: CTank2View 类的实现
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "Tank2.h"
#endif

#include "Tank2Doc.h"
#include "Tank2View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTank2View

IMPLEMENT_DYNCREATE(CTank2View, CView)

BEGIN_MESSAGE_MAP(CTank2View, CView)
END_MESSAGE_MAP()

// CTank2View 构造/析构

CTank2View::CTank2View() noexcept
{
	// TODO: 在此处添加构造代码

}

CTank2View::~CTank2View()
{
}

BOOL CTank2View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CTank2View 绘图

void CTank2View::OnDraw(CDC* /*pDC*/)
{
	CTank2Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}


// CTank2View 诊断

#ifdef _DEBUG
void CTank2View::AssertValid() const
{
	CView::AssertValid();
}

void CTank2View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CTank2Doc* CTank2View::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTank2Doc)));
	return (CTank2Doc*)m_pDocument;
}
#endif //_DEBUG


// CTank2View 消息处理程序
