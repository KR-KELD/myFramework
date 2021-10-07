#pragma once
#include "myDevice.h"
#include "myTimer.h"
#include "myInput.h"
#include "myWrite.h"
#include "mySoundManager.h"
class myCore : public myDevice
{
public:
	myTimer				m_Timer;
	bool				m_isGameRun;
	bool				m_isDebugText;
public:
	virtual bool		Init() { return true; }
	virtual bool		PreInit() { return true; }
	virtual bool		PostInit() { return true; }
	virtual bool		Frame() { return true; }
	virtual bool		PreFrame() { return true; }
	virtual bool		PostFrame() { return true; }
	virtual bool		Render() { return true; }
	virtual bool		PreRender();
	virtual bool		PostRender();
	virtual bool		Release() { return true; }
	virtual HRESULT		DeleteDXResource();
	virtual HRESULT		CreateDXResource(UINT w, UINT h);
private:
	bool				GameInit();
	bool				GameFrame();
	bool				GameRender();
	bool				GameRun();
	bool				GameRelease();
public:
	bool				InitTool(HWND hWnd, HINSTANCE hInstance);
	bool				ReleaseTool();
	bool				RunTool();
public:
	bool				Run();
public:
	myCore();
	virtual ~myCore();
};



//_CrtSetBreakAlloc(56);

//_CrtDumpMemoryLeaks();