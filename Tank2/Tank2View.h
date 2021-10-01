
// Tank2View.h: CTank2View 类的接口
//

#pragma once


class CTank2View : public CView
{
protected: // 仅从序列化创建
	CTank2View() noexcept;
	DECLARE_DYNCREATE(CTank2View)

// 特性
public:
	CTank2Doc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// 实现
public:
	virtual ~CTank2View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // Tank2View.cpp 中的调试版本
inline CTank2Doc* CTank2View::GetDocument() const
   { return reinterpret_cast<CTank2Doc*>(m_pDocument); }
#endif

