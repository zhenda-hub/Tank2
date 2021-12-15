#include "pch.h"
#include "CGame.h"
CGame::CGame()
{

}

CGame::~CGame() {}

bool CGame::EnterFrame(DWORD dwTime)
{
	GameRunDraw();
	return false;
}

void CGame::SetHandle(HWND hWnd)
{
	m_hWnd = hWnd;
}

void CGame::OnMouseMove(UINT nFlags, CPoint point)
{

}

void CGame::OnLButtonUp(UINT nFlags, CPoint point)
{

}

void CGame::GameRunDraw()
{
	HDC hdc = ::GetDC(m_hWnd);//��ȡ�豸���

	CRect rc;
	GetClientRect(m_hWnd, &rc);
	CDC* dc = CClientDC::FromHandle(hdc);//�Ѿ��ת��ΪCDC����

	CDC m_dcMemory;
	CBitmap bmp;
	bmp.CreateCompatibleBitmap(dc, rc.Width(), rc.Height()); // ��������

	m_dcMemory.CreateCompatibleDC(dc);//�����豸���
	CBitmap* pOldBitmap = m_dcMemory.SelectObject(&bmp);//���ѡ��λͼ

	Graphics gh(m_dcMemory.GetSafeHdc());//������Ķ���
	gh.Clear(Color::White);
	gh.ResetClip();
	//����
	DrawFps(gh);//����֡��
	//����ͼƬ
	{
		Gdiplus::Image* img = Image::FromFile(_T("menu_background.png"));
		gh.DrawImage(img, rc.left, rc.top, rc.right, rc.bottom);
		
	}

	::BitBlt(hdc, 0, 0, rc.Width(), rc.Height(), m_dcMemory.GetSafeHdc(), 0, 0, SRCCOPY);
	
	dc->DeleteDC();
	return;

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
		Gdiplus::Font font(_T("����"), 10.0);
		CRect rc;
		::GetClientRect(m_hWnd, &rc);
		PointF origin(static_cast<float>(rc.right - 50), static_cast<float>(rc.top + 2));
		gh.DrawString(s.GetString(), -1, &font, origin, &brush);
	}
}
