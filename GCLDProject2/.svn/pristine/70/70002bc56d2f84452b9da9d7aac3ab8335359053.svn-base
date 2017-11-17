#include "MapMgr.h"
#include "BaseMap.h"

MapMgr::MapMgr() : m_curIncrementID(0)
{

}
//BaseMap* MapMgr::Create(int typ)
//{
//   /* switch (typ){
//    default: return new BaseMap(++m_curIncrementID, typ, name);
//    }*/
//
//    return new BaseMap(++m_curIncrementID, typ);
//}
CMap* MapMgr::GetMapByID(short id)
{
    MapIt it = m_mapMaps.find(id);
    if (it != m_mapMaps.end())
    {
        return it->second;
    }
    return NULL;
}
