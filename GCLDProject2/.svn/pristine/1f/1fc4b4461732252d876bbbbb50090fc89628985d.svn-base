#pragma once

#include "def/TypeDef.h"
#include "boost/function.hpp"

class City;
class CityMgr {
public:
    std::map<uint16, City*> m_cityLst;
public:
    static CityMgr& Instance(){ static CityMgr T; return T; }

    void ForEach(boost::function<void(City*)> func);

    void BuildEdgeCityLst(std::vector<City*>& lst);
    void ResetEncircledCity(); //服务器宕机，重建城池的“包围”状态

    City* GetCity(uint16 id);
    bool HavePartner(uint16 id, uint16 exceptId = 0);
};
#define sCityMgr CityMgr::Instance()
