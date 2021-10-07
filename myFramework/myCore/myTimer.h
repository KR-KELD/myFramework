#pragma once
#include "myStd.h"

class myStopwatch
{
public:
	myStopwatch() { QueryPerformanceFrequency(&m_liPerFreq); Start(); }
	void Start() { QueryPerformanceCounter(&m_liPerfStart); }

	__int64 Now()
	{    // Start �� ȣ��� �� ����� �и��ʸ� ����
		LARGE_INTEGER liPerfNow;
		QueryPerformanceCounter(&liPerfNow);
		__int64 qwElapsedTime = (((liPerfNow.QuadPart - m_liPerfStart.QuadPart)));// / m_liPerFreq.QuadPart);
		return qwElapsedTime;
	}
	void Output(const TCHAR* pData)
	{
#ifdef _DEBUG
		TCHAR  strMessage[MAX_PATH] = { 0, };
		__int64 qwElapsedTime = Now();
		float Second = static_cast<float>(qwElapsedTime) / static_cast<float>(m_liPerFreq.QuadPart);
		_stprintf_s(strMessage, _T("<=====%s[%10.4f]====>\n"), pData, Second);
		OutputDebugString(strMessage);
#endif
	}
private:
	LARGE_INTEGER m_liPerFreq;    // �ʴ� ���
	LARGE_INTEGER m_liPerfStart;   // ���� ���
};

class myTimer
{
private:
	LARGE_INTEGER m_Frequency;// ���ļ�
	LARGE_INTEGER m_Current;
	LARGE_INTEGER m_Frame;
	int			  m_iTmpCounter;
public:
	//���� ���� �� �帥 ��ü�ð�
	float	m_fGameTimer;
	//�����Ӵ� �ɸ��� �ð�
	float	m_fSecondPerFrame;
	//�ʴ� ���ư��� ������
	int		m_iFPS;

	WCHAR	m_szBuffer[MAX_PATH] = { 0, };
public:

	bool	Init();
	bool	Frame();
	bool	Render();
	bool	Release();
	bool	Reset();
public:
	myTimer();
	virtual ~myTimer();
};
