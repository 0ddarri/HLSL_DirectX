#pragma once
class Texture
{

private:
	LPDIRECT3DTEXTURE9 LoadTexture(std::wstring path);
	std::list<LPDIRECT3DTEXTURE9> texturelist;
protected:
public:
	LPDIRECT3DTEXTURE9 earthDiffuse;

	LPDIRECT3DTEXTURE9 brickDiffuse;
	LPDIRECT3DTEXTURE9 brickSpecular;
	LPDIRECT3DTEXTURE9 brickNormal;

	void LoadTextures();
	void Release();
};

