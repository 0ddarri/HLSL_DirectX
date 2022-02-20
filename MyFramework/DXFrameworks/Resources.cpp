#include "DXUT.h"
#include "Resources.h"

void Resources::AddMeshList(LPD3DXMESH* object)
{
	for (auto l : m_meshlist)
	{
		if (l == object) return;
	}
	m_meshlist.push_back(object);
}

void Resources::DeleteMeshList(LPD3DXMESH* object)
{
	for (auto l : m_meshlist)
	{
		if (l == object)
		{
			m_meshlist.remove(object);
			return;
		}
	}
}

void Resources::Release()
{
	for (LPD3DXMESH* l : m_meshlist)
	{
		delete(l);
	}
	m_meshlist.clear();
}
