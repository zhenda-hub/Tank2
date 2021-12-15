
// MainFrm.cpp: CMainFrame 类的实现
//

#include "pch.h"
#include "framework.h"
#include "Tank2.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define GAME_WIN_W (800)//窗口宽度
#define GAME_WIN_H (600)//窗口高度

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)
//消息映射
BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 状态行指示器
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame 构造/析构

CMainFrame::CMainFrame() noexcept
{
	// TODO: 在此添加成员初始化代码
	m_iWidth = 800;
	m_iHeight = 600;
#define MY_STYLE (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU |\
	WS_MINIMIZEBOX | FWS_ADDTOTITLE)
	//窗口
	Create(NULL, _T("明日科技.坦克大战"), MY_STYLE, 
		CRect(0, 0, GAME_WIN_W, GAME_WIN_H));
	//客户区
	{
		CRect rcCli;
		GetClientRect(rcCli);
		RECT rcFrame = { 0, 0, m_iWidth + m_iWidth - rcCli.right,
		m_iHeight + m_iHeight - rcCli.bottom };//边框大小
		MoveWindow(&rcFrame, TRUE);//使得主窗口在左上角显示
	}

}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)//创建窗口
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	SetTimer(ETimerIdGameLoop, 0, NULL);//设置定时器
	m_game.SetHandle(GetSafeHwnd());//把当前窗口句柄传给CGame类

	return 0;
}

void CMainFrame::OnTimer(UINT_PTR nIDEvent)
{
	switch (nIDEvent)
	{
	case ETimerIdGameLoop:
		static DWORD dwLastUpdate = GetTickCount();
		if (GetTickCount64()-dwLastUpdate >= 20)
		{
			m_game.EnterFrame(GetTickCount());
			break;
		}
		
	default://其他消息，让系统默认处理
		break;
	}

	
	
}

void CMainFrame::OnMouseMove(UINT nFlags, CPoint point)
{
	m_game.OnMouseMove(nFlags, point);//转发消息给CGame类
	CFrameWnd::OnMouseMove(nFlags, point);//转发消息给父类
}
void CMainFrame::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_game.OnLButtonUp(nFlags, point);
	CFrameWnd::OnLButtonUp(nFlags, point);
}
