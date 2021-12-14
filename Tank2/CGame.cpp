#include "pch.h"
#include "CGame.h"
CGame::CGame()
{

}

CGame::~CGame() {}

bool CGame::EnterFrame(DWORD dwTime)
{
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
