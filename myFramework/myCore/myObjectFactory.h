#pragma once
#include "myGameObject.h"
#include "myComponent.h"

class myObjectFactory : public mySingleTon<myObjectFactory>
{
public:
	friend  class mySingleTon<myObjectFactory>;
public:
	template <class Component_T>
	Component_T* NewComponent()
	{
		Component_T* newComp = new Component_T(Component_T::baseComponent);
		return newComp;
	}
	template <class Component_T>
	Component_T* CloneComponent(Component_T* pOwner)
	{
		Component_T* newComp = new Component_T(*pOwner);
		return newComp;
	}
	myGameObject* NewGameObject(wstring strName = L"myGameObject")
	{
		myGameObject* newObj = new myGameObject(strName.c_str());
		return newObj;
	}
	myGameObject* CloneGameObject(myGameObject* pOwner)
	{
		myGameObject* newObj = new myGameObject(*pOwner);
		for (pOwner->m_ComIter = pOwner->m_ComponentList.begin();
			pOwner->m_ComIter != pOwner->m_ComponentList.end();
			pOwner->m_ComIter++)
		{
			newObj->InsertComponent(CloneComponent((*pOwner->m_ComIter).second));
		}
		for (pOwner->m_ObjIter = pOwner->m_Childs.begin();
			pOwner->m_ObjIter != pOwner->m_Childs.end();
			pOwner->m_ObjIter++)
		{
			newObj->AddChild(CloneGameObject((*pOwner->m_ObjIter).second));
		}
		return newObj;
	}
};
#define g_ObjFactory myObjectFactory::GetInstance()