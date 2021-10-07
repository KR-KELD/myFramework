#include "myCore.h"
HRESULT myCore::DeleteDXResource()
{
	//�������� ���ҽ� ����
	I_Write.DeleteDependentResource();
	return S_OK;
}

HRESULT myCore::CreateDXResource(UINT w, UINT h)
{
	//����� �ʱ�ȭ
	ComPtr<IDXGISurface1> pBackBuffer = nullptr;
	//����� ��������
	m_pSwapChain->GetBuffer(0, __uuidof(IDXGISurface), (LPVOID*)pBackBuffer.GetAddressOf());
	//���ҽ����� ����ۿ� ���������ؼ� �ܴ�
	I_Write.ResizeDevice(w, h, pBackBuffer.Get());
	//����� ����
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
	//dx2d ��ο� ����
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

	//�����Ͷ����� ������Ʈ ����
	m_pd3dContext->RSSetState(myDxState::g_pRSEdit.Get());
	//�ȼ� ���̴��� ���÷� ����(������)
	m_pd3dContext->PSSetSamplers(0, 1, &myDxState::g_pSSWrapLinear);
	//���� ���Ľ� ������Ʈ ����(���̰� ����)
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
	//HeapEnableTerminateOnCorruption �ɼ��� �����ϸ� �ջ�� ���� �̿��ϴ� ���� �ǿ뿡 ���� ���ø����̼��� ������ ���� �� �����Ƿ� �����ϰ� ����.
	//HeapSetInformation(NULL, HeapEnableTerminationOnCorruption, NULL, 0);

	HRESULT hr = S_OK;
	//CoInitializeEx �� COM ���̺귯���� ����ϴ� �� �����忡 ���� �� �� �̻� ȣ���ؾ�  �Ѵ�.
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
	//�̴ϼȶ����� ����
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
	g_hWnd = m_hWnd = hWnd;					//���� �ڵ鿡 ����
	GetClientRect(m_hWnd, &m_rtClient);		//Ŭ���̾�Ʈ�� ũ�⸦ Ŭ���̾�Ʈ ��Ʈ�� ��´�
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
	//�̴ϼȶ����� ����
	CoUninitialize();
	return true;
}

bool myCore::RunTool()
{
	if (GameFrame() == false) return false;
	if (GameRender() == false) return false;
	return true;
}
