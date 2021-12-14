
// MainFrm.h: CMainFrame 类的接口
//

#pragma once
#include "CGame.h"

class CMainFrame : public CFrameWnd
{
public:
	CMainFrame() noexcept;
protected: // 仅从序列化创建
	DECLARE_DYNCREATE(CMainFrame)

// 特性
public:

// 操作
public:

// 重写
public:
	//virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 实现
public:
	virtual ~CMainFrame();
//#ifdef _DEBUG
//	virtual void AssertValid() const;
//	virtual void Dump(CDumpContext& dc) const;
//#endif

protected:  // 控件条嵌入成员
	CToolBar          m_wndToolBar;
	CStatusBar        m_wndStatusBar;

// 生成的消息映射函数
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()

	int m_iWidth;//窗口 宽度
	int m_iHeight;//窗口 高度

	enum ETimerId {ETimerIdGameLoop = 1};
	CGame m_game;//游戏类的对象
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};


