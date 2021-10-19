#pragma once
#include "myBaseStruct.h"
#include "myTextureManager.h"

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

class myMesh
{
};

