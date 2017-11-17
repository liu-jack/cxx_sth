#include "MapMgr.h"
#include "map.h"

MapMgr::MapMgr() : m_curIncrementID(0)
{

}
iMap* MapMgr::Create(char typ, std::string name)
{
   /* switch (typ){
    default: return new iMap(++m_curIncrementID, typ, name);
    }*/

	return new iMap(++m_curIncrementID, typ, name);
}
iMap* MapMgr::GetMapByID(short id)
{
    MapIt it = m_mapMaps.find(id);
    if (it != m_mapMaps.end())
    {
        return it->second;
    }
    return NULL;
}
void MapMgr::Update()
{
    for (MapIt it = m_mapMaps.begin(); it != m_mapMaps.end(); ++it)
	{
        if (it->second) it->second->Update();
	}
}