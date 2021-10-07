#pragma once
#include "myBasicStd.h"

#include "dxtk/WICTextureLoader.h"
#include "dxtk/DDSTextureLoader.h"
#include "dxtk/ScreenGrab.h"

static const TCHAR* DataFolderPath = L"../../data/";

struct myKeyMap
{
	bool bCapsLockActive;
};

extern myKeyMap				g_KeyMap;
extern HWND					g_hWnd;
extern HINSTANCE			g_hInstance;
extern ID3D11Device* g_pd3dDevice;
extern ID3D11DeviceContext* g_pImmediateContext;
extern HDC					g_hScreenDC;
extern HDC					g_hOffScreenDC;
extern RECT					g_rtClient;
extern bool					g_bActive;
extern float				g_fGameTimer;
extern float				g_fSecondPerFrame;