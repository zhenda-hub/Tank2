#pragma once
//����ҵ�����̣� ϵͳ -> ���� -> �����߼�
#include "CGameMenuPanel.h"
#include "GameMenuBackground.h"
class CGame
{
	HWND m_hWnd;//����
	void GameRunDraw();
	void DrawFps(Graphics& gh);

	int m_fps{0};
	CGameMenuPanel m_menuSelect;//��ʼ�˵�
	CGameMenuBackground m_menu;
public:
	CGame();
	~CGame();
	void SetHandle(HWND hWnd);//������ڵľ��

	bool EnterFrame(DWORD dwTime);

	void OnMouseMove(UINT nFlags, CPoint point);
	void OnLButtonUp(UINT nFlags, CPoint point);
};

