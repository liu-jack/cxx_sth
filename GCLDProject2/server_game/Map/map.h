#pragma once

#include "def/TypeDef.h"
#include "MapData.h"
#include "memory_buffer/MemoryPool.h"
#include "boost/function.hpp"

class Player;
class NetPack;
typedef Player MapObj;
struct MapCampInfo {
    ArrayData m_syncData; //Ҫͬ����client������
	std::set<MapObj*> m_setPlayer;
	std::map<string, int> m_mapParam;

    void Clear();
    void SendMsgAll(NetPack& pack);
};

class iMap {
    DECLARE_OBJ_POOL(iMap)
public:
    iMap(short mapID, char typ, string name);
    virtual ~iMap(){}
    virtual void Update();
    virtual bool EnterMap(MapObj* pObj);
    virtual void ExitMap(MapObj* pObj);

    void SendMsgAll(NetPack& pack);

//--------------------------------------------------------------------
    //������������lua
    void SetMapData(uint8 n, int v);
    void AddMapData(uint8 n, int v);
    int GetMapData(uint8 n) const { return m_syncData[n]; }
    void ClearMapData();

    void SetMapParam(const char* szParam, int v);
    void AddMapParam(const char* szParam, int v);
    int GetMapParam(const char* szParam) const;
    void ClearMapParam(){ m_mapParam.clear(); }

    void SetMapStrParam(const char* szParam, const char* szValue);
    const char* GetMapStrParam(const char* szParam) const;
    void ClearMapStrParam(){ m_mapStrParam.clear(); }

    void SetCampData(uint8 nCamp, uint8 n, int v);
    void AddCampData(uint8 nCamp, uint8 n, int v);
    int GetCampData(uint8 nCamp, uint8 n) const;
    void ClearCampData(uint8 nCamp);

    void SetCampParam(uint8 nCamp, const char* szParam, int v);
    void AddCampParam(uint8 nCamp, const char* szParam, int v);
    int GetCampParam(uint8 nCamp, const char* szParam) const;
    void ClearCampParam(uint8 nCamp);
//--------------------------------------------------------------------
public:
    void ForEachPlayer(const boost::function<void(MapObj*)> func)
    {
		for (std::set<MapObj*>::iterator it = m_setPlayer.begin(); it != m_setPlayer.end(); ++it)
		{
			func(*it);
		}
    }
    void ForEachCampPlayer(uint8 nCamp, const boost::function<void(MapObj*)> func){
        if (nCamp < MAX_CAMP_IN_MAP)
        {
            std::set<MapObj*>& set = m_arrCampInfo[nCamp].m_setPlayer;
			for (std::set<MapObj*>::iterator it = set.begin(); it != set.end(); ++it)
			{
				func(*it);
			}
        }
    }
    MapObj* FindPlayer(const boost::function<bool(MapObj*)> func)
    {
		for (std::set<MapObj*>::iterator it = m_setPlayer.begin(); it != m_setPlayer.end(); ++it)
		{
			if (func(*it)) return *it;
		}
        return NULL;
    }
protected:
    const short	id;
    char	m_type;
    string	m_name;	    // ��ͼ��
    string	m_script;	// ��ͼ�ű��ļ���
	std::set<MapObj*> m_setNpc; //TODO���о�npc�����Ǹ��������ɵ�ͼ�����͹��ˣ�������������˵
	std::set<MapObj*> m_setPlayer;

    ArrayData m_syncData;   // Ҫͬ���ĵ�ͼ����
    std::map<string, int>       m_mapParam;
    std::map<string, string>    m_mapStrParam;

    static const uint8 MAX_CAMP_IN_MAP = 6;	// ��ͼ֧�ֵ������Ӫ�������[0,n-1]
    MapCampInfo m_arrCampInfo[MAX_CAMP_IN_MAP];
};