#include "Sample.h"
GAMERUN

Sample::Sample()
{

}

bool Sample::Init()
{
	Comp1 = g_CompFactory.NewComponent<myComponent>();
	return true;
}

bool Sample::Frame()
{
	return true;
}

bool Sample::Render()
{
	return true;
}

bool Sample::Release()
{
	SAFE_DELETE(Comp1);
	return true;
}
