#pragma once
class Texture
{

private:
	LPDIRECT3DTEXTURE9 LoadTexture(std::wstring path);
	LPDIRECT3DCUBETEXTURE9 LoadCubeMapTexture(std::wstring path);
	std::list<LPDIRECT3DTEXTURE9> texturelist;
	std::list<LPDIRECT3DCUBETEXTURE9> cubeMapList;
protected:
public:
	LPDIRECT3DTEXTURE9 earthDiffuse;
	LPDIRECT3DTEXTURE9 brickDiffuse;
	LPDIRECT3DTEXTURE9 brickSpecular;
	LPDIRECT3DTEXTURE9 brickNormal;

	LPDIRECT3DCUBETEXTURE9 en_Snow;

	void LoadTextures();


	void Release();
};

