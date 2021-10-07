#include "myWrite.h"
bool myWrite::Set(HWND hWnd, int iWidth, int iHeight,
	IDXGISurface1* pSurface)
{

	m_iWidth = iWidth;
	m_iHeight = iHeight;
	//독립적인 리소스 생성
	CreateIndependentResource();
	//종속적인 리소스 생성
	CreateDependentResource(iWidth, iHeight, pSurface);
	return false;
}

void myWrite::ResizeDevice(int iWidth, int iHeight, IDXGISurface1* pSurface)
{
	//종속적인 리소스 생성
	CreateDependentResource(iWidth, iHeight, pSurface);
}

HRESULT myWrite::CreateIndependentResource()
{
	//2d 팩토리 생성
	HRESULT hr = D2D1CreateFactory(
		D2D1_FACTORY_TYPE_MULTI_THREADED,
		&m_pd2dFactory);
	if (FAILED(hr))
	{
		return hr;
	}
	//인치당 점의 갯수를 받아온다
	m_iDpi = GetDpiForWindow(g_hWnd);
	//출력 팩토리 생성
	IDWriteFactory* m_pd2dWrite;
	hr = DWriteCreateFactory(
		DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(IDWriteFactory),
		(IUnknown**)&m_pd2dWrite);
	if (FAILED(hr))
	{
		return hr;
	}
	//텍스트 옵션 설정
	hr = m_pd2dWrite->CreateTextFormat(
		L"궁서",//L"Gabriola",
		NULL,
		DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		20,
		L"ko-kr",//L"en-us",//L"ko-kr"
		&m_pTextFormat);
	if (FAILED(hr))
	{
		return hr;
	}


	return S_OK;
}

HRESULT myWrite::CreateDependentResource(int iWidth, int iHeight, IDXGISurface1* pSurface)
{
	//2d팩토리가 생성되어있지 않으면 리턴
	if (m_pd2dFactory == nullptr) return S_OK;
	//랜더타겟의 정보 세팅
	D2D1_RENDER_TARGET_PROPERTIES Properties;
	Properties.type = D2D1_RENDER_TARGET_TYPE_DEFAULT;
	Properties.pixelFormat.format = DXGI_FORMAT_UNKNOWN;
	Properties.pixelFormat.alphaMode =
		D2D1_ALPHA_MODE_PREMULTIPLIED;
	Properties.dpiX = (FLOAT)m_iDpi;
	Properties.dpiY = (FLOAT)m_iDpi;
	Properties.usage = D2D1_RENDER_TARGET_USAGE_NONE;
	Properties.minLevel = D2D1_FEATURE_LEVEL_DEFAULT;
	//랜더타겟 서페이스 생성
	HRESULT hr = m_pd2dFactory->CreateDxgiSurfaceRenderTarget(
		pSurface,
		&Properties,
		&m_pd2dRT);
	if (FAILED(hr))
	{
		return hr;
	}
	D2D1_COLOR_F color;
	color.r = 255.0f;
	color.g = 0.0f;
	color.b = 0.0f;
	color.a = 255.0f;
	//브러시 생성
	hr = m_pd2dRT->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::Red),
		&m_pBrush);
	if (FAILED(hr))
	{
		return hr;
	}
	return hr;
}

void myWrite::DeleteDependentResource()
{
	if (m_pBrush)
	{
		m_pBrush->Release();
	}
	if (m_pd2dRT)
	{
		m_pd2dRT->Release();
	}
	m_pBrush = nullptr;
	m_pd2dRT = nullptr;
}

bool myWrite::Init()
{
	return true;
}

bool myWrite::Frame()
{
	return true;
}

bool myWrite::Render()
{
	if (m_pd2dRT == nullptr) return true;
	m_pd2dRT->BeginDraw();
	{
		std::lock_guard<std::mutex> lock(m_hMutex);
		for (int iMsg = 0; iMsg < m_TextList.size(); iMsg++)
		{
			m_TextList[iMsg].rt.top = 25 + iMsg * 25;
			D2D1_MATRIX_3X2_F matWorld = D2D1::IdentityMatrix();
			m_pd2dRT->SetTransform(matWorld);
			D2D1_RECT_F layoutRect =
			{
				(float)m_TextList[iMsg].rt.left,
				(float)m_TextList[iMsg].rt.top,
				(float)m_TextList[iMsg].rt.right,
				(float)m_TextList[iMsg].rt.bottom,
			};
			D2D1::ColorF fColor = m_TextList[iMsg].dwColor;
			m_pBrush->SetColor(fColor);
			m_pd2dRT->DrawText(
				m_TextList[iMsg].msg.c_str(),
				(UINT32)m_TextList[iMsg].msg.size(),
				m_pTextFormat,
				&layoutRect,
				m_pBrush);
		}
	}
	m_pd2dRT->EndDraw();
	return true;
}

bool myWrite::Release()
{
	if (m_pd2dFactory == nullptr) return true;
	if (m_pBrush)m_pBrush->Release();
	if (m_pd2dFactory)m_pd2dFactory->Release();
	if (m_pd2dRT)m_pd2dRT->Release();
	if (m_pd2dWrite)m_pd2dWrite->Release();
	if (m_pTextFormat)m_pTextFormat->Release();
	return true;
}

void   myWrite::push(myMsgBox& msg)
{
	std::lock_guard<std::mutex> lock(m_hMutex);
	if (m_TextList.size() > 10)
	{
		m_TextList.pop_back();
	}
	m_TextList.insert(m_TextList.begin(), msg);
}
void myWrite::Draw(int x, int y, wstring msg, DWORD color)
{
	POINT p = { x, y };
	Draw(p, msg, color);
}
void myWrite::Draw(POINT pos, wstring msg, DWORD color)
{
	std::lock_guard<std::mutex> lock(m_hMutex);
	if (m_pd2dRT == nullptr) return;
	m_pd2dRT->BeginDraw();
	{
		D2D1_RECT_F layoutRect =
		{
			(float)pos.x, 
			(float)pos.y, 
			(float)g_rtClient.right,
			(float)g_rtClient.bottom
		};
		D2D1_MATRIX_3X2_F matWorld = D2D1::IdentityMatrix();
		m_pd2dRT->SetTransform(matWorld);
		D2D1::ColorF fColor = color;
		m_pBrush->SetColor(fColor);
		m_pd2dRT->DrawText(
			msg.c_str(),
			(UINT32)msg.size(),
			m_pTextFormat,
			&layoutRect,
			m_pBrush);
	}
	m_pd2dRT->EndDraw();
}

myWrite::myWrite()
{
	m_iDpi = 0;
	m_iWidth = 0;
	m_iHeight = 0;
	m_pd2dFactory = nullptr;
	m_pd2dRT = nullptr;
	m_pd2dWrite = nullptr;
	m_pTextFormat = nullptr;
}

myWrite::~myWrite()
{
}