#pragma once

#include <map>

class iMap;
class MapMgr {
    typedef std::map<short, iMap*>::iterator MapIt;
public:
	MapMgr();
	static MapMgr& Instance(){ static MapMgr T; return T; }
	iMap* Create(char typ, std::string name);
    iMap* GetMapByID(short id);
    void Update();
private:
    short m_curIncrementID;
	std::map<short, iMap*> m_mapMaps;
};
