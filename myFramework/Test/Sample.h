#pragma once
#include "myCore.h"
class Sample : public myCore
{
public:
	virtual bool Init();
	virtual bool Frame();
	virtual bool Render();
	virtual bool Release();

	Sample();
};

