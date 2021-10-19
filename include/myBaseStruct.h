#pragma once
#include "myStd.h"

struct P_VERTEX
{
	Vector3 p;
	Vector3 n;
	Vector4 c;
	Vector2 t;
	P_VERTEX() {}
	P_VERTEX(
		Vector3 p,
		Vector3 n,
		Vector4 c,
		Vector2 t)
	{
		this->p = p;
		this->n = n;
		this->c = c;
		this->t = t;
	}
};

struct PNCT_VERTEX
{
	Vector3 p;
	Vector3 n;
	Vector4 c;
	Vector2 t;
	bool operator == (const PNCT_VERTEX& Vertex)
	{
		if (p == Vertex.p && n == Vertex.n && c == Vertex.c && t == Vertex.t)
		{
			return true;
		}
		return  false;
	}
	PNCT_VERTEX() {}
	PNCT_VERTEX(
		Vector3 vp,
		Vector3 vn,
		Vector4 vc,
		Vector2 vt)
	{
		p = vp, n = vn, c = vc, t = vt;
	}
};

struct PNCT2_VERTEX
{
	Vector3 p;
	Vector3 n;
	Vector4 c;
	Vector2 t;
	Vector3 tangent;
	bool operator == (const PNCT2_VERTEX& Vertex)
	{
		if (p == Vertex.p && n == Vertex.n && c == Vertex.c && t == Vertex.t && tangent == Vertex.tangent)
		{
			return true;
		}
		return  false;
	}
	PNCT2_VERTEX() {}
	PNCT2_VERTEX(
		Vector3 vp,
		Vector3 vn,
		Vector4 vc,
		Vector2 vt,
		Vector3 vTangent)
	{
		p = vp, n = vn, c = vc, t = vt, tangent = vTangent;
	}
};

struct IW_VERTEX
{
	float i1[4];
	float w1[4];
	IW_VERTEX()
	{
		i1[0] = i1[1] = i1[2] = i1[3] = 0.0f;
		w1[0] = w1[1] = w1[2] = w1[3] = 0.0f;
	}
};

struct PNCTIW_VERTEX
{
	Vector3 p;
	Vector3 n;
	Vector4 c;
	Vector2 t;
	Vector4 i;
	Vector4	w;
	bool operator == (const PNCTIW_VERTEX& Vertex)
	{
		if (p == Vertex.p && n == Vertex.n && c == Vertex.c && t == Vertex.t
			&& i == Vertex.i && w == Vertex.w)
		{
			return true;
		}
		return  false;
	}
	PNCTIW_VERTEX() {}
	PNCTIW_VERTEX(
		Vector3 vp,
		Vector3 vn,
		Vector4 vc,
		Vector2 vt,
		Vector4 vi,
		Vector4 vw)
	{
		p = vp, n = vn, c = vc, t = vt, i = vi, w = vw;
	}
};

struct myTriangle
{
	//pnct2로 변환
	PNCT_VERTEX vVertex[3];
	IW_VERTEX vVertexIW[3];
	Vector3		vNormal;
	int			iSubIndex;
	myTriangle(int iIndex) : iSubIndex(iIndex) {}
	myTriangle() : iSubIndex(-1) {}
};

struct myTriangle2
{
	PNCT2_VERTEX vVertex[3];
	IW_VERTEX	 vVertexIW[3];
	Vector3		 vNormal;
	int			 iSubIndex;
	myTriangle2(int iIndex) : iSubIndex(iIndex) {}
	myTriangle2() : iSubIndex(-1) {}
};

struct myDataCB
{
	Matrix  matWorld;
	Matrix  matView;
	Matrix  matProject;
	float vColor[4];
	float vTime[4];
};

enum myObjectType
{
	OBJECT_OBJECT = 0,
	OBJECT_PLAYER,
	OBJECT_ENEMY,
	OBJECT_MAP,
	OBJECT_UI,

};

enum myCollisionState
{
	COLLISION_NONE = 0,
	COLLISION_BEGIN,
	COLLISION_STAY,
	COLLISION_END,

};

enum myOverlapState
{
	OVERLAP_DEFAULT = 0,
	OVERLAP_HOVER,
	OVERLAP_FOCUS,
	OVERLAP_ACTIVE,
	OVERLAP_SELECTED,

};

struct MY_SPHERE
{
	Vector3		vCenter;
	float		fRadius;
};

struct MY_BOX
{
	// Common
	Vector3		vCenter;
	Vector3		vPos[8];
	// AABB
	Vector3		vMax;
	Vector3		vMin;
	// OBB
	Vector3		vAxis[3];
	float		fExtent[3];
};

struct MY_PLANE
{
	//평면의 방정식
	//a,b,c는 평면의 노말벡터 x,y,z
	//d는 원점과 평면 사이의 거리
	float a, b, c, d;
	//점 3개로 평면을 구성하는 방법
	void Create(Vector3 v0, Vector3 v1, Vector3 v2)
	{
		Vector3 n;
		Vector3 e0 = v1 - v0;
		Vector3 e1 = v2 - v0;
		n = e0.Cross(e1);
		n.Normalize();
		a = n.x;
		b = n.y;
		c = n.z;
		d = -n.Dot(v0);
	}
	//노말벡터 1개와 평면의 점 1개
	void Create(Vector3 n, Vector3 v)
	{
		n.Normalize();
		a = n.x;
		b = n.y;
		c = n.z;
		d = -n.Dot(v);
	}
};

struct MY_RAY
{
	Vector3 vOrigin;
	Vector3 vDir;
	MY_RAY()
	{
		vOrigin = Vector3::Zero;
		vDir = Vector3(0.0f, 0.0f, 1.0f);
	}
};