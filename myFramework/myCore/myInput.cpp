#include "myInput.h"
myKeyMap g_KeyMap = { 0, };

myInput::myInput()
{
	m_MousePos = { 0,0 };
	m_bEnable = true;
	ZeroMemory(&m_dwKeyState, sizeof(DWORD) * 256);
	ZeroMemory(&g_KeyMap, sizeof(myKeyMap));
}

myInput::~myInput()
{
}

bool myInput::Init()
{
	Reset();
	return true;
}

bool myInput::Frame()
{
	//if (g_bActive == false) return true;
	//Ŀ���� ��ġ�� �޾ƿ��� �Լ�
	GetCursorPos(&m_MousePos);
	//ȭ�� ��ǥ�� Ŭ���̾�Ʈ ��ǥ�� ��ȯ���ִ� �Լ�
	//HWND hWnd = WindowFromPoint(m_MousePos);
	ScreenToClient(g_hWnd, &m_MousePos);
	//if (hWnd != g_hWnd) return true;

	if (m_bEnable == false) return true;
	for (int iKey = 0; iKey < 256; iKey++)
	{
		//Ű�� �Է��� �񵿱�� �޾ƿ��� �Լ�(�Է��� ������ ����)
		SHORT sKey = GetAsyncKeyState(iKey);
		if (sKey & 0x8000)
		{
			if (m_dwKeyState[iKey] == KEY_FREE)
			{
				m_dwKeyState[iKey] = KEY_PUSH;
			}
			else
			{
				m_dwKeyState[iKey] = KEY_HOLD;
			}
		}
		else
		{
			if (m_dwKeyState[iKey] == KEY_PUSH ||
				m_dwKeyState[iKey] == KEY_HOLD)
			{
				m_dwKeyState[iKey] = KEY_UP;
			}
			else
			{
				m_dwKeyState[iKey] = KEY_FREE;
			}
		}
	}
	g_KeyMap.bCapsLockActive = false;
	//���� Ű�� ���¸� �޾ƿ��� �Լ�
	SHORT dwKey = GetKeyState(VK_CAPITAL);
	//���� Ű�� Ȱ��ȭ ���¸� üũ (0x8000�� ������ �� 0x01�� ������ ����)
	if (dwKey & 0x01)
		g_KeyMap.bCapsLockActive = true;
	return true;
}

bool myInput::Render()
{
	return true;
}

bool myInput::Release()
{
	return true;
}

bool myInput::Reset()
{
	ZeroMemory(&m_dwKeyState, sizeof(DWORD) * 256);
	ZeroMemory(&g_KeyMap, sizeof(myKeyMap));
	return true;
}

DWORD myInput::GetKey(DWORD dwKey)
{
	return m_dwKeyState[dwKey];
}

POINT& myInput::GetMouse()
{
	return m_MousePos;
}

void myInput::SetEnable(bool bEnable)
{
	m_bEnable = bEnable;
	if (m_bEnable == false)
	{
		ZeroMemory(&m_dwKeyState, sizeof(DWORD) * 256);
		ZeroMemory(&g_KeyMap, sizeof(myKeyMap));
	}
}

bool myInput::GetEnable()
{
	return m_bEnable;
}

