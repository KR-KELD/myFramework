#include "myCore.h"
HRESULT myCore::DeleteDXResource()
{
	//종속적인 리소스 제거
	I_Write.DeleteDependentResource();
	return S_OK;
}

HRESULT myCore::CreateDXResource(UINT w, UINT h)
{
	//백버퍼 초기화
	ComPtr<IDXGISurface1> pBackBuffer = nullptr;
	//백버퍼 가져오기
	m_pSwapChain->GetBuffer(0, __uuidof(IDXGISurface), (LPVOID*)pBackBuffer.GetAddressOf());
	//리소스들을 백버퍼에 리사이즈해서 단다
	I_Write.ResizeDevice(w, h, pBackBuffer.Get());
	//백버퍼 제거
	return S_OK;
}

bool myCore::GameInit()
{
	PreInit();
	m_isGameRun = true;
	if (myDevice::Init() == false)
	{
		return false;
	}
	SetMode(m_bFullScreen);

	m_Timer.Init();
	g_Input.Init();
	g_SoundMgr.Init();
	ComPtr<IDXGISurface1> pBackBuffer = nullptr;
	m_pSwapChain->GetBuffer(0, __uuidof(IDXGISurface),
		(LPVOID*)pBackBuffer.GetAddressOf());
	//dx2d 드로우 세팅
	I_Write.Set(m_hWnd,
		g_rtClient.right,
		g_rtClient.bottom, pBackBuffer.Get());


	Init();
	PostInit();
	return true;
}

bool myCore::GameFrame()
{
	PreFrame();
	m_Timer.Frame();
	g_Input.Frame();
	g_SoundMgr.Frame();
	if (g_Input.GetKey('0') == KEY_PUSH)
	{
		m_isDebugText = !m_isDebugText;
	}
	Frame();
	PostFrame();
	return true;
}

bool myCore::PreRender()
{
	myDevice::PreRender();

	//레스터라이저 스테이트 세팅
	m_pd3dContext->RSSetState(myDxState::g_pRSEdit.Get());
	//픽셀 섀이더에 샘플러 세팅(보간법)
	m_pd3dContext->PSSetSamplers(0, 1, &myDxState::g_pSSWrapLinear);
	//뎁스 스탠실 스테이트 세팅(깊이값 버퍼)
	m_pd3dContext->OMSetDepthStencilState(myDxState::g_pDSSDepthEnable, 0);
	return true;
}

bool myCore::GameRender()
{
	if (PreRender() == false) return false;
	if (Render() == false) return false;
	if (PostRender() == false) return false;
	return true;
}

bool myCore::PostRender()
{
	m_Timer.Render();
	g_Input.Render();
	I_Write.Render();
	g_SoundMgr.Render();
	if (m_isDebugText)
	{
		I_Write.Draw(0, 0, m_Timer.m_szBuffer);
	}

	myDevice::PostRender();
	return true;
}

bool myCore::GameRun()
{
	if (GameFrame() == false) return false;
	if (GameRender() == false) return false;
	return true;
}

bool myCore::Run()
{
	//HeapEnableTerminateOnCorruption 옵션을 설정하면 손상된 힙을 이용하는 보안 악용에 대한 애플리케이션의 노출을 줄일 수 있으므로 강력하게 권장.
	//HeapSetInformation(NULL, HeapEnableTerminationOnCorruption, NULL, 0);

	HRESULT hr = S_OK;
	//CoInitializeEx 는 COM 라이브러리를 사용하는 각 스레드에 대해 한 번 이상 호출해야  한다.
	hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
		if (GameInit())
		{
			ShowWindow(m_hWnd, SW_SHOWNORMAL);
		}
		while (m_isGameRun)
		{
			if (MsgProcess() == true)
			{
				GameRun();
			}
			else
			{
				m_isGameRun = false;
			}
		}
		GameRelease();
	//이니셜라이즈 해제
	CoUninitialize();
	return true;
}

myCore::myCore()
{
	m_isGameRun = false;
	m_isDebugText = false;
}

myCore::~myCore()
{
}

bool myCore::GameRelease()
{
	Release();
		I_Write.Release();
		m_Timer.Release();
		g_Input.Release();
		g_SoundMgr.Release();
	myDevice::Release();
	return true;
}

bool myCore::InitTool(HWND hWnd, HINSTANCE hInstance)
{
	HRESULT hr = S_OK;
	g_hWnd = m_hWnd = hWnd;					//전역 핸들에 전달
	GetClientRect(m_hWnd, &m_rtClient);		//클라이언트의 크기를 클라이언트 렉트에 담는다
	GetWindowRect(m_hWnd, &m_rtWindow);
	g_rtClient = m_rtClient;
	g_hInstance = hInstance;
	hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
	GameInit();
	return true;
}

bool myCore::ReleaseTool()
{
	GameRelease();
	//이니셜라이즈 해제
	CoUninitialize();
	return true;
}

bool myCore::RunTool()
{
	if (GameFrame() == false) return false;
	if (GameRender() == false) return false;
	return true;
}
