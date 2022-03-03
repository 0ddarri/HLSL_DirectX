#include "DXUT.h"
#include "Texture.h"

LPDIRECT3DTEXTURE9 Texture::LoadTexture(std::wstring path)
{
	LPDIRECT3DTEXTURE9 texture;

	auto isFail = D3DXCreateTextureFromFile(DEVICE, path.c_str(), &texture);
	if (FAILED(isFail))
	{
		std::wcout << "Texture Load Fail : " << path.c_str() << std::endl;
		delete texture;
		return nullptr;
	}
	std::wcout << "Texture Load Success : " << path.c_str() << std::endl;
	texturelist.push_back(texture);
	return texture;
}

void Texture::LoadTextures()
{
	earthDiffuse = LoadTexture(L"res/texture/Earth.jpg");

	brickDiffuse = LoadTexture(L"res/texture/Fieldstone.tga");
	brickSpecular = LoadTexture(L"res/texture/specular.jpg");
	brickNormal = LoadTexture(L"res/texture/normal.tga");
}

void Texture::Release()
{
	for (LPDIRECT3DTEXTURE9 it : texturelist)
	{
		it->Release();
	}
	texturelist.clear();
}
