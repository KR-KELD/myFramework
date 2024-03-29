#include "myWindow.h"

//전역 핸들과 전역 클라이언트렉트 초기화
HWND		g_hWnd = 0;
HINSTANCE	g_hInstance = 0;
bool		g_bActive = false;
RECT		g_rtClient = { 0, 0, 0, 0 };

static bool		m_bDrag = false;
static POINT	m_ptClick = { 0,0 };
myWindow* g_pWindow = nullptr;

//프로시저
LRESULT CALLBACK WndProc(
	HWND hWnd,
	UINT message,
	WPARAM wParam,
	LPARAM lParam)
{
	int iRet = g_pWindow->MsgProc(hWnd, message, wParam, lParam);
	if (iRet > -1)
	{
		return iRet;
	}
	switch (message)
	{
	case WM_SIZE:
	{
		//if (SIZE_MAXHIDE)// 다른윈도우가 최대화 되어 가려지면
		//if (SIZE_MAXSHOW) // 최대화 되어 가려진 후 다시 복구될 때
		//if (SIZE_MAXIMIZED)// 최대화		
		//if (SIZE_RESTORED) // 크기가 변경되었을 때
		if (SIZE_MINIMIZED != wParam) // 최소화
		{
			UINT w = LOWORD(lParam);
			UINT h = HIWORD(lParam);
			g_pWindow->m_rtClient.right = w;
			g_pWindow->m_rtClient.bottom = h;
			g_rtClient = g_pWindow->m_rtClient;
			g_pWindow->ResizeDevice(w, h);
		}
	}break;
	case WM_MBUTTONDOWN:
	{
		m_bDrag = true;
		m_ptClick.x = LOWORD(lParam);
		m_ptClick.y = HIWORD(lParam);
	}return 0;
	case WM_MOUSEMOVE:
	{
		if (m_bDrag)
		{
			RECT rtWindow;
			GetWindowRect(hWnd, &rtWindow);
			rtWindow.left -= m_ptClick.x - LOWORD(lParam);
			rtWindow.top -= m_ptClick.y - HIWORD(lParam);
#if NDEBUG
			SetWindowPos(hWnd, HWND_TOPMOST, rtWindow.left, rtWindow.top, 0, 0, SWP_NOSIZE);//HWND_TOP
#else
			SetWindowPos(hWnd, HWND_NOTOPMOST, rtWindow.left, rtWindow.top, 0, 0, SWP_NOSIZE);
#endif
		}
	}return 0;
	case WM_MBUTTONUP:
	{
		m_bDrag = false;
	}return 0;
	case WM_ACTIVATE:
	{
		if (LOWORD(wParam) == WA_INACTIVE)
		{
			g_bActive = false;
			m_bDrag = false;
		}
		else
		{
			::SetFocus(g_hWnd);
			g_bActive = true;
		}

	}break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
//윈도우 생성
bool myWindow::SetWindow(HINSTANCE hInstance)
{
	m_hInstance = hInstance;			//인스턴스 핸들
	g_hInstance = m_hInstance;
	WNDCLASSEX wc;						//윈도우 구조체
	ZeroMemory(&wc, sizeof(wc));
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;	//윈도우 스타일 지정
	wc.lpfnWndProc = WndProc;			//프로시저 등록
	wc.hbrBackground =					//배경색 지정
		(HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hInstance = m_hInstance;			//인스턴스 핸들 지정
	wc.lpszClassName = m_szClassName.c_str();		//프로세스 이름 지정
	if (RegisterClassEx(&wc) == 0)
	{
		return false;
	}
	//메뉴를 제외한 클라이언트의 크기를 렉트에 담는다
	RECT rt = { 0, 0, WINDOWSIZEX, WINDOWSIZEY };
	AdjustWindowRectEx(&rt, m_dwStyle, FALSE, m_dwExStyle);
	m_hWnd = CreateWindowEx(
		m_dwExStyle,
		m_szClassName.c_str(),				//프로세스 이름
		m_szWindowName.c_str(),				//프로그램 이름
		m_dwStyle,							//윈도우 옵션
		WINDOWSTARTX, WINDOWSTARTY,			//윈도우 시작 위치
		rt.right - rt.left, rt.bottom - rt.top,	//윈도우 크기
		nullptr, nullptr,
		m_hInstance, nullptr);
	if (m_hWnd == NULL)
	{
		return false;
	}
	g_hWnd = m_hWnd;						//전역 핸들에 전달
	GetClientRect(m_hWnd, &m_rtClient);		//클라이언트의 크기를 클라이언트 렉트에 담는다
	GetWindowRect(m_hWnd, &m_rtWindow);
	g_rtClient = m_rtClient;				//전역 클라이언트 렉트에 전달
	//ShowWindow(m_hWnd, SW_SHOW);			//윈도우 띄우기
	return true;
}
//메시지큐 처리 프로세스
bool myWindow::MsgProcess()
{
	//메시지큐에 메시지가 들어있으면 가져온다
	while (PeekMessage(&m_msg, NULL, 0, 0, PM_REMOVE))
	{
		//입력받은 키를 번역한다
		TranslateMessage(&m_msg);
		//프로시저로 전달
		DispatchMessage(&m_msg);
		if (m_msg.message == WM_QUIT)
		{
			return false;
		}
		//MsgEvent(m_msg);
	}
	return true;
}

//void myWindow::MsgEvent(MSG msg)
//{
//}

void myWindow::ResizeDevice(UINT w, UINT h)
{
}

LRESULT myWindow::MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return -1;
}

myWindow::myWindow()
{
	g_pWindow = this;
	ZeroMemory(&m_msg, sizeof(MSG));
	m_bDrag = false;
	m_szClassName = L"MyWindow";
#if NDEBUG
	m_dwExStyle = WS_EX_TOPMOST;
	m_szWindowName = L"MyGame(Release)";
	m_dwStyle = WS_POPUPWINDOW;// WS_OVERLAPPEDWINDOW,,;
#else
	m_dwExStyle = WS_EX_APPWINDOW;
	m_szWindowName = L"MyGame(Debug)";
	m_dwStyle = WS_OVERLAPPEDWINDOW; //WS_POPUPWINDOW;
#endif
}

myWindow::~myWindow()
{
}
