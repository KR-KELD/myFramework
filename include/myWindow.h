#pragma once
#include "myCoreStd.h"
class myWindow
{
public:
	static const int WINDOWSTARTX = 300;
	static const int WINDOWSTARTY = 300;
	static const int WINDOWSIZEX = 720;
	static const int WINDOWSIZEY = 480;
protected:

	HINSTANCE	m_hInstance;	//�ν��Ͻ� �ڵ�
	HWND		m_hWnd;			//������ �ڵ�
	DWORD		m_dwExStyle;
	DWORD		m_dwStyle;
	T_STR		m_szClassName;
	T_STR		m_szWindowName;
	bool		m_bFullScreen = false;
public:
	MSG			m_msg;			//�޽��� ����ü
	RECT		m_rtClient;		//Ŭ���̾�Ʈ ��Ʈ
	RECT		m_rtWindow;
	bool		m_bDrag;
	POINT		m_ptClick;
public:
	bool		SetWindow(HINSTANCE hInstance);
	bool		MsgProcess();
public:
	//virtual	void	MsgEvent(MSG msg);
	virtual void ResizeDevice(UINT w, UINT h);
	virtual LRESULT	 MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
public:
	myWindow();
	virtual ~myWindow();
};

