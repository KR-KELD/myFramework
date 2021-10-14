#pragma once
#include "myStd.h"

struct null_t {};

template <bool Test, class True_Meta_Func, class False_Meta_Func>
struct _If
{

};

template <class True_Meta_Func, class False_Meta_Func>
struct _If<true, True_Meta_Func, False_Meta_Func>
{
	typedef typename True_Meta_Func type;
};

template <class True_Meta_Func, class False_Meta_Func>
struct _If<false, True_Meta_Func, False_Meta_Func>
{
	typedef typename False_Meta_Func type;
};

template <class Component_T>
struct Is_Unique_Component
{

	typedef typename Component_T::Component_Identifier_T			Identifier_T;
	typedef typename Identifier_T::Parent_Component_T				Parent_T;

	enum
	{
		value = _If<Identifier_T::is_unique, std::true_type, Is_Unique_Component<Parent_T>>::type::value
	};
};

template <>
struct Is_Unique_Component<null_t>
{
	enum { value = false };
};

template <class Component_T>
struct Get_Unique_Component
{
	typedef typename Component_T::Component_Identifier_T	Identifier_T;
	typedef typename Identifier_T::Parent_Component_T		Parent_T;

	typedef typename _If<Identifier_T::is_unique, Component_T, typename Get_Unique_Component<Parent_T>::type>::type type;
};

template <>
struct Get_Unique_Component<null_t>
{
	typedef null_t type;
};

template <class Component_T>
struct Get_Component
{
	typedef typename _If<
		Is_Unique_Component<Component_T>::value,
		typename Get_Unique_Component<Component_T>::type,
		Component_T>::type type;
};

template <class Component_T, class Parent_Component_T, bool _is_unique>
struct Component_Identifier
{
	typedef Component_T Current_Component_T;
	typedef Parent_Component_T Parent_Component_T;

	enum { is_unique = _is_unique };
};

class myGameObject;
class myTransform;
class myComponent;


class myComponentFactory : public mySingleTon<myComponentFactory>
{
public:
	friend  class mySingleTon<myComponentFactory>;
public:
	template <class Component_T>
	Component_T* NewComponent()
	{
		Component_T* newComp = new Component_T(Component_T::baseComponent);
		return newComp;
	}
};
#define g_CompFactory myComponentFactory::GetInstance()

#define DEFINE_COMPONENT(component_name, parent_component_name, unique_component) \
	public: \
		typedef Component_Identifier<component_name, parent_component_name, unique_component> Component_Identifier_T; \
		static size_t GetComponentId(void) { return reinterpret_cast<size_t>(&identifier); } \
		virtual const char* GetComponentName_(void) { return #component_name; } \
		static const char* GetComponentName(void) { return #component_name; } \
	private: \
		static Component_Identifier_T identifier; \
		static const component_name baseComponent; \
		friend class myComponentFactory; \

#define DECLARE_COMPONENT(component_name) \
	component_name::Component_Identifier_T component_name::identifier; \
	const component_name component_name::baseComponent; \

class myComponent
{
public:
	bool			m_isActive;
	DEFINE_COMPONENT(myComponent, null_t, false)
public:
	myComponent()
	{
		m_isActive = true;
	}
	virtual ~myComponent() {}
};
