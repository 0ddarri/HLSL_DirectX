#pragma once
#include "Singleton.h"
#include "Base.h"

class Resources : public Singleton<Resources>
{
private:
	std::list<Base*> m_resourceList;
	std::list<LPD3DXMESH*> m_meshlist;
public:
	void AddMeshList(LPD3DXMESH* object);
	void DeleteMeshList(LPD3DXMESH* object);

	void Render();
	void Release();
};