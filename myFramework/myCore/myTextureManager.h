#pragma once
#include "myTexture.h"
class myTextureManager : public mySingleTon<myTextureManager>
{
public:
	friend  class mySingleTon<myTextureManager>;
private:
	std::map<wstring, myTexture*>				m_List;
	std::map<wstring, myTexture*>::iterator		m_iter;
	std::wstring								m_szDefaultPath;
public:
	myTexture* m_pWhiteTexture;
public:
	myTexture* Load(const TCHAR* filename);
	myTexture* GetPtr(wstring filename);
	bool		Init();
	bool		Frame();
	bool		Render();
	bool		Release();
private:
	myTextureManager();
public:
	~myTextureManager();
};
#define g_TextureMgr myTextureManager::GetInstance()
