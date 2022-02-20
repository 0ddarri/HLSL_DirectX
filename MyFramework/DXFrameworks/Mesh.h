#pragma once
#include "Base.h"

class Mesh : Base
{
private:
	LPD3DXMESH gSphere;
	LPD3DXMESH gSphere_X;

	std::list<LPD3DXMESH> meshlist;
protected:
public:
	Mesh();

	LPD3DXMESH LoadMesh(std::wstring path);
	LPD3DXMESH LoadSphere(float r, float slice, float stack);

	void Initialize();

	void Render();

	void Release();
};

