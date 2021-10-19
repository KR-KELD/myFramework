#pragma once
#include "myTransform.h"

class myGameObject
{
	friend class myObjectFactory;
public:
	T_STR			m_strName;
	T_STR			m_strTag;
	int				m_iObjectID;
private:
	myGameObject*									m_pParent;
	multimap<wstring, myGameObject*>				m_Childs;
	multimap<wstring, myGameObject*>::iterator		m_ObjIter;
	unordered_map<size_t, myComponent*>				m_ComponentList;
	unordered_map<size_t, myComponent*>::iterator	m_ComIter;
public:
	virtual bool	Init();
	virtual bool	PreFrame();
	virtual bool	Frame();
	virtual bool	PostFrame();
	virtual bool	PreRender(ID3D11DeviceContext* pd3dContext);
	virtual bool	Render(ID3D11DeviceContext* pd3dContext);
	virtual bool	PostRender(ID3D11DeviceContext* pd3dContext);
	virtual void	Update(ID3D11DeviceContext* pd3dContext);
public:
	void SetParent(myGameObject* pParent)
	{
		m_pParent = pParent;
	}
	void SetName(const TCHAR* szName)
	{
		m_strName = szName;
	}
	void SetTag(const TCHAR* szTag)
	{
		m_strTag = szTag;
	}
	myGameObject* AddChild(wstring strName = L"");
	myGameObject* AddChild(myGameObject* pGameObject);
	myGameObject* GetGameObject(wstring strName);
	//list<myGameObject*>* GetGameObjects(wstring strName);
public:
	template <class Component_T>
	void InsertComponent(Component_T* pComponent)
	{
		size_t CompID = Get_Component<Component_T>::type::GetComponentId();
		pComponent->Combine(this);
		this->m_ComponentList[CompID] = pComponent;
	}
	template <class Component_T>
	Component_T* GetComponent()
	{
		size_t CompID = Get_Component<Component_T>::type::GetComponentId();
		if (this->m_ComponentList.find(CompID) == this->m_ComponentList.end())
			return nullptr;
		return reinterpret_cast<Component_T*>(this->m_ComponentList[CompID]);
	}

	myGameObject()
	{
		m_pParent = nullptr;
	}
	myGameObject(myGameObject& origin);

	myGameObject(const TCHAR* szName)
	{
		m_pParent = nullptr;
		m_strName = szName;
	}
	~myGameObject() {}
};

