#include "pch.h"
#include "CGame.h"
#include <algorithm>

CGame::CGame()
{

}

CGame::~CGame() {}

bool CGame::EnterFrame(DWORD dwTime)
{
	GameRunDraw();
	GameLogic();
	return false;
}

void CGame::SetHandle(HWND hWnd)
{
	m_hWnd = hWnd;
}

void CGame::OnMouseMove(UINT nFlags, CPoint point)
{
	m_menuSelect.OnMouseMove(nFlags, point);
}

void CGame::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_menuSelect.ONLButtonUp(nFlags, point);
}

void CGame::GameRunDraw()
{
	HDC hdc = ::GetDC(m_hWnd);//获取设备句柄

	CRect rc;
	GetClientRect(m_hWnd, &rc);
	CDC* dc = CClientDC::FromHandle(hdc);//把句柄转化为CDC对象

	CDC m_dcMemory;
	CBitmap bmp;
	bmp.CreateCompatibleBitmap(dc, rc.Width(), rc.Height()); // 创建对象

	m_dcMemory.CreateCompatibleDC(dc);//创建设备句柄
	CBitmap* pOldBitmap = m_dcMemory.SelectObject(&bmp);//句柄选择位图

	Graphics gh(m_dcMemory.GetSafeHdc());//创建类的对象
	gh.Clear(Color::White);
	gh.ResetClip();
	//绘制
	DrawFps(gh);//绘制帧数
	//绘制图片
	{
		/*m_menu.Draw(gh);

		m_menuSelect.Draw(gh);*/
		m_player01.Draw(gh);
		for (auto &blt : m_lstBullets)
		{
			blt.Draw(gh);
		}
	}

	::BitBlt(hdc, 0, 0, rc.Width(), rc.Height(), m_dcMemory.GetSafeHdc(), 0, 0, SRCCOPY);
	
	dc->DeleteDC();
	return;

}

void CGame::GameLogic()
{
	#define KEYDOWN(vk) (GetAsyncKeyState(vk) & (0x8000))//使用windowsAPI 和 与运算 创建一个宏
	{
		if (KEYDOWN(VK_LEFT))
		{
			m_player01.RotateLeft();
		}
		if (KEYDOWN(VK_RIGHT))
		{
			m_player01.RotateRight();
		}
		if (KEYDOWN(VK_UP))
		{
			m_player01.Forward();
		}
		if (KEYDOWN(VK_DOWN))
		{
			m_player01.Backward();
		}
		if (KEYDOWN('M'))
		{
			CBullet blt;
			if (m_player01.Fire(blt))
			{
				m_lstBullets.push_back(blt);
			}
		}

		for (auto& blt : m_lstBullets)
		{
			blt.Move();
		}
		{
			//移除超时的子弹，调用了标准库的函数
			auto itRemove = std::remove_if(m_lstBullets.begin(), m_lstBullets.end(),
				[](CBullet& blt)->bool {return blt.IsTimeout(); });

			for (auto it = itRemove; it != m_lstBullets.end(); ++it)
			{
				it->SetActive(false);
				it->GetOwner()->AddBullet(*it);
			}
			m_lstBullets.erase(itRemove, m_lstBullets.end());
		}
	}
}

void CGame::DrawFps(Graphics& gh)
{
	static int fps = 0;
	m_fps++;
	static DWORD dwLast = GetTickCount();
	if (GetTickCount() - dwLast >= 1000)
	{
		fps = m_fps;
		m_fps = 0;
		dwLast = GetTickCount();

	}

	{
		CString s;
		s.Format(_T("FPS:%d"), fps);
		SolidBrush brush(Color(0, 0, 0xFF));
		Gdiplus::Font font(_T("宋体"), 10.0);
		CRect rc;
		::GetClientRect(m_hWnd, &rc);
		PointF origin(static_cast<float>(rc.right - 50), static_cast<float>(rc.top + 2));
		gh.DrawString(s.GetString(), -1, &font, origin, &brush);
	}
}
