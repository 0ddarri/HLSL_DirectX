#pragma once
#include "Base.h"

class Mesh : Base
{
private:
	LPD3DXMESH mesh;
protected:
public:
	Mesh();

	LPD3DXMESH LoadMesh(LPD3DXMESH m);
	void LoadSphere(float r, float slice, float stack);

	void Render();

	void Release();
};

