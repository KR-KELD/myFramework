#include "myComponent.h"
DECLARE_COMPONENT(myComponent)

bool myComponent::Init()
{
	return true;
}

bool myComponent::PreFrame()
{
	return true;
}

bool myComponent::Frame()
{
	return true;
}

bool myComponent::PostFrame()
{
	return true;
}

bool myComponent::PreRender(ID3D11DeviceContext* pd3dContext)
{
	return true;
}

bool myComponent::Render(ID3D11DeviceContext* pd3dContext)
{
	return true;
}

bool myComponent::PostRender(ID3D11DeviceContext* pd3dContext)
{
	return true;
}

void myComponent::Update(ID3D11DeviceContext* pd3dContext)
{
}
