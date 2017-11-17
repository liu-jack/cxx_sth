#pragma once
#include <map>

class CMap;
class MapMgr {
    typedef std::map<int, CMap*>::iterator MapIt;
    MapMgr();
public:
	static MapMgr& Instance(){ static MapMgr T; return T; }
    //BaseMap* Create(int typ);
    CMap* GetMapByID(short id);
private:
    int m_curIncrementID;
    std::map<int, CMap*> m_mapMaps;
};
