#include "DXUT.h"
#include "Mesh.h"
#include "Resources.h"

Mesh::Mesh()
{
}

LPD3DXMESH Mesh::LoadMesh(LPD3DXMESH m)
{
	return LPD3DXMESH();
}

void Mesh::LoadSphere(float r, float slice, float stack)
{
	LPD3DXMESH sphere;
	auto isFail = D3DXCreateSphere(DEVICE, r, slice, stack, &sphere, NULL);
	if (FAILED(isFail))
	{
		std::wcout << "Sphere Load Fail" << std::endl;
		return;
	}
	std::wcout << "Sphere Load Success" << std::endl;

	Resources::GetIns()->AddMeshList(&sphere);
	mesh = sphere;
}

void Mesh::Render()
{
	mesh->DrawSubset(0);
}

void Mesh::Release()
{
	mesh->Release();
	mesh = nullptr;
}
