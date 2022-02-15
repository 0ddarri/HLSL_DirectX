#include "DXUT.h"
#include "Mesh.h"
#include "Resources.h"

Mesh::Mesh()
{
}

LPD3DXMESH Mesh::LoadMesh(std::wstring path)
{
	LPD3DXMESH mesh;

	auto isFail = D3DXLoadMeshFromX(path.c_str(), D3DXMESH_SYSTEMMEM, DEVICE, NULL, NULL, NULL, NULL, &mesh);
	if (FAILED(isFail))
	{
		std::wcout << "Mesh Load Failed : " << path.c_str() << std::endl;
		delete mesh;
		return nullptr;
	}
	std::wcout << "Mesh Load Success : " << path.c_str() << std::endl;
	meshlist.push_back(mesh);
	return mesh;
}

LPD3DXMESH Mesh::LoadSphere(float r, float slice, float stack)
{
	LPD3DXMESH sphere;
	auto isFail = D3DXCreateSphere(DEVICE, r, slice, stack, &sphere, NULL);
	if (FAILED(isFail))
	{
		std::wcout << "Sphere Load Fail" << std::endl;
		return nullptr;
	}
	meshlist.push_back(sphere);
	std::wcout << "Sphere Load Success" << std::endl;
	return sphere;
}

void Mesh::Initialize()
{
	gSphere = LoadSphere(30, 20, 20);
	gSphere_X = LoadMesh(L"res/model/sphere.x");
}

void Mesh::Render()
{
	//gSphere->DrawSubset(0);
	gSphere_X->DrawSubset(0);
}

void Mesh::Release()
{
	for (LPD3DXMESH mesh : meshlist)
	{
		mesh->Release();
	}
	meshlist.clear();
}
