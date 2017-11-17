#include "CityMgr.h"
#include "city.h"
#include "../Table/MapTableMgr.h"
#include "../Table/MapCity.h"


void CityMgr::ForEach(boost::function<void(City*)> func)
{
    for (std::map<uint16, City*>::iterator it = m_cityLst.begin();
        it != m_cityLst.end(); ++it)
    {
        func(it->second);
    }
}
City* CityMgr::GetCity(uint16 id)
{
    std::map<uint16, City*>::iterator it = m_cityLst.find(id);
    if (it != m_cityLst.end())
    {
        return it->second;
    }
    return NULL;
}
bool CityMgr::HavePartner(uint16 id, uint16 exceptId /* = 0 */)
{
    const uint8 countryId = sCityMgr.GetCity(id)->GetCountryId();
    const MapCity* table = sMapTableMgr.GetMapCity(id);
    for (size_t i = 0; i < table->m_linkCity.size(); ++i)
    {
        const uint16 linkId = table->m_linkCity[i];

        if (linkId == exceptId) continue;

        if (sCityMgr.GetCity(linkId)->GetCountryId() == countryId)
        {
            return true;
        }
    }
    return false;
}
void CityMgr::BuildEdgeCityLst(std::vector<City*>& lst)
{
    for (std::map<uint16, City*>::iterator it = m_cityLst.begin(); it != m_cityLst.end(); ++it)
    {
        if (it->second->IsCountryEdge())
        {
            lst.push_back(it->second);
        }
    }
}
void CityMgr::ResetEncircledCity()
{
    for (std::map<uint16, City*>::iterator it = m_cityLst.begin(); it != m_cityLst.end(); ++it)
    {
        it->second->TryEncircled();
    }
}