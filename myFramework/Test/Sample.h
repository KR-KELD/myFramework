#pragma once
#include "myCore.h"
#include "myComponent.h"
class Sample : public myCore
{
public:
	myComponent* Comp1;
public:
	virtual bool Init();
	virtual bool Frame();
	virtual bool Render();
	virtual bool Release();

	template <class Component_T>
	Component_T* CloneComponent()
	{
		size_t aa = Get_Component<Component_T>::type::GetComponentId();
		Component_T* newObj = nullptr;

		return newObj;
	}
	Sample();
};

