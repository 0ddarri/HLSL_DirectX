#pragma once
class Shader
{
private:
	std::list<LPD3DXEFFECT> effectlist;
	LPD3DXEFFECT LoadShader(std::wstring path);
public:
	LPD3DXEFFECT colorShader;
	LPD3DXEFFECT lightShader;
	void LoadShaders();
	void Release();
};

