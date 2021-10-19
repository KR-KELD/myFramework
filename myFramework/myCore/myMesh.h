#pragma once
#include "myBaseStruct.h"
#include "myTextureManager.h"
#include "myComponent.h"

struct mySubMesh
{
	vector<myTriangle>		m_TriangleList;
	//pnct2
	vector<PNCT_VERTEX>		m_VertexList;
	vector<IW_VERTEX>		m_VertexListIW;
	vector<DWORD>			m_IndexList;
	ComPtr<ID3D11Buffer>	m_pVertexBuffer;
	ComPtr<ID3D11Buffer>	m_pVertexBufferIW;
	ComPtr<ID3D11Buffer>	m_pIndexBuffer;
	myTexture* m_pTexture;
	int						m_iFaceCount;
	UINT					m_iNumVertex;
	UINT					m_iNumIndex;
	void	SetUniqueBuffer(myTriangle& tri)
	{
		//비어있는 인덱스 버퍼와 버텍스 버퍼를 만들어놓고
		//트라이앵글 리스트를 돌리면서
		//버텍스 리스트에 들어있으면 인덱스리스트에만 푸시
		//버텍스 리스트에 들어있지않으면
		//버텍스,인덱스리스트 모두 푸시
		for (int iVertex = 0; iVertex < 3; iVertex++)
		{
			bool bAdd = true;
			int iPosReturn = -1;
			for (int iIndex = 0; iIndex < m_VertexList.size(); iIndex++)
			{
				if (m_VertexList[iIndex] == tri.vVertex[iVertex])
				{
					m_IndexList.push_back(iIndex);
					bAdd = false;
					break;
				}
			}
			if (bAdd)
			{
				m_VertexList.push_back(tri.vVertex[iVertex]);
				m_VertexListIW.push_back(tri.vVertexIW[iVertex]);
				m_IndexList.push_back(m_VertexList.size() - 1);
			}
		}
		m_iFaceCount++;
	}
	mySubMesh()
	{
		m_iFaceCount = 0;
		m_iNumVertex = 0;
		m_iNumIndex = 0;
	}
};

struct mySubMesh2 : public mySubMesh
{
	vector<myTriangle2>		m_TriangleList2;
	vector<PNCT2_VERTEX>	m_VertexList2;
	void	SetUniqueBuffer(myTriangle2& tri)
	{
		for (int iVertex = 0; iVertex < 3; iVertex++)
		{
			bool bAdd = true;
			int iPosReturn = -1;
			for (int iIndex = 0; iIndex < m_VertexList2.size(); iIndex++)
			{
				if (m_VertexList2[iIndex] == tri.vVertex[iVertex])
				{
					m_IndexList.push_back(iIndex);
					m_iNumIndex++;
					bAdd = false;
					break;
				}
			}
			if (bAdd)
			{
				m_VertexList2.push_back(tri.vVertex[iVertex]);
				m_VertexListIW.push_back(tri.vVertexIW[iVertex]);
				m_IndexList.push_back(m_VertexList2.size() - 1);
				m_iNumIndex++;
				m_iNumVertex++;
			}
		}
		m_iFaceCount++;
	}
	mySubMesh2()
	{
		mySubMesh::mySubMesh();
	}
};

class myMesh : public myComponent
{
public:
	DEFINE_COMPONENT(myMesh, myComponent, true)
public:
	ID3DBlob* m_pVSObj;
	UINT						m_iTopology;
	C_STR						m_szVertexShader;
	C_STR						m_szPixelShader;
public:
	Vector3						m_vMin;
	Vector3						m_vMax;
	Vector3						m_vCenter;
public:
	UINT						m_iNumVertex;
	UINT						m_iVertexSize;
	UINT						m_iNumIndex;
public:
	vector<wstring>		m_MaterialList;
	myDataCB					m_cbData;
	ComPtr<ID3D11Buffer>		m_pVertexBuffer;
	ComPtr<ID3D11Buffer>		m_pIndexBuffer;
	ComPtr<ID3D11Buffer>		m_pConstantBuffer;
	ComPtr<ID3D11InputLayout>	m_pInputLayout;
	ComPtr<ID3D11VertexShader>	m_pVertexShader;
	ComPtr<ID3D11PixelShader>	m_pPixelShader;
public:
	virtual bool	Init();
	virtual bool	Frame();
	virtual void    Update(ID3D11DeviceContext* pd3dContext);
	virtual bool	PreRender(ID3D11DeviceContext* pd3dContext);
	virtual bool	Render(ID3D11DeviceContext* pd3dContext);
	virtual bool	PostRender(ID3D11DeviceContext* pd3dContext);
	virtual bool	Release();
	virtual bool	SettingPipeLine(ID3D11DeviceContext* pd3dContext);
	//임시
	virtual bool	MultiDraw(ID3D11DeviceContext* pd3dContext);
	virtual bool	Draw(ID3D11DeviceContext* pd3dContext);
	virtual bool    CreateVertexData(Vector3 vCenter, float fRange);
	virtual bool    CreateVertexData();

	virtual bool    CreateIndexData();
	virtual bool    CreateVertexBuffer();
	virtual bool    CreateIndexBuffer();
	virtual bool    CreateConstantBuffer();
	virtual bool	LoadShader(T_STR szVS, T_STR szPS);
	virtual bool	CreateInputLayout();
	virtual bool	LoadTexture(T_STR szTex);
	virtual bool	Create(T_STR szVS, T_STR szPS, T_STR	szTex);
	void			CompilerCheck(ID3DBlob* pErrorMsgs);
public:
	myMesh();
	virtual ~myMesh();
};

