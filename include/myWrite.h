#pragma once
#include "myStd.h"
#include <mutex>
#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#pragma comment(lib,"d2d1.lib")
#pragma comment(lib,"dwrite.lib")
struct myMsgBox
{
	RECT	rt;
	wstring	msg;
	DWORD   dwColor;
	myMsgBox() { dwColor = 0; rt = { 0,0,0,0 }; }
	myMsgBox(wstring _msg, RECT _rt, DWORD _dwColor)
	{
		msg = _msg;
		rt = _rt;
		dwColor = _dwColor;
	}
};
class myWrite : public mySingleTon<myWrite>
{
public:
	ID2D1Factory* m_pd2dFactory;
	ID2D1RenderTarget* m_pd2dRT;
	IDWriteFactory* m_pd2dWrite;
	IDWriteTextFormat* m_pTextFormat;
	ID2D1SolidColorBrush* m_pBrush = nullptr;
	UINT				m_iWidth;
	UINT				m_iHeight;
	UINT				m_iDpi;
	//float				m_fdpiX;
	//float				m_fdpiY;
	std::vector<myMsgBox>   m_TextList;
public:
	virtual bool Init();
	virtual bool Frame();
	virtual bool Render();
	virtual bool Release();
	virtual bool Set(HWND hWnd,
		int iWidth, int iHeight,
		IDXGISurface1* pSurface);
	void ResizeDevice(int iWidth, int iHeight,
		IDXGISurface1* pSurface);
	HRESULT CreateIndependentResource();
	HRESULT CreateDependentResource(int iWidth, int iHeight,
		IDXGISurface1* pSurface);
	void DeleteDependentResource();
public:
	std::mutex      m_hMutex;
	virtual void   push(myMsgBox& msg);
	virtual void   Draw(POINT pos, wstring msg, DWORD color = RGB(255, 0, 0));
	virtual void   Draw(int x, int y, wstring msg, DWORD color = RGB(255, 0, 0));
public:
	myWrite();
	virtual ~myWrite();
};
#define I_Write myWrite::GetInstance()