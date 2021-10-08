#pragma once
#include "myStd.h"
class myTexture
{
public:
	ComPtr<ID3D11ShaderResourceView> m_pTextureSRV;
public:
	bool  Load(T_STR strTex);
public:
	myTexture();
	~myTexture();
};

