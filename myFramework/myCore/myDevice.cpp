#include "myDevice.h"
ID3D11Device* g_pd3dDevice = nullptr;
ID3D11DeviceContext* g_pImmediateContext = nullptr;
HRESULT myDevice::CreateGIFactory()
{
	//����̽��� �����Ǿ� ���� ������ ����
	if (m_pd3dDevice.Get() == NULL) return E_FAIL;
	HRESULT hr;
	ComPtr<IDXGIDevice> pDXGIDevice;
	//GI����̽��� ������ uid��ȣ�� �ҷ��´�
	hr = m_pd3dDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)pDXGIDevice.GetAddressOf());
	//GI���(�׷���ī��) ������ �ҷ��´�
	ComPtr<IDXGIAdapter> pDXGIAdapter;
	hr = pDXGIDevice->GetParent(__uuidof(IDXGIAdapter), (void**)pDXGIAdapter.GetAddressOf());
	//�� ���μ����� ���� GIFactory�� �ҷ��´�
	pDXGIAdapter->GetParent(__uuidof(IDXGIFactory), (void**)m_pGIFactory.GetAddressOf());
	//�ҷ����µ� ����� ����̽��� ��͸� ������ ���ش�
	return S_OK;
}

HRESULT myDevice::CreateDevice()
{
	//D2D RT
	//dx3d�� dx2d�� �����ϴ� �÷���
	UINT	Flags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#ifdef _DEBUG
	//����� ��� �÷���
	Flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	//DirectX ����
	D3D_FEATURE_LEVEL	pFeatureLevels[] =
	{
		//D3D_FEATURE_LEVEL_12_0,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_0,
	};
	// �׷���ī��
	IDXGIAdapter* pAdapter = nullptr;
	//����� ����̹� Ÿ��
	D3D_DRIVER_TYPE		DriverType[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
	};
	HMODULE				Software = NULL; // �ܺθ�� �������

	//UINT				FeatureLevels = ARRAYSIZE(pFeatureLevels);
	UINT				FeatureLevels = sizeof(pFeatureLevels) / sizeof(pFeatureLevels[0]);
	UINT				SDKVersion = D3D11_SDK_VERSION;
	D3D_FEATURE_LEVEL   OutputFeatureLevel;

	HRESULT hr = S_OK;
	//����̽� ����
	for (int iType = 0; iType < ARRAYSIZE(DriverType); iType++)
	{
		hr = D3D11CreateDevice(
			pAdapter,
			DriverType[iType],
			Software,
			Flags,
			pFeatureLevels,
			FeatureLevels,
			SDKVersion,
			m_pd3dDevice.GetAddressOf(),
			&OutputFeatureLevel,
			m_pd3dContext.GetAddressOf());

		if (SUCCEEDED(hr))
		{
			break;
		}
	}
	g_pd3dDevice = m_pd3dDevice.Get();
	g_pImmediateContext = m_pd3dContext.Get();
	return hr;
}

HRESULT myDevice::CreateSwapChain()
{
	//����ü�� ��������
	DXGI_SWAP_CHAIN_DESC pSwapChainDesc;
	ZeroMemory(&pSwapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
	//���� ������
	pSwapChainDesc.BufferDesc.Width = g_rtClient.right;
	pSwapChainDesc.BufferDesc.Height = g_rtClient.bottom;
	//���� Ÿ�� = RGBA 32��Ʈ�� ����ϰ� �븻������ ���� �ʴ´�
	pSwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	//������
	pSwapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	pSwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	pSwapChainDesc.SampleDesc.Count = 1;
	//���� Ÿ�� ����
	pSwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	//���� ����
	pSwapChainDesc.BufferCount = 1;
	//�ѷ��� ������ �ڵ�
	pSwapChainDesc.OutputWindow = g_hWnd;
	//â��� ����
	pSwapChainDesc.Windowed = true;
	//pSwapChainDesc.SwapEffect;
	//�÷��� �� ���
	pSwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	//����ü�� ����
	HRESULT hr = m_pGIFactory->CreateSwapChain(
		m_pd3dDevice.Get(),
		&pSwapChainDesc,
		m_pSwapChain.GetAddressOf());
	return hr;
}

HRESULT myDevice::SetRenderTargetView()
{

	//����۸� ����ش�
	ComPtr<ID3D11Texture2D> pBackBuffer = nullptr;
	//����ü�ο��� ����۸� �����´�
	m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D),
		(LPVOID*)pBackBuffer.GetAddressOf());
	//���� Ÿ���� ����۸� ����ؼ� ������ش�
	HRESULT hr = m_pd3dDevice->CreateRenderTargetView(pBackBuffer.Get(), NULL,
		m_pRenderTargetView.GetAddressOf());

	//����۸� ����� �� ������ ���ش�
	return hr;
}

HRESULT myDevice::SetDepthStencilView()
{
	// create depth texture
	ComPtr<ID3D11Texture2D> pTexture = nullptr;
	D3D11_TEXTURE2D_DESC texDesc;
	ZeroMemory(&texDesc, sizeof(D3D11_TEXTURE2D_DESC));
	texDesc.Width = g_rtClient.right;
	texDesc.Height = g_rtClient.bottom;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	HRESULT hr = m_pd3dDevice->CreateTexture2D(&texDesc, NULL, pTexture.GetAddressOf());
	if (FAILED(hr))
	{
		return E_FAIL;
	}
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	ZeroMemory(&dsvDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Texture2D.MipSlice = 0;
	hr = m_pd3dDevice->CreateDepthStencilView(
		pTexture.Get(),
		&dsvDesc,
		m_pDSV.GetAddressOf());
	if (FAILED(hr))
	{
		return E_FAIL;
	}
	return S_OK;
}

bool myDevice::SetViewport()
{
	//����Ʈ ����
	m_Viewport.TopLeftX = 0;
	m_Viewport.TopLeftY = 0;
	m_Viewport.Width = g_rtClient.right;
	m_Viewport.Height = g_rtClient.bottom;
	m_Viewport.MinDepth = 0.0f;
	m_Viewport.MaxDepth = 1.0f;
	return true;
}

bool myDevice::Init()
{
	if (FAILED(CreateDevice()))
	{
		return false;
	}
	if (FAILED(CreateGIFactory()))
	{
		return false;
	}
	if (FAILED(CreateSwapChain()))
	{
		return false;
	}
	if (FAILED(SetRenderTargetView()))
	{
		return false;
	}
	if (FAILED(SetDepthStencilView()))
	{
		return false;
	}
	if (SetViewport() == false)
	{
		return false;
	}
	myDxState::SetState(g_pd3dDevice);
	if (FAILED(m_pGIFactory->MakeWindowAssociation(m_hWnd,
		DXGI_MWA_NO_WINDOW_CHANGES |
		DXGI_MWA_NO_ALT_ENTER)))
	{
		return false;
	}
	return true;
}

bool myDevice::Frame()
{
	//if (m_pd3dContext.Get())
	//{
	//	m_pd3dContext->RSSetViewports(1, &m_Viewport);
	//	m_pd3dContext->OMSetRenderTargets(1, m_pRenderTargetView.GetAddressOf(), m_pDSV.Get());
	//	float clearColor[] = { 0,0,0,1 };
	//	m_pd3dContext->ClearRenderTargetView(m_pRenderTargetView.Get(), clearColor);
	//	m_pd3dContext->ClearDepthStencilView(m_pDSV.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
	//}
	return true;
}

bool myDevice::PreRender()
{
	if (m_pd3dContext.Get())
	{
		m_pd3dContext->RSSetViewports(1, &m_Viewport);
		//������ ���������� �ƿ�ǲ ���տ� ����Ÿ���� �������ش�
		m_pd3dContext->OMSetRenderTargets(1, m_pRenderTargetView.GetAddressOf(), m_pDSV.Get());
		float clearColor[] = { 0.3f,0.3f,0.3f,1 };
		//����Ÿ���� �ʱ�ȭ���ش�
		m_pd3dContext->ClearRenderTargetView(m_pRenderTargetView.Get(), clearColor);
		m_pd3dContext->ClearDepthStencilView(m_pDSV.Get(), D3D10_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
		//ȭ�� ��ǥ�� ��ȯ���ش�

		//IA�� �׷��� Ÿ�� ����
		m_pd3dContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	}
	return true;
}

bool myDevice::Render()
{
	return true;
}

bool myDevice::PostRender()
{
	//����ۿ� ����Ʈ ���۸� �������ش�
	if (m_pSwapChain.Get())
	{
		m_pSwapChain->Present(0, 0);
	}
	return true;
}

bool myDevice::Release()
{
	myDxState::Release();
	//m_pDSV->Release();
	//m_pRenderTargetView->Release();
	//m_pSwapChain->Release();
	//m_pd3dContext->Release();
	//m_pd3dDevice.Get()->Release();
	//m_pGIFactory->Release();
	return true;
}

void myDevice::SetMode(bool bFullScreen)
{
	//���� ������ ��Ŀ���� ���� �ʰ� �ؾ���
	m_bFullScreen = bFullScreen;
	//Ǯ��ũ�� ��ȯ
	m_pSwapChain->SetFullscreenState(m_bFullScreen, NULL);
	//Ǯ��ũ���� �ƴϸ� ������ ����
	if (m_bFullScreen == FALSE)
	{
		ShowWindow(m_hWnd, SW_SHOW);
	}
}

void myDevice::ResizeDevice(UINT w, UINT h)
{
	//����̽��� �����Ǿ����� ������ ����
	if (m_pd3dDevice.Get() == NULL)  return;
	//���� ���ҽ��� ����
	DeleteDXResource();
	HRESULT hr = S_OK;
	//����Ÿ�� �ʱ�ȭ
	m_pd3dContext->OMSetRenderTargets(0, NULL, NULL);
	//����Ÿ�� ������

	if (m_pRenderTargetView.Get()) m_pRenderTargetView->Release();
	if (m_pDSV.Get()) m_pDSV->Release();

	//����ü�� ���� ��������
	m_pSwapChain->GetDesc(&m_pSwapChainDesc);
	//���� ������ ����
	if (FAILED(m_pSwapChain->ResizeBuffers(
		m_pSwapChainDesc.BufferCount,
		w,
		h,
		m_pSwapChainDesc.BufferDesc.Format,
		m_pSwapChainDesc.Flags)))
	{

	}
	m_pSwapChain->GetDesc(&m_pSwapChainDesc);
	m_rtClient.right = m_pSwapChainDesc.BufferDesc.Width;
	m_rtClient.bottom = m_pSwapChainDesc.BufferDesc.Height;
	g_rtClient = m_rtClient;
	//����Ÿ�� ����
	SetRenderTargetView();
	SetDepthStencilView();
	//����Ʈ ����
	SetViewport();
	//���ҽ� ����
	CreateDXResource(w, h);
}

HRESULT myDevice::DeleteDXResource()
{
	return S_OK;
}

HRESULT myDevice::CreateDXResource(UINT w, UINT h)
{
	return S_OK;
}

myDevice::myDevice()
{
	m_pGIFactory = nullptr;
	m_pd3dDevice = nullptr;
	m_pd3dContext = nullptr;
	m_pSwapChain = nullptr;
	m_pRenderTargetView = nullptr;
}

myDevice::~myDevice()
{
}
