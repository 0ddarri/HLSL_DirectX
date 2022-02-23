#pragma once
class Shader
{
private:
	std::list<LPD3DXEFFECT> effectlist;
	LPD3DXEFFECT LoadShader(std::wstring path);
public:
	LPD3DXEFFECT colorShader;
	LPD3DXEFFECT textureMappingShader;
	LPD3DXEFFECT lightShader;
	LPD3DXEFFECT specularlightShader;
	LPD3DXEFFECT specularMappingShader;

	void LoadShaders();
	void Release();
};

