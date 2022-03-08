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

LPDIRECT3DCUBETEXTURE9 Texture::LoadCubeMapTexture(std::wstring path)
{
	LPDIRECT3DCUBETEXTURE9 cubeTexture;

	auto isFail = D3DXCreateCubeTextureFromFile(DEVICE, path.c_str(), &cubeTexture);
	if (FAILED(isFail))
	{
		std::wcout << "CubeMap Load Fail : " << path << std::endl;
		return nullptr;
	}
	std::wcout << "CubeMap Load Success : " << path << std::endl;
	cubeMapList.push_back(cubeTexture);
	return cubeTexture;
}

void Texture::LoadTextures()
{
	earthDiffuse = LoadTexture(L"res/texture/Earth.jpg");

	brickDiffuse = LoadTexture(L"res/texture/Fieldstone.tga");
	brickSpecular = LoadTexture(L"res/texture/specular.jpg");
	brickNormal = LoadTexture(L"res/texture/normal.tga");

	en_Snow = LoadCubeMapTexture(L"res/texture/Snow_ENV.dds");
}

void Texture::Release()
{
	for (LPDIRECT3DTEXTURE9 it : texturelist)
	{
		it->Release();
	}
	texturelist.clear();

	for (LPDIRECT3DCUBETEXTURE9 it : cubeMapList)
	{
		it->Release();
	}
	cubeMapList.clear();
}
