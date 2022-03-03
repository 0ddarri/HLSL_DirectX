#include "DXUT.h"
#include "Shader.h"

LPD3DXEFFECT Shader::LoadShader(std::wstring path)
{
	LPD3DXEFFECT shader;
	LPD3DXBUFFER errormsg = nullptr;
	auto isFail = D3DXCreateEffectFromFile(DEVICE, path.c_str(), NULL, NULL, D3DXSHADER_DEBUG, NULL, &shader, &errormsg);
	if (FAILED(isFail))
	{
		std::wcout << "SHADER FAIL : " << path.c_str() << std::endl;
		MessageBox(DXUTGetHWND(), static_cast<WCHAR*>(errormsg->GetBufferPointer()), L"Shader Create Error", MB_OK);
		return nullptr;
	}
	std::wcout << "SHADER LOAD : " << path.c_str() << std::endl;
	effectlist.push_back(shader);
	return shader;
}

void Shader::LoadShaders()
{
	colorShader = LoadShader(L"res/shader/ColorShader.fx");
	lightShader = LoadShader(L"res/shader/light.fx");
	textureMappingShader = LoadShader(L"res/shader/mapping.fx");
	specularlightShader = LoadShader(L"res/shader/specularLight.fx");
	specularMappingShader = LoadShader(L"res/shader/MySpecularMapping_matrixMix.fx");
	toonShader = LoadShader(L"res/shader/toonShader.fx");
	normalMappingShader = LoadShader(L"res/shader/NormalMapping.fx");
}

void Shader::Release()
{
	for (LPD3DXEFFECT effect : effectlist)
	{
		effect->Release();
	}
}
