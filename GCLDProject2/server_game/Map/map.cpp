#include "map.h"
#include "../object/Player.h"
#include "memory_buffer/NetPack.h"

#define Map_Data_Sync 1

iMap::iMap(short mapID, char typ, string name)
    : id(mapID)
    , m_type(typ)
    , m_name(name)
{

}
void iMap::Update(){

	// 调地图脚本
	if (!m_script.empty())
	{
		//RunScript(m_script, "Update", m_name, m_type);
	}
}
bool iMap::EnterMap(MapObj* pObj){
    if (pObj == NULL) return false;

    m_setPlayer.insert(pObj);

	// 先同步地图变量
	Pack_ArrayData pack(Map_Data_Sync);
	pack.FillMsg(Pack_ArrayData::MapData, m_syncData);
    pObj->Send(pack);

    // 再同步阵营变量
    uint8 nCamp = pObj->GetCamp();
    if (nCamp < MAX_CAMP_IN_MAP)
    {
        MapCampInfo& rCampInfo = m_arrCampInfo[nCamp];
        rCampInfo.m_setPlayer.insert(pObj);

        pack.FillMsg(Pack_ArrayData::CampData, rCampInfo.m_syncData);
        pObj->Send(pack);
    }

    // 调地图脚本
    if (!m_script.empty())
    {
        //RunScript(m_script, "OnEnterMap", m_name, m_type, pObj->m_accountId);
    }

    return false;
}
void iMap::ExitMap(MapObj* pObj){
    if (pObj == NULL) return;

    m_setPlayer.erase(pObj);

    // 从阵营移除
    uint8 nCamp = pObj->GetCamp();
    if ( nCamp < MAX_CAMP_IN_MAP)
    {
        MapCampInfo& rCampInfo = m_arrCampInfo[nCamp];
        rCampInfo.m_setPlayer.erase(pObj);
    }

    // 调地图脚本
    if (!m_script.empty())
    {
        //RunScript(m_script, "OnExitMap", m_name, m_type, pObj->m_accountId);
    }
}
void iMap::SendMsgAll(NetPack& pack)
{
	for (std::set<MapObj*>::iterator it = m_setPlayer.begin(); it != m_setPlayer.end(); ++it)
    {
		if (MapObj* p = *it)
		{
			p->Send(pack);
		}
    }
}


void iMap::SetMapData(uint8 n, int v)
{
    if (m_syncData.Change(n, v))
    {
		Pack_ArrayData pack(Map_Data_Sync);
		pack.FillMsg(Pack_ArrayData::MapData, m_syncData, n);
        SendMsgAll(pack);
    }
}
void iMap::AddMapData(uint8 n, int v)
{
    SetMapData(n, m_syncData[n] + v);
}
void iMap::ClearMapData()
{
    if (m_syncData.Clear())
    {
		Pack_ArrayData pack(Map_Data_Sync);
		pack.FillMsg(Pack_ArrayData::MapData, m_syncData);
        SendMsgAll(pack);
    }
}
void iMap::SetMapParam(const char* szParam, int v)
{
    if (szParam) m_mapParam[szParam] = v;
}
void iMap::AddMapParam(const char* szParam, int v)
{
    if (szParam) m_mapParam[szParam] += v;
}
int iMap::GetMapParam(const char* szParam) const{
    if (szParam) {
		std::map<string, int>::const_iterator it = m_mapParam.find(szParam);
        if (it != m_mapParam.end()){
            return it->second;
        }
    }
    return 0;
}
void iMap::SetMapStrParam(const char* szParam, const char* szValue)
{
    if (szParam && szValue) m_mapStrParam[szParam] = szValue;
}
const char* iMap::GetMapStrParam(const char* szParam) const{
    if (szParam){
        std::map<string, string>::const_iterator it = m_mapStrParam.find(szParam);
        if (it != m_mapStrParam.end()){
            return it->second.c_str();
        }
    }
    return "";
}

void iMap::SetCampData(uint8 nCamp, uint8 n, int v){
    if (nCamp < MAX_CAMP_IN_MAP){
        MapCampInfo& rCampInfo = m_arrCampInfo[nCamp];
        if (rCampInfo.m_syncData.Change(n, v))
        {
			Pack_ArrayData pack(Map_Data_Sync);
			pack.FillMsg(Pack_ArrayData::CampData, m_syncData, n);
            rCampInfo.SendMsgAll(pack);
        }
    }
}
int iMap::GetCampData(uint8 nCamp, uint8 n) const{
    if (nCamp < MAX_CAMP_IN_MAP)
    {
        return m_arrCampInfo[nCamp].m_syncData[n];
    }
    return 0;
}
void iMap::AddCampData(uint8 nCamp, uint8 n, int v)
{
    int nData = GetCampData(nCamp, n);
    SetCampData(nCamp, n, nData + v);
}
void iMap::ClearCampData(uint8 nCamp){
    if (nCamp < MAX_CAMP_IN_MAP){
        MapCampInfo& rCampInfo = m_arrCampInfo[nCamp];
        if (rCampInfo.m_syncData.Clear())
        {
			Pack_ArrayData pack(Map_Data_Sync);
			pack.FillMsg(Pack_ArrayData::CampData, m_syncData);
            rCampInfo.SendMsgAll(pack);
        }
    }
}
void iMap::SetCampParam(uint8 nCamp, const char* szParam, int v){
    if (nCamp < MAX_CAMP_IN_MAP && szParam)
    {
        m_arrCampInfo[nCamp].m_mapParam[szParam] = v;
    }
}
int iMap::GetCampParam(uint8 nCamp, const char* szParam) const{
    if (nCamp < MAX_CAMP_IN_MAP && szParam){
        const std::map<string, int>& rMap = m_arrCampInfo[nCamp].m_mapParam;
		std::map<string, int>::const_iterator it = rMap.find(szParam);
        if (it != rMap.end()){
            return it->second;
        }
    }
    return 0;
}
void iMap::AddCampParam(uint8 nCamp, const char* szParam, int v){
    if (nCamp < MAX_CAMP_IN_MAP && szParam){
        std::map<string, int>& rMap = m_arrCampInfo[nCamp].m_mapParam;
        rMap[szParam] += v;
    }
}
void iMap::ClearCampParam(uint8 nCamp)
{
    if (nCamp < MAX_CAMP_IN_MAP)
    {
        m_arrCampInfo[nCamp].Clear();
    }
}
void MapCampInfo::Clear()
{
    m_setPlayer.clear();
    m_syncData.Clear();
    m_mapParam.clear();
}
void MapCampInfo::SendMsgAll(NetPack& pack)
{
	for (std::set<MapObj*>::iterator it = m_setPlayer.begin(); it != m_setPlayer.end(); ++it)
	{
		if (MapObj* p = *it)
		{
			p->Send(pack);
		}
	}
}