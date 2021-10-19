#include "myGameObject.h"
#include "myObjectFactory.h"

bool myGameObject::Init()
{
    return false;
}

bool myGameObject::PreFrame()
{
    return false;
}

bool myGameObject::Frame()
{
    return false;
}

bool myGameObject::PostFrame()
{
    return false;
}

bool myGameObject::PreRender(ID3D11DeviceContext* pd3dContext)
{
    return false;
}

bool myGameObject::Render(ID3D11DeviceContext* pd3dContext)
{
    return false;
}

bool myGameObject::PostRender(ID3D11DeviceContext* pd3dContext)
{
    return false;
}

void myGameObject::Update(ID3D11DeviceContext* pd3dContext)
{
}

myGameObject* myGameObject::AddChild(wstring strName)
{
	myGameObject* obj = nullptr;
	if (strName.empty())
	{
		obj = g_ObjFactory.NewGameObject();
		obj->SetParent(this);
		m_Childs.insert(make_pair(obj->m_strName, obj));
		return obj;
	}
	obj = g_ObjFactory.NewGameObject(strName);
	obj->SetParent(this);
	m_Childs.insert(make_pair(strName, obj));
	return obj;
}

myGameObject* myGameObject::AddChild(myGameObject* pGameObject)
{
	if (pGameObject == nullptr) return nullptr;

	pGameObject->SetParent(this);
	m_Childs.insert(make_pair(pGameObject->m_strName, pGameObject));
	return pGameObject;
}

myGameObject* myGameObject::GetGameObject(wstring strName)
{
	m_ObjIter = m_Childs.find(strName);
	myGameObject* pObj = nullptr;
	if (m_ObjIter != m_Childs.end())
	{
		return (*m_ObjIter).second;
	}

	for (m_ObjIter = m_Childs.begin();
		m_ObjIter != m_Childs.end();
		m_ObjIter++)
	{
		pObj = GetGameObject(strName);
		if (pObj != nullptr)
		{
			return pObj;
		}
	}
	return nullptr;
}

myGameObject::myGameObject(myGameObject& origin)
{
	m_strName = origin.m_strName + L"(Clone)";
	m_ComponentList.clear();
	m_Childs.clear();
	m_pParent = nullptr;
}

//list<myGameObject*>* myGameObject::GetGameObjects(wstring strName)
//{
//	list<myGameObject*>* result = new list<myGameObject*>;
//	for (m_ObjIter = m_Childs.lower_bound(strName);
//		m_ObjIter != m_Childs.upper_bound(strName); m_ObjIter++)
//	{
//		result->push_back(m_ObjIter->second);
//	}
//	return result;
//}
