#pragma once
//����ҵ�����̣� ϵͳ -> ���� -> �����߼�
class CGame
{
	HWND m_hWnd;//����
public:
	CGame();
	~CGame();
	void SetHandle(HWND hWnd);//������ڵľ��

	bool EnterFrame(DWORD dwTime);

	void OnMouseMove(UINT nFlags, CPoint point);
	void OnLButtonUp(UINT nFlags, CPoint point);
};

