#pragma once
//����ҵ�����̣� ϵͳ -> ���� -> �����߼�
class CGame
{
	HWND m_hWnd;//����
	void GameRunDraw();
	void DrawFps(Graphics& gh);

	int m_fps{0};

public:
	CGame();
	~CGame();
	void SetHandle(HWND hWnd);//������ڵľ��

	bool EnterFrame(DWORD dwTime);

	void OnMouseMove(UINT nFlags, CPoint point);
	void OnLButtonUp(UINT nFlags, CPoint point);
};

