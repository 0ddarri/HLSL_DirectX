#pragma once
#include "Base.h"

class Mesh : Base
{
private:
	LPD3DXMESH gSphere;
	LPD3DXMESH gSphere_X;
	LPD3DXMESH gTeapot_X;
	LPD3DXMESH gSphere_Tangent;

	std::list<LPD3DXMESH> meshlist;

	int meshSwitch = 'Q';
protected:
public:
	Mesh();

	LPD3DXMESH LoadMesh(std::wstring path);
	LPD3DXMESH LoadSphere(float r, float slice, float stack);

	void Initialize();

	void Update(float deltaTime);
	void Render();

	void Release();
};

