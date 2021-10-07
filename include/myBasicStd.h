#pragma once
//#include <winsock2.h>
#include <Windows.h>
#include <tchar.h>
#include <list>
#include <map>
#include <unordered_map>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <fstream>
#include <assert.h>
#include <conio.h>
#include <stdio.h>
#include <memory>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <crtdbg.h>

#include <wrl.h>
#include "myUtil.h"
#include "myMath.h"

#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "winmm.lib")

#if NDEBUG
#pragma comment (lib, "myCore_R.lib")
#else
#pragma comment (lib, "myCore_D.lib")
#endif

#pragma comment(lib, "fmod_vc.lib")
#pragma comment(lib, "Msimg32.lib")

using namespace std;

using namespace Microsoft::WRL;

template <class T> class mySingleTon
{
public:
	static T& GetInstance()
	{
		static T Single;
		return Single;
	}
};

#ifndef SAFE_ZERO
#define SAFE_ZERO(A)				{ A = 0; }
#endif

#ifndef SAFE_NEW
#define SAFE_NEW(A, B)				{ if (!A) A = new B; }
#endif 

#ifndef SAFE_DELETE
#define SAFE_DELETE(A)					{ if (A) delete A; (A)=NULL; }
#endif 

#ifndef SAFE_NEW_ARRAY
#define SAFE_NEW_ARRAY(A, B, C)		{ if (!A && C) A = new B[C]; }
#endif 

#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(A)		{ if (A) delete [] A; (A)=NULL; }
#endif 

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(A)				{ if(A) { (A)->Release(); (A)=NULL; } }
#endif 

#ifndef SAFE_NEW_CLEAR
#define SAFE_NEW_CLEAR( A, B )			{ if (!A) A = new B; if(A) memset( A, 0, sizeof(B) ); };
#endif 

#ifndef SAFE_NEW_ARRAY_CLEAR
#define SAFE_NEW_ARRAY_CLEAR( A, B, C )	{ if (!A && C) A = new B[C]; if(A) memset( A, 0, sizeof(B)*C ); };
#endif