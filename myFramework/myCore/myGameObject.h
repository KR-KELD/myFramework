#pragma once
#include "myComponent.h"

class myGameObject : public myComponent
{
public:
	T_STR			m_strName;
	T_STR			m_strTag;
	int				m_iObjectID;
public:
	myGameObject*									m_pParent;
	multimap<wstring, myGameObject*>				m_Childs;
	multimap<wstring, myGameObject*>::iterator		m_ObjIter;
	unordered_map<size_t, myComponent*>				m_ComponentList;
	unordered_map<size_t, myComponent*>::iterator	m_ComIter;
};

