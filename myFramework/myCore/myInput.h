#pragma once
#include "myStd.h"

enum KeyState
{
	KEY_FREE = 0,
	KEY_PUSH,
	KEY_UP,
	KEY_HOLD
};

class myInput : public mySingleTon<myInput>
{
	friend class mySingleTon<myInput>;
private:
	DWORD		m_dwKeyState[256];
	POINT		m_MousePos;
	bool		m_bEnable;
public:
	bool		Init();
	bool		Frame();
	bool		Render();
	bool		Release();
	bool		Reset();
public:

	DWORD		GetKey(DWORD dwKey);
	POINT& GetMouse();
	void		SetEnable(bool bEnable = true);
	bool		GetEnable();
private:
	myInput();
public:
	~myInput();

};
#define g_Input myInput::GetInstance()

